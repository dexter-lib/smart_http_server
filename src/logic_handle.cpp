/*
 * logichandle.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: root
 */

#include "logic_handle.h"

#include <boost/foreach.hpp>

logic_handle::logic_handle() {
	// TODO Auto-generated constructor stub

}

logic_handle::~logic_handle() {
	// TODO Auto-generated destructor stub
}

bool logic_handle::handle(const http::server::request& req, http::server::reply& rep)
{
	LOG_TRACE("method %s", req.method.c_str());
	LOG_TRACE("body:%s", req.content.c_str());

	BOOST_FOREACH(http::server::header h, req.headers)
	{
		LOG_TRACE("header name:%s, value:%s", h.name.c_str(), h.value.c_str());
	}

	BOOST_FOREACH(http::server::param p, req.params)
	{
		LOG_TRACE("param name:%s, value:%s", p.name.c_str(), p.value.c_str());
	}

	rep = http::server::reply::stock_reply(http::server::reply::ok);

	return true;
}

