/*
 * logichandle.h
 *
 *  Created on: Jan 14, 2019
 *      Author: root
 */

#ifndef SRC_LOGIC_HANDLE_H_BAK_
#define SRC_LOGIC_HANDLE_H_BAK_

#include "common_structs.h"
#include "common_logic_handle.h"

class logic_handle: public http::server::common_logic_handle {
public:
	logic_handle();
	virtual ~logic_handle();
public:
	bool handle(const http::server::request& req, http::server::reply& rep);
};

#endif /* SRC_LOGIC_HANDLE_H_BAK_ */
