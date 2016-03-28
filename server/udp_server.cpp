#include "udp_server.h"


static void print_log(string _log)
{
	cerr << _log << endl;
}

udp_server::udp_server(unsigned short _port)
	:port(_port)
{
	sock = -1;
}

bool udp_server::register_user(const struct sockaddr_in &client, string& _msg)
{
	string _ip_key = inet_ntoa(client.sin_addr);
	map<string, struct sockaddr_in>::iterator _iter = online_user.find(_ip_key);
	udp_data _data;
	_data.str_to_val(_msg);
	string _cmd;
	_data.get_cmd(_cmd);
	if(_cmd == "MESSAGE")
	{
	if(_iter != online_user.end())
	{
		return false;
	}
    online_user.insert(pair<string, struct sockaddr_in>(_ip_key, client));
	}
	else if(_cmd == "CMD-QUIT")
	{
		online_user.erase(_ip_key);
	}

	return true;
}
//if success,return 0
//else errno
int udp_server::init_server(int port)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{		
		print_log(strerror(errno));
		exit(1);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		print_log(strerror(errno));
		exit(1);
	}
	return 0;
}

int udp_server::reliable_recv_msg()
{
	string _out_msg;
	int ret = recv_msg(_out_msg);
	if(ret > 0)
	{
		pool.put_msg(_out_msg);
	}
	return ret;
}

int udp_server::recv_msg(string &_out_msg)
{
	char buf[G_BLOCK_SIZE];
	memset(buf, '\0', sizeof(buf));
	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	ssize_t _size = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&client, &len);


	if(_size < 0)
	{
		print_log(strerror(errno));
		_out_msg = "";
		return 1;
	}
	else if( _size >= 0)
	{
		_out_msg = buf;
		register_user(client, _out_msg);
	}
	return _size;
}

int udp_server::reliable_send_msg(const string &_in_msg, const struct sockaddr_in& client, socklen_t len)
{
	//method
	return send_msg(_in_msg, client, len);
}

int udp_server::send_msg(const string &_in_msg, const struct sockaddr_in& client, socklen_t len)
{
	ssize_t _size = sendto(sock, _in_msg.c_str(), _in_msg.size(), 0, (struct sockaddr*)&client, len);

	if(_size < 0)
	{
		print_log(strerror(errno));
		return 1;
	}
	else
	{

	}
	return 0;
}


int udp_server::broadcast_msg()
{
	string msg;
	pool.get_msg(msg);
	map<string, struct sockaddr_in>::iterator _iter = online_user.begin();
	for(; _iter != online_user.end(); _iter++)
	{
		reliable_send_msg(msg, _iter->second, sizeof(_iter->second));
	}
}

udp_server::~udp_server()
{
	if(sock != -1)
	{
		close(sock);
	}
}
