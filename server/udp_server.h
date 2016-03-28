#ifndef _UDP_SERVER_
#define _UDP_SERVER_

#include <iostream>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "comm.h"
#include "data_pool.h"
#include "udp_data.h"
using namespace std;

class udp_server
{
	public:
		udp_server(unsigned short _port = 8080);
        int init_server(int port);
        int reliable_recv_msg();
        int reliable_send_msg(const string &_in_msg, const struct sockaddr_in& client, socklen_t len);
		int broadcast_msg();
		~udp_server();
	private:
		int sock;
		unsigned short port;
		data_pool pool;
		map<string, struct sockaddr_in> online_user;
		int recv_msg(string &_out_msg);
        int send_msg(const string &_in_msg, const struct sockaddr_in& client, socklen_t len);
        bool register_user(const struct sockaddr_in& client, string& _msg);
};
   
#endif
