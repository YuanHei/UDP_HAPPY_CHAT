#include "udp_client.h"
#include "window.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <signal.h>
#include <algorithm>
#include <strstream>

chat_window win;
vector <string> list;

void* client_header(void* arg)
{
	string msg = "WELCOME TO Linux Chat System";
	win.draw_header();
	win.win_refresh(win.get_header());
	int x = 0, y;

	while(1)
	{
		int max_y, max_x;
		getmaxyx(win.get_header(), max_y, max_x);
		y = max_y/2-1;
	    win.clear_win_line(win.get_header(), y, 1);	
		win.put_str_to_win(win.get_header(), y, ++x%max_x, msg);
		sleep(1);
		win.win_refresh(win.get_header());
	}
}


void* client_output(void* arg)
{
    udp_data _client_data;
    udp_client* client = (udp_client*)arg;

	string _msg;
	string _nick_name;
	string _school;
	string _info;
	string _cmd;

	int total = 1;

	win.draw_output();
	win.win_refresh(win.get_output());
	while(1)
	{
	    win.win_refresh(win.get_output());

		if(client->reliable_recv_msg(_msg) == 0)
		{
			_client_data.str_to_val(_msg);
			_client_data.get_nick_name(_nick_name);
			_client_data.get_school(_school);
			_client_data.get_msg(_info);
			_client_data.get_cmd(_cmd);
			if(_cmd == "MESSAGE")
			{
				string show_msg = _nick_name + "/" + _school;

		       	list.push_back(show_msg);

			    show_msg += ": "; 
				show_msg += _info;
			    total %= (LINES*3/5-1);
			    if(total == 0)
		   	    {
			 		win.clear_win_line(win.get_output(), 1, LINES*3/5);
		   		    total = 1;
		   	    }  
		   	    win.put_str_to_win(win.get_output(), total++, 1, show_msg);
		   	    win.win_refresh(win.get_output());
	     	}

			win.win_refresh(win.get_output());
		}
	}
}

void* client_list(void* arg)
{
	//list
	win.draw_friends_list();
	while(1)
	{
		int max_y,max_x;
		getmaxyx(win.get_friends_list(), max_y, max_x);

		int i = 0;
		sort(list.begin(), list.end());
		vector<string>::iterator iter = unique(list.begin(), list.end());
		list.erase(iter, list.end());
		int total = list.size();

		int page = max_y-3;

		int page_num = total/page;
		int page_mod = total%page;
		if(page_mod > 0)
		{
			page_num++;
		}
		strstream ss;
		string str_page_num;
		ss<<page_num;
		string _out_page;
        win.win_refresh(win.get_friends_list());
		for( i = 0; i < page_num; ++i){
			string str_i;
			strstream ssi;
			ssi << i+1;
			ssi >> str_i;		
			_out_page = str_i+"/"+str_page_num;
			win.clear_win_line(win.get_friends_list(), max_y-2, 1);
			win.put_str_to_win(win.get_friends_list(), max_y-2, max_x/2, _out_page);

			int y = 0;
			for(int j=0; j < page; ++j)
			{
				y++;
				y = y%(max_y-3);
				int index = i*page+j;
				if( index < total)
				{
					string _fri = list[index];
					if(y == 0){
						win.win_refresh(win.get_friends_list());
						sleep(3);
						win.clear_win_line(win.get_friends_list(), 1, max_y-4);
						continue;
					}
					win.put_str_to_win(win.get_friends_list(), y, 2, _fri);
				}
				else
				{
					break;
				}
			}
		}
	    win.win_refresh(win.get_friends_list());
		usleep(10000);
	}
}

void* client_input(void* arg)
{
	win.draw_input();
    win.win_refresh(win.get_input());

	string _nick_name;
	string _school;
	string _cmd = "MESSAGE";
	string send_string;


	udp_client* client = (udp_client*)arg;
	udp_data _client_data;

	string message = "Please Enter : ";
	string _client_msg;

	win.put_str_to_win(win.get_input(), 1, 2, "Please input nick_name:");
    win.win_refresh(win.get_input());
	win.get_str_from_win(win.get_input(), _nick_name);
    win.win_refresh(win.get_input());

	win.put_str_to_win(win.get_input(), 2, 2, "Please input school:");
    win.win_refresh(win.get_input());
	win.get_str_from_win(win.get_input(), _school);
    win.win_refresh(win.get_input());

	win.clear_win_line(win.get_input(), 1, 2);
    win.win_refresh(win.get_input());

	while(1)
	{
		win.put_str_to_win(win.get_input(), 1, 2, message);
        win.win_refresh(win.get_input());
		win.get_str_from_win(win.get_input(), _client_msg);

		_client_data.set_nick_name(_nick_name);
        _client_data.set_school(_school);
		_client_data.set_msg(_client_msg);
	    _client_data.set_cmd(_cmd);

		_client_data.val_to_str(send_string);
		client->reliable_send_msg(send_string);

		win.clear_win_line(win.get_input(), 1, 1);
		usleep(10000);
	}
}

void handler(int sig)
{
	udp_client client;
	udp_data _client_data;
	string _nick_name = "ZY";
	string _school = "XAGD";
	string _client_msg = "None";
	string _cmd = "CMD-OUT";
	string send_string;

	switch(sig)
	{
		case SIGINT:
			{
				_client_data.set_nick_name(_nick_name);
	            _client_data.set_school(_school);
	        	_client_data.set_msg(_client_msg);
	          	_client_data.set_cmd(_cmd);

				_client_data.val_to_str(send_string);
		        client.reliable_send_msg(send_string);
				
				exit(1);
			}
			break;
		case SIGTERM:
            endwin();
			break;
		default:
			break;
	}
}

int main()
{
	udp_client client;
	udp_data _client_data;
	client.init_client();
	win.init();


	signal(SIGINT, handler);
	signal(SIGTERM, handler);

	win.draw_header();
	win.draw_output();
	win.draw_friends_list();
	win.draw_input();

	pthread_t header, output, list, input;
	pthread_create(&header, NULL, client_header, NULL);
	pthread_create(&output, NULL, client_output, (void*)&client);
	pthread_create(&list, NULL, client_list, NULL);
	pthread_create(&input, NULL, client_input, (void*)&client);

	pthread_join(header, NULL);
	pthread_join(output, NULL);
	pthread_join(list, NULL);
	pthread_join(input, NULL);

	return 0;
}
