// libeventClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>

#include<event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/util.h>


#include "base_type.h"
#include "common_marco.h"
#include "NetMessage.h"
#include "business.h"

#include "Message.h"
#include "platform.pb.h"

using namespace LW;


#define BUF_SIZE	1024

static const char SEND_MESSAGE[] = "{\"data\":\"1111111111\"}";

static void read_cb(struct bufferevent* bev, void* arg);
static void event_cb(struct bufferevent *bev, short event, void *arg);
static void time_cb(evutil_socket_t fd, short event, void *arg);


static lw_int32 send_socket_data(struct bufferevent *bev, lw_int32 cmd, void* object, lw_int32 objectSize);
static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata);

struct CLIENT
{
	struct event timer;
	struct bufferevent* bev;
	bool t;
};

static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata)
{
	struct bufferevent *bev = (struct bufferevent *)userdata;
	switch (cmd)
	{
	case 100:
	{
		platform::csc_msg_heartbeat msg;
		msg.ParseFromArray(buf, bufsize);
		printf(" order: %d\n", msg.order());
	}break;
	case 20001:
	{
		platform::sc_msg_userinfo userinfo;
		userinfo.ParseFromArray(buf, bufsize);
		printf(" userid : %d\n age: %d\n sex: %d\n name: %s\n address: %s\n", userinfo.userid(),
			userinfo.age(), userinfo.sex(), userinfo.name().c_str(), userinfo.address().c_str());
	}break;
	default:
		break;
	}
}

static lw_int32 send_socket_data(struct bufferevent *bev, lw_int32 cmd, void* object, lw_int32 objectSize)
{
	LW_NET_MESSAGE* p = lw_create_net_message(cmd, object, objectSize);

	lw_int32 result = bufferevent_write(bev, p->buf, p->size);
	lw_free_net_message(p);

	return result;
}


static void time_cb(evutil_socket_t fd, short event, void *arg)
{
	struct CLIENT* client = (CLIENT*)arg;
	if (client->t)
	{
		// ���ö�ʱ���ص����� 
		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		// ����¼�  
		event_add(&client->timer, &tv);

		{
			platform::csc_msg_heartbeat msg;
			msg.set_order(100);

			int len = (int)msg.ByteSizeLong();
			char s[256] = { 0 };
			bool ret = msg.SerializeToArray(s, len);
			if (ret) {
				send_socket_data(client->bev, 100, s, len);
			}
		}

		{
			platform::sc_msg_request_userinfo msg;
			msg.set_userid(400001);

			int len = (int)msg.ByteSizeLong();
			char s[256] = { 0 };
			bool ret = msg.SerializeToArray(s, len);
			if (ret) {
				send_socket_data(client->bev, 10001, s, len);
			}
		}
	}
}

static void read_cb(struct bufferevent* bev, void* arg)
{
	struct evbuffer *input;
	input = bufferevent_get_input(bev);
	size_t input_len = evbuffer_get_length(input);

	char *read_buf = (char*)malloc(input_len);

	size_t read_len = bufferevent_read(bev, read_buf, input_len);

	if (lw_on_parse_socket_data(read_buf, read_len, on_socket_recv, bev) == 0)
	{

	}

	free(read_buf);
}

static void event_cb(struct bufferevent *bev, short event, void *arg)
{
	struct CLIENT* item = (CLIENT*)arg;

	if (event & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		printf("some other error\n");
	}
	else if (event & BEV_EVENT_TIMEOUT)
	{
		printf("timeout ...\n");
	}
	else if (event & BEV_EVENT_CONNECTED)
	{
		item->t = true;
		printf("conneted ...\n");

		return;
	}

	//�⽫�Զ�close�׽��ֺ�free��д������  
	bufferevent_free(bev);
	item->t = false;

// 	struct event *ev = (struct event*)arg;
// 	event_free(ev);
}

int main(int argc, char** argv)
{
#if defined(WIN32) || defined(_WIN32)
	WSADATA WSAData;
	if (int ret = WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		std::cout << "Can not initilize winsock.dll" << std::endl;
		std::cout << "Error Code:" << WSAGetLastError() << std::endl;
		return 1;
	}
#endif

	struct CLIENT client = { 0 };

	struct event_base *base = event_base_new();
	if (nullptr == base)
	{
		goto exe_over;
	}

	client.bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.169");
	
	bufferevent_setcb(client.bev, read_cb, nullptr, event_cb, (void*)&client);
	
	int con = bufferevent_socket_connect(client.bev, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (con < 0)
	{
		/* error starting connection */
		bufferevent_free(client.bev);
		goto exe_over;
	}

	bufferevent_enable(client.bev, EV_READ | EV_PERSIST);

	const char* method = event_base_get_method(base);

	{
		event_assign(&client.timer, base, -1, 0, time_cb, (void*)&client);
		// ���ö�ʱ���ص����� 
		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		// ����¼�  
		event_add(&client.timer, &tv);
	}
	
	event_base_dispatch(base);
	
	event_base_free(base);
	
exe_over:
	printf("exe_over. \n");


#if defined(WIN32) || defined(_WIN32)
	WSACleanup();
#endif

	return 0;
}

