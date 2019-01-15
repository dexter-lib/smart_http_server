#pragma once
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include <set>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "http_request_handler.h"
#include "http_request_parser.h"
#include "common_logic_handle.h"
#include "common_structs.h"

namespace http {
namespace server {



// connection
class connection_manager;

class connection
	: public boost::enable_shared_from_this<connection>,
	private boost::noncopyable
{
public:
	explicit connection(boost::asio::io_service& io_service,
	connection_manager& manager,
	request_handler& handler,
	common_logic_handle *logic_handle);

	boost::asio::ip::tcp::socket& socket();
	void start();
	void stop();

private:
	void handle_read(const boost::system::error_code& e,std::size_t bytes_transferred);

	void handle_write(const boost::system::error_code& e);

	boost::asio::ip::tcp::socket socket_;

	connection_manager& connection_manager_;

	request_handler& request_handler_;

	boost::array<char, 8192> buffer_;

	request request_;

	request_parser request_parser_;

	reply reply_;

	common_logic_handle * logic_handle_;
};

typedef boost::shared_ptr<connection> connection_ptr;


// connect_manager
class connection_manager
  : private boost::noncopyable
{
public:
	void start(connection_ptr c);

	void stop(connection_ptr c);

	void stop_all();

private:
	std::set<connection_ptr> connections_;
};


//server
class httpserver: private boost::noncopyable
{
public:

	explicit httpserver(const std::string& address, const std::string& port);

	//void run();
	
	boost::asio::io_service *  get_io_service();

public:
	void set_logic_handle(common_logic_handle *logic_handle);
	void start();
private:

	void start_accept();

	void handle_accept(const boost::system::error_code& e);

	void handle_stop();

	boost::asio::io_service io_service_;

	boost::asio::signal_set signals_;

	boost::asio::ip::tcp::acceptor acceptor_;

	connection_manager connection_manager_;

	connection_ptr new_connection_;

	request_handler request_handler_;

	common_logic_handle *logic_handle_;
};

inline void httpserver::set_logic_handle(common_logic_handle *logic_handle)
{
	this->logic_handle_ = logic_handle;
}

} //namespace server
}//namespace http

#endif
