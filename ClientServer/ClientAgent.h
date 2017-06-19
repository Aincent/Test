/*
 * ClientAgent.h
 *
 *  Created on: Jun 20, 2017
 *      Author: root
 */

#ifndef CLIENTAGENT_H_
#define CLIENTAGENT_H_
#include "antlib.h"

#define CONN_GATE_INFO 	1001

void client_service_domsg(handler_msg* msg,void* g,uint32_t hid);

void client_service_doproc(void* hdata,uint32_t hid);

void client_server_init(int handler_group);

uint32_t client_server_handlerid();

int client_service_conn(int svrid,const char* ip,int port,int protocol_type);

void client_service_call(uint64_t charid,char* name);

#endif /* CLIENTAGENT_H_ */
