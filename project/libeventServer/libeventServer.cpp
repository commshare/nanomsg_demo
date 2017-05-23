// libevent-demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <winsock2.h>
#include <stdio.h>
#include <iostream>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/util.h>

static void do_read(evutil_socket_t fd, short events, void *arg);
static void do_accept(evutil_socket_t listener, short event, void *arg);

//struct bufferevent�ڽ�������event(read/write)�Ͷ�Ӧ�Ļ�����(struct evbuffer *input, *output)�����ṩ��Ӧ�ĺ�����������>
//������(����ֱ�Ӳ���bufferevent)
//���յ����ݺ��ж��ǲ�һ��һ����Ϣ�Ľ�����������־Ϊ"over"�ַ���
void readcb(struct bufferevent *bev, void *ctx)
{
	printf("called readcb!\n");
	struct evbuffer *input, *output;
	char *request_line;
	size_t len;
	input = bufferevent_get_input(bev);//��ʵ����ȡ��bufferevent�е�input
	output = bufferevent_get_output(bev);//��ʵ����ȡ��bufferevent�е�output

	size_t input_len = evbuffer_get_length(input);
	printf("input_len: %d\n", input_len);
	size_t output_len = evbuffer_get_length(output);
	printf("output_len: %d\n", output_len);

	evutil_socket_t fd = bufferevent_getfd(bev);

	while (1)
	{
		int ret = evbuffer_read(input, fd, 1024);
		request_line = evbuffer_readln(input, &len, /*EVBUFFER_EOL_CRLF*/EVBUFFER_EOL_ANY);//��evbufferǰ��ȡ��һ�У���һ���·���Ŀ��ַ�����
		//���ַ���������һ��, EVBUFFER_EOL_CRLF��ʾ��β��һ����ѡ�Ļس�������һ�����з�
		if (NULL == request_line)
		{
			printf("The first line has not arrived yet.\n");
			::free(request_line);//֮����Ҫ����free����Ϊ line = mm_malloc(n_to_copy+1))�������������malloc
			break;
		}
		else
		{
			printf("Get one line date: %s\n", request_line);
			if (strstr(request_line, "over") != NULL)//�����ж��ǲ���һ����Ϣ�Ľ���
			{
				char *response = "Response ok! Hello Client!\r\n";
				evbuffer_add(output, response, strlen(response));//Adds data to an event buffer
				printf("����˽���һ��������ɣ��ظ��ͻ���һ����Ϣ: %s\n", response);
				::free(request_line);
				break;
			}
		}
		::free(request_line);
	}

	size_t input_len1 = evbuffer_get_length(input);
	printf("input_len1: %d\n", input_len1);
	size_t output_len1 = evbuffer_get_length(output);
	printf("output_len1: %d\n\n", output_len1);
}

void errorcb(struct bufferevent *bev, short error, void *ctx)
{
	if (error & BEV_EVENT_EOF)
	{
		/* connection has been closed, do any clean up here */
		printf("connection closed\n");
	}
	else if (error & BEV_EVENT_ERROR)
	{
		/* check errno to see what error occurred */
		printf("some other error\n");
	}
	else if (error & BEV_EVENT_TIMEOUT)
	{
		/* must be a timeout event handle, handle it */
		printf("Timed out\n");
	}
	bufferevent_free(bev);
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	struct sockaddr_storage ss;
	//struct sockaddr_in sin;
	int slen = sizeof(ss);
	int fd = ::accept(listener, (struct sockaddr*)&ss, &slen);
	if (fd < 0)
	{
		perror("accept");
	}
	else if (fd > FD_SETSIZE)
	{
		evutil_closesocket(fd);
	}
	else
	{
		struct bufferevent *bev;
		int ret = evutil_make_socket_nonblocking(fd);

		//ʹ��bufferevent_socket_new����һ��struct bufferevent *bev��������sockfd���йܸ�event_base
		////BEV_OPT_CLOSE_ON_FREE��ʾ�ͷ�buffereventʱ�رյײ㴫��˿ڡ��⽫�رյײ��׽��֣��ͷŵײ�bufferevent�ȡ�
		bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

		//���ö�д��Ӧ�Ļص�����
		bufferevent_setcb(bev, readcb, nullptr, errorcb, nullptr);
		//      bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);

		//���ö�д�¼�,��ʵ�ǵ�����event_add����Ӧ��д�¼������¼���������poll�������ĵ���˵�������Ӧ�¼�����Ϊtrue��buf
		//ferevent�ǲ����д���ݵ�
		bufferevent_enable(bev, EV_READ | EV_WRITE);
	}
}

static void run_server(u_short port = 9876)
{
	struct event_base *base;

	base = event_base_new();
	if (!base)
	{
		return;
	}

	evutil_socket_t listener;
 	listener = ::socket(AF_INET, SOCK_STREAM, 0);

	//�����ΰ�ͬһ����ַ��Ҫ����socket��bind֮��  
	evutil_make_listen_socket_reuseable(listener);

#ifndef WIN32
	{
		int one = 1;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	}
#endif

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);
	if (::bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		perror("bind");
		return;
	}

	if (::listen(listener, 16) < 0)
	{
		perror("listen");
		return;
	}

	//��ƽ̨ͳһ�ӿڣ����׽�������Ϊ������״̬
	evutil_make_socket_nonblocking(listener);

	struct event *listener_event;
	listener_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);

	event_add(listener_event, nullptr);

	event_base_dispatch(base);

	event_free(listener_event);
}

#if FALSE

int main(int argc, char** argv)
{
#if defined(WIN32) || defined(_WIN32)
	WSADATA WSAData;
	int ret;
	if (ret = WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		std::cout << "Can not initilize winsock.dll" << std::endl;
		std::cout << "Error Code:" << WSAGetLastError() << std::endl;
		return 1;
	}
	else
	{
		std::cout << "winsock.dll load" << std::endl;
	}
#endif

	//Server server;
	//server.run();

	run_server();

	WSACleanup();

	return 0;
}

#endif // TEST
