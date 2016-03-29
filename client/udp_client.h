#ifndef _UDP_CLIENT_
#define _UDP_CLIENT_

#include <iostream>
#include <string.h>
#include <map>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "comm.h"
#include "udp_data.h"
using namespace std;


class udp_client
{
	public:
		udp_client(string _remote_ip = "127.0.0.1", unsigned short _port = 8080);
     	int init_client();
        int reliable_recv_msg(string &_out_msg);
        int reliable_send_msg(const string &_out_msg);

		~udp_client();
		private:
		int sock;
		string remote_ip;
		unsigned short remote_port;
};

#endif
