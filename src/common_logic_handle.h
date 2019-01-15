/*
 * common_handle_http.h
 *
 *  Created on: Jan 13, 2019
 *      Author: root
 */

#ifndef SRC_COMMON_LOGIC_HANDLE_H_
#define SRC_COMMON_LOGIC_HANDLE_H_

#include "common_structs.h"

namespace http{
namespace server{
//
//struct request;
//struct reply;
class common_logic_handle
{
public:
	common_logic_handle(){}
	virtual ~common_logic_handle(){}
public:
	virtual bool handle(const request& req, reply& rep) = 0;
};
}
}



#endif /* SRC_COMMON_LOGIC_HANDLE_H_ */
