#pragma once
#ifndef _HTTPREQUEST_HANDLER_H_
#define _HTTPREQUEST_HANDLER_H_

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/format.hpp>

#include "common_structs.h"

namespace http {
namespace server {
//
//struct header
//{
//	std::string name;
//	std::string value;
//};
//
//struct param
//{
//	std::string name;
//	std::string value;
//};
//
//struct request
//{
//  std::string method;
//  std::string uri;
//  int http_version_major;
//  int http_version_minor;
//  std::vector<header> headers;
//  std::vector<param> params;
//  std::string content;
//};
//
//
////reply
//struct reply
//{
//	enum status_type
//	{
//		ok = 200,
//		created = 201,
//		accepted = 202,
//		no_content = 204,
//		multiple_choices = 300,
//		moved_permanently = 301,
//		moved_temporarily = 302,
//		not_modified = 304,
//		bad_request = 400,
//		unauthorized = 401,
//		forbidden = 403,
//		not_found = 404,
//		internal_server_error = 500,
//		not_implemented = 501,
//		bad_gateway = 502,
//		service_unavailable = 503
//	} status;
//
//	std::vector<header> headers;
//
//	std::string content;
//
//	std::vector<boost::asio::const_buffer> to_buffers();
//
//	static reply stock_reply(status_type status);
//};


//request_handler
class request_handler
  : private boost::noncopyable
{
public:
	explicit request_handler();
	bool handle_request(request& req, reply& rep);
private:
	static bool url_decode(const std::string& in, std::string& out);
private:
	boost::format        m_err_fmt;
};


} //namespace server
}// namespace http




#endif
