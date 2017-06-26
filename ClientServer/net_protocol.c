/*
 * net_protocol.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#include "net_protocol.h"
#include "antlib.h"
#include "ClientAgent.h"
#include "protocollib/tcp_protocol.h"

void do_gate_netmsg(handler_msg* msg,uint64_t userid,uint64_t netid)
{
	uint32_t handlerid = client_server_handlerid();
	if(handlerid > 0)
		send_handler_msg(handlerid,msg);
	else
		write_log("do_gate_netmsg handlerid = %d error",handlerid);
}

void init_netprotocol()
{
	tcpmsg_regist_binary(CONN_GATE_INFO,NULL,0,do_gate_netmsg);
	tcpmsg_regist_binary(CONN_HEART_INFO,NULL,0,do_gate_netmsg);
	tcpmsg_regist_binary(TCPCONNECT_MSGID,NULL,0,do_gate_netmsg);
	tcpmsg_regist_binary(TCPCONNECT_LOST_MSGID,NULL,0,do_gate_netmsg);
}
