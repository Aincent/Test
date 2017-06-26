/*
 * ClientAgent.c
 *
 *  Created on: Jun 20, 2017
 *      Author: root
 */
#include "ClientAgent.h"
#include "protocollib/tcp_stream.h"
#include "protocollib/tcp_protocol.h"
#include "net_packet.h"

struct client_service_config
{
	uint32_t _isuse;
	uint32_t _handlerid;
//	uint32_t _ip;
//	uint32_t _port;
//	uint32_t _type;
	uint64_t _netid;
	uint64_t _last_update_time;
};

struct client_service_config* _c_server = NULL;

void client_server_init(int handler_group)
{
	ASSERT(_c_server == NULL);
	_c_server = (struct client_service_config*)my_malloc(sizeof(struct client_service_config));
	_c_server->_last_update_time = 10;
	_c_server->_handlerid = register_handler(handler_group,"clientserver_01",client_service_domsg,client_service_doproc,NULL,_c_server);
	ASSERT(_c_server->_handlerid > 0);
}

void client_service_domsg(handler_msg* msg,void* g,uint32_t hid)
{
	switch(msg->_msgid)
	{
	case TCPCONNECT_MSGID:
		{
			struct tcp_connect_data* data = (struct tcp_connect_data*)msg->_data;
			if(data->_user_handlerid == _c_server->_handlerid)
			{
				_c_server->_netid = msg->_userid;
			}

			printf("client connect success .......netid = %ld _handlerid = %d \n", msg->_userid,data->_user_handlerid);

			break;
		}
	case TCPCONNECT_LOST_MSGID:
		{
			printf("connect error ... %d \n", hid);
			free_handler_msg(msg);
			break;
		}
	case CONN_GATE_INFO:
		{
			tcp_stream* info = (tcp_stream*)msg->_data;
			struct client_service_config* _g_server_config = (struct client_service_config*)g;
			printf("Hello ....msgid = %d  netid = %ld _handlerid = %d  hid = %d \n",msg->_msgid, info->_netid,_g_server_config->_handlerid,hid);
			int code = normal_pop_tcpstream_int32(info);
			int port = normal_pop_tcpstream_int32(info);
			uint16_t localID = normal_pop_tcpstream_int16(info);
			int len = normal_pop_tcpstream_int32(info);
			char ip[32] = {0};
			pop_tcpstream_str(info,ip,len);
			len = normal_pop_tcpstream_int32(info);
			char name[32] = {0};
			pop_tcpstream_str(info,name,len);

			printf("code = %d \n",code);
			printf("name = %s \n", name);
			printf("port = %d \n", port);
			printf("localID = %d \n", localID);
			printf("%d \n", len);
			printf("ip = %s \n", ip);
			uint64_t charid = 0;
			printf("charid =  %ld \n", charid);
			if(_g_server_config->_netid > 0)
			{
				info->_len = sprintf(info->_buf,"{\"msg\":%s,\"charid\":%ld,\"ip\":%s,\"port\":%d,\"localid\":%d}",name,charid,ip,port,localID);
				msg->_msgid = 0;
				tcp_sendmsg(_g_server_config->_netid,msg);
			}

			break;
		}
	case CONN_HEART_INFO:
		{
			tcp_stream* info = (tcp_stream*)msg->_data;
			struct client_service_config* _g_server_config = (struct client_service_config*)g;
			printf("heart ....msgid = %d  netid = %ld _handlerid = %d  hid = %d \n",msg->_msgid, info->_netid,_g_server_config->_handlerid,hid);
			break;
		}
	}
}

void client_service_doproc(void* hdata,uint32_t hid)
{
	uint32_t cur_time = get_mytime();
	struct client_service_config* g = (struct client_service_config*)hdata;
	if(g->_last_update_time < cur_time)
	{
		ASSERT(cur_time > 10);
		cur_time -= 10;
		client_heart_call();
		g->_last_update_time = get_mytime()+10;
	}
}

uint32_t client_server_handlerid()
{
	if(_c_server != NULL)
	{
		return _c_server->_handlerid;
	}

	return 0;
}

int client_service_conn(int svrid,const char* ip,int port,int protocol_type)
{
	struct sockaddr_in addr;
	if (ip&&ip[0])
	{
		 if( inet_pton(AF_INET,ip,&addr.sin_addr) <=0 ) {
				 printf("inet_pton error for %s\n",ip);
				 exit(0);
		 }
	}



//	tcp_packet* packet = create_packet(CONN_GATE_INFO);
//	push_packet_int32(packet,3305);
//	push_packet_end(packet);

	net_packet* packet = create_net_packet(0,0);
	push_net_packet_int8(packet,0);
	push_net_packet_int16(packet,1);
	push_net_packet_end(packet);

	conn_tcpprotocol(svrid,addr.sin_addr.s_addr,port,packet->_buf,packet->_off,0,_c_server->_handlerid,protocol_type);
	destory_net_packet(packet);
	return 0;
}


void client_heart_call()
{
	ASSERT(_c_server != NULL);
	net_packet* packet = create_net_packet(CONN_HEART_INFO,2);
	push_net_packet_end(packet);

	handler_msg* hmsg = create_handler_msg(packet->_off);
	hmsg->_datalen = packet->_off;
	memcpy(hmsg->_data,packet->_buf,packet->_off);

	destory_net_packet(packet);
	printf("client_heart_call............................\n");
	tcp_sendmsg(_c_server->_netid,hmsg);
}

void client_service_call(uint64_t charid,char* name)
{
	ASSERT(_c_server != NULL);
//	tcp_packet* packet = create_packet(CONN_GATE_INFO);
//	push_packet_int32(packet,3305);
//	push_packet_int32(packet,len);
//	push_packet_str(packet,message,len);
//	push_packet_end(packet);

	net_packet* packet = create_net_packet(CONN_GATE_INFO,2);
	push_net_packet_int64(packet,charid);
	push_net_packet_int32(packet,strlen(name));
	push_net_packet_str(packet,name,strlen(name));
	push_net_packet_end(packet);

	handler_msg* hmsg = create_handler_msg(packet->_off);
	hmsg->_datalen = packet->_off;
	memcpy(hmsg->_data,packet->_buf,packet->_off);

	destory_net_packet(packet);

	tcp_sendmsg(_c_server->_netid,hmsg);
}


