/*
Copyright 2016 Garrett D'Amore <garrett@damore.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

"nanomsg" is a trademark of Martin Sustrik
*/

/*  This program serves as an example for how to write a simple RPC service,
using the request/reply pattern.  The server is just a single threaded
for loop, which processes each request.  The requests run quickly enough
that there is no need for parallelism.

Our demonstration application layer protocol is simple.  The client sends
a name, and the server replies with a greeting based on the time of day.
The messages are sent in ASCII, and are not zero terminated.

To run this program, start the server as rpc_demo <url> -s
Then connect to it with the client as rpc_client <url> <name>.
The client will print a timezone appropriate greeting, based on
the time at the server.  For example:

% ./rpc_demo tcp://127.0.0.1:5555 -s &
% ./rpc_demo tcp://127.0.0.1:5555 Garrett
Good morning, Garrett.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nn.h>
#include <reqrep.h>

#pragma warning(disable:4996)

#include "base_type.h"
#include "common_marco.h"

#include "business.h"

#include "NetHead.h"
#include "NetMessage.h"
#include "ProtocolData.h"

#include "Message.h"
#include "platform.pb.h"

using namespace LW;

#ifdef _WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000, 1);
#else
#define SLEEP(seconds) sleep(seconds);
#endif

static lw_int32 send_socket_data(lw_int32 sock, lw_int32 cmd, void* object, lw_int32 objectSize);
static lw_int32 recv_socket_data(lw_int32 sock);
static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata);

static void on_socket_recv(lw_int32 cmd, char* buf, lw_int32 bufsize, void* userdata)
{
	switch (cmd)
    {
        case 10000:
        {
            sc_userinfo *user = (sc_userinfo*)(buf);
            printf(" age: %d\n sex: %d\n name: %s\n address: %s\n",
                   user->age, user->sex, user->name, user->address);
        } break;
            //        case 10001:	//
            //        {
            //            platform::sc_msg_userinfo msg;
            //            msg.ParseFromArray(buf, bufsize);
            //            printf(" age: %d\n sex: %d\n name: %s\n address: %s\n",
            //                   msg.age(), msg.sex(), msg.name().c_str(), msg.address().c_str());
            //        }break;
        case 10002:
        {
            
        }break;
        default:
            break;
    }
}

static lw_int32 send_socket_data(lw_int32 sock, lw_int32 cmd, void* object, lw_int32 objectSize)
{
    LW_NET_MESSAGE* p = lw_create_net_message(cmd, object, objectSize);
    
    lw_int32 result = nn_send(sock, p->buf, p->size, 0);
    
    lw_free_net_message(p);
    
    return result;
}

static lw_int32 recv_socket_data(lw_int32 sock)
{
    char *buf = NULL;
    lw_int32 result = nn_recv(sock, &buf, NN_MSG, 0);
    if (result > 0)
    {
        lw_on_parse_socket_data(buf, result, on_socket_recv, NULL);
        
        nn_freemsg(buf);
    }
    else
    {
        
    }
    
    return result;
}

/*  The server runs forever. */
int rep_server(const char *url)
{
	int fd;

	/*  Create the socket. */
	fd = nn_socket(AF_SP, NN_REP);
	if (fd < 0) {
		fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
		return (-1);
	}

	/*  Bind to the URL.  This will bind to the address and listen
	synchronously; new clients will be accepted asynchronously
	without further action from the calling program. */

	if (nn_bind(fd, url) < 0) {
		fprintf(stderr, "nn_bind: %s\n", nn_strerror(nn_errno()));
		nn_close(fd);
		return (-1);
	}

	printf("rep server start ...\n");

	/*  Now we can just process results.  Note that there is no explicit
	accept required.  We just receive a request, and reply to it.
	Its important to note that we must not issue two receives in a
	row without replying first, or the following receive(s) will
	cancel any unreplied requests. */

	for (;;) {
		char username[128];
		char greeting[128];
		time_t secs;
		struct tm *now;
		char *daytime;
		int rc;
		char *fmt;

		rc = nn_recv(fd, username, sizeof(username), 0);
		if (rc < 0) {
			/*  Any error here is unexpected. */
			fprintf(stderr, "nn_recv: %s\n", nn_strerror(nn_errno()));
			break;
		}

		secs = time(NULL);
		now = localtime(&secs);
		if (now->tm_hour < 12) {
			daytime = "morning";
		}
		else if (now->tm_hour < 17) {
			daytime = "afternoon";
		}
		else if (now->tm_hour < 20) {
			daytime = "evening";
		}
		else {
			daytime = "night";
		}

		/*  Ensure ASCIIZ terminated string. */
		if (rc < sizeof(username)) {
			username[rc] = '\0';
		}
		else {
			username[sizeof(username) - 1] = '\0';
		}

		fmt = "Good %s, %s.";

		/*  Technically this might be overly pessimistic about size. */
		if ((strlen(username) + strlen(daytime) + strlen(fmt)) >=
			sizeof(greeting)) {
			fmt = "I'm sorry, your name is too long.  But good %s anyway.";
		}

		/*  snprintf would be safer, but the above check protects us. */
		sprintf(greeting, fmt, daytime, username);

		rc = nn_send(fd, greeting, strlen(greeting), 0);
		if (rc < 0) {
			/*  There are several legitimate reasons this can fail.
			We note them for debugging purposes, but then ignore
			otherwise.  If the socket is closed or failing, we will
			notice in recv above, and exit then. */
			fprintf(stderr, "nn_send: %s (ignoring)\n",
				nn_strerror(nn_errno()));
		}
	}

	nn_close(fd);
	return (-1);
}

/*  The client runs just once, and then returns. */
int req_client(const char *url, const char *username)
{
	int fd;
	int rc;
	char *greeting;
	char *msg;

	fd = nn_socket(AF_SP, NN_REQ);
	if (fd < 0) {
		fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
		return (-1);
	}

	if (nn_connect(fd, url) < 0) {
		fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
		nn_close(fd);
		return (-1);
	}

	if (nn_send(fd, username, strlen(username), 0) < 0) {
		fprintf(stderr, "nn_send: %s\n", nn_strerror(nn_errno()));
		nn_close(fd);
		return (-1);
	}

	/*  Here we ask the library to allocate response buffer for us (NN_MSG). */
	rc = nn_recv(fd, &msg, NN_MSG, 0);
	if (rc < 0) {
		fprintf(stderr, "nn_recv: %s\n", nn_strerror(nn_errno()));
		nn_close(fd);
		return (-1);
	}

	nn_close(fd);

	/*  Response is not ASCIIZ terminated. */
	greeting = (char*)malloc(rc + 1);
	if (greeting == NULL) {
		fprintf(stderr, "malloc: %s\n", strerror(errno));
		return (-1);
	}
	memcpy(greeting, msg, rc);

	nn_freemsg(msg);
	printf("%s\n", greeting);
	return (0);
}

int __main(int argc, char **argv)
{
	int rc;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <url> [-s|name]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[2], "-s") == 0) {
		rc = rep_server(argv[1]);
	}
	else {
		rc = req_client(argv[1], argv[2]);
	}
	exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

