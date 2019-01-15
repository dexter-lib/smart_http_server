#include "http_server.h"
#include "logic_handle.h"

#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

namespace http {
namespace server {


//connection_manager
void connection_manager::start(connection_ptr c)
{
	connections_.insert(c);
	c->start();
}

void connection_manager::stop(connection_ptr c)
{
	connections_.erase(c);
	c->stop();
}

void connection_manager::stop_all()
{
	std::for_each(connections_.begin(), connections_.end(), boost::bind(&connection::stop, _1));
	connections_.clear();
}
//end connection_manager


//connection
connection::connection(boost::asio::io_service& io_service,
		connection_manager& manager,
		request_handler& handler,
		common_logic_handle *logic_handle)
: socket_(io_service)
, connection_manager_(manager)
, request_handler_(handler)
, logic_handle_(logic_handle)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start()
{
	socket_.async_read_some(boost::asio::buffer(buffer_),
								boost::bind(&connection::handle_read, shared_from_this(),
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
	socket_.close();
}

void connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	if (!e)
	{
		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);

		if (result)
		{
		    char *pos = strstr(buffer_.data(), "\r\n\r\n");
		    if(pos != NULL)
		    {
		        //减去/r/n/r/n的长度
	            size_t content_size = bytes_transferred - (pos - buffer_.data()) - 4;
	            if(content_size > 0)
	            {
	                request_.content.assign(pos + 4, content_size);
	                //log
	            }
		    }
		    //request handle

			if(request_handler_.handle_request(request_, reply_))
			{
				logic_handle_->handle(request_, reply_);
				reply_ = reply::stock_reply(reply_.ok);
			}
			boost::asio::async_write(socket_, reply_.to_buffers(),
								boost::bind(&connection::handle_write, shared_from_this(),
																boost::asio::placeholders::error));
		}
		else if (!result)
		{
			reply_ = reply::stock_reply(reply::bad_request);
			boost::asio::async_write(socket_, reply_.to_buffers(),
									boost::bind(&connection::handle_write, shared_from_this(),
													boost::asio::placeholders::error));
		}
		else
		{
			socket_.async_read_some(boost::asio::buffer(buffer_),
								boost::bind(&connection::handle_read, shared_from_this(),
																	boost::asio::placeholders::error,
																	boost::asio::placeholders::bytes_transferred));
		}
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_write(const boost::system::error_code& e)
{
	if (!e)
	{
		boost::system::error_code ignored_ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	if (e != boost::asio::error::operation_aborted)
	{
		connection_manager_.stop(shared_from_this());
	}
}
// end connection

//httpserver
httpserver::httpserver(const std::string& address, const std::string& port)
: io_service_(),
	signals_(io_service_),
	acceptor_(io_service_),
	connection_manager_(),
	new_connection_(),
	request_handler_(),
	logic_handle_(NULL)
{

#if 0
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals_.async_wait(boost::bind(&httpserver::handle_stop, this));
#endif

	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
}


boost::asio::io_service* httpserver::get_io_service()
{
    return &io_service_;
}


void httpserver::start()
{
	start_accept();
}

void httpserver::start_accept()
{
	new_connection_.reset(new connection(io_service_,connection_manager_, request_handler_, logic_handle_));
	acceptor_.async_accept(new_connection_->socket(),
								boost::bind(&httpserver::handle_accept, this,
												boost::asio::placeholders::error));
}

void httpserver::handle_accept(const boost::system::error_code& e)
{

	if (!acceptor_.is_open())
	{
		return;
	}

	if (!e)
	{
		connection_manager_.start(new_connection_);
	}

	start_accept();
}

void httpserver::handle_stop()
{
	acceptor_.close();
	connection_manager_.stop_all();
}

} //namespace server
}//namespace http


