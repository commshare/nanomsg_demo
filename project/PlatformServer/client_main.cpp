#include "client_main.h"

#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>

#include "business.h"

#include "NetCmd.h"
#include "Message.h"
#include "platform.pb.h"
#include <thread>

using namespace LW;


#ifdef _WIN32
#define LW_SLEEP(seconds) SleepEx(seconds * 1000, 1);
#else
#define LW_SLEEP(seconds) sleep(seconds);
#endif

#define BUF_SIZE	1024

static void read_cb(struct bufferevent* bev, void* arg);
static void event_cb(struct bufferevent *bev, short event, void *arg);
static void time_cb(evutil_socket_t fd, short event, void *arg);

static lw_int32 send_socket_data(struct bufferevent *bev, lw_int32 cmd, void* object, lw_int32 objectSize);
static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata);

struct CLIENT
{
	lw_int32 fd;
	struct bufferevent* bev;
	bool live;
};

struct event __g_timer;

static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata)
{
	struct bufferevent *bev = (struct bufferevent *)userdata;
	switch (cmd)
	{
	case CMD_HEART_BEAT:
	{
		platform::csc_msg_heartbeat msg;
		msg.ParseFromArray(buf, bufsize);
		printf("heartbeat: %d\n", msg.time());
	} break;
	case CMD_PLATFORM_SC_USERINFO:
	{
		platform::sc_msg_userinfo userinfo;
		userinfo.ParseFromArray(buf, bufsize);
		printf("userid: %d age:%d sex:%d name:%s address:%s\n", userinfo.userid(),
			userinfo.age(), userinfo.sex(), userinfo.name().c_str(), userinfo.address().c_str());
	}break;
	default:
		break;
	}
}

static lw_int32 send_socket_data(struct bufferevent *bev, lw_int32 cmd, void* object, lw_int32 objectSize)
{
	lw_int32 result = 0;
	{
		LW_NET_MESSAGE* p = lw_create_net_message(cmd, object, objectSize);
		result = bufferevent_write(bev, p->buf, p->size);
		lw_free_net_message(p);
	}
	return result;
}

static void time_cb(evutil_socket_t fd, short event, void *arg)
{
	struct CLIENT* client = (CLIENT*)arg;
	if (client->live)
	{
		// ���ö�ʱ���ص����� 
		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		event_add(&__g_timer, &tv);

		{
			platform::csc_msg_heartbeat msg;
			time_t t;
			t = time(NULL);
			msg.set_time(t);

			lw_int32 buf_len = (lw_int32)msg.ByteSizeLong();
			lw_char8 buf[256] = { 0 };
			bool ret = msg.SerializeToArray(buf, buf_len);
			if (ret)
			{
				send_socket_data(client->bev, CMD_HEART_BEAT, buf, buf_len);
			}
		}
	}
}

static void read_cb(struct bufferevent* bev, void* arg)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t input_len = evbuffer_get_length(input);

	{
		char *read_buf = (char*)malloc(input_len);

		size_t read_len = bufferevent_read(bev, read_buf, input_len);

		if (lw_parse_socket_data(read_buf, read_len, on_socket_recv, bev) == 0)
		{

		}

		free(read_buf);
	}
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
		item->live = true;
		printf("Զ��RPC�������ӳɹ���\n");

		return;
	}

	//�⽫�Զ�close�׽��ֺ�free��д������  
	bufferevent_free(bev);
	item->live = false;
}

void runClient(lw_short16 port)
{

	struct event_base *base = event_base_new();
	if (NULL != base)
	{
		struct CLIENT client;

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		client.bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);	
		bufferevent_setcb(client.bev, read_cb, NULL, event_cb, (void*)&client);
		int con = bufferevent_socket_connect(client.bev, (struct sockaddr *)&addr, sizeof(addr));
		if (con >= 0)
		{
			bufferevent_enable(client.bev, EV_READ | EV_PERSIST);

			// ���ö�ʱ�� 
			event_assign(&__g_timer, base, -1, 0, time_cb, (void*)&client);
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			event_add(&__g_timer, &tv);

			event_base_dispatch(base);

			event_base_free(base);
		}
		else
		{
			bufferevent_free(client.bev);
		}

	}
}

int run_rpc_client(const lw_char8* addr, lw_short16 port)
{
	std::thread t(runClient, port);
	t.detach();

	return 0;
}

