/*
 * server.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */
#include "server.h"
#include "server_config.h"
#include "ClientAgent.h"
#include <signal.h>


int* _g_server_running = 0;

void signal_proc(int sg) {
	*_g_server_running = 0;
}

//////////////////////////////////////////////////////////////////

int init_server(int* running)
{
	_g_server_running = running;
	signal(SIGHUP,SIG_IGN);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGUSR1,signal_proc);
	signal(SIGUSR2,signal_proc);
	srand((unsigned int)time(0));
	//
	void* config = read_server_config("config.ini");
	if(0 == config) {
		return -1;
	}
	//8K
	create_handler_msg_cachepool(8142,config_server_cache_poolsize(config));

	//handler
	init_handlerlib(config_server_id(config),config_handlers_count(config));
	create_handlers_group(1,config_tcp_threads_count(config));
	create_handlers_group(2,config_work_threads_count(config));
	//log
	init_loglib(2,config_log_level(config),config_log_prev(config));
	//net
	init_tcplib(1,config_tcp_handlers_count(config));
	//protocol
	init_protocollib();

	client_server_init(2);

	init_netprotocol();

	{
		int i = 0;
		if(is_config_gate(config,i) != 0)
		{
			return 1;
		}

		int svrid = config_gate_svrid(config,i);
		const char* ip = config_gate_ip(config,i);
		int port = config_gate_port(config,i);
		int protocol = config_gate_protocol(config,i);

		client_service_conn(svrid,ip,port,protocol);
	}

	close_server_config(config);
	return 0;
}


void uninit_server()
{
	uninit_tcplib();
	uninit_protocollib();
	uninit_loglib();
	uninit_handlerlib();
	destory_handler_msg_cachepool();
}


/////////////////////////////////////////////////////////////////

void start_server()
{
	start_all_handlers();
}
void stop_server()
{
	stop_all_handlers();
}
void server_timer()
{
	do_timer();
}
