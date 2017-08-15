#ifndef __session_H__
#define __session_H__

#include <string>
#include <unordered_map>

#include <event2/util.h>

#include "base_type.h"
#include "object.h"

#include "socket_hanlder.h"

class SocketSession;
class SocketProcessor;

typedef std::function<bool(char* buf, lw_int32 bufsize)> SocketCallback;

enum SESSION_TYPE
{
	NONE = 0,
	Client = 1,
	Server = 2,
};

class SocketSession : public Object
{
	friend class CoreSocket;

public:
	SocketSession(ISocketSessionHanlder* handler);
	virtual ~SocketSession();

public:
	evutil_socket_t getSocket();

public:
	int create(SESSION_TYPE c, SocketProcessor* processor, evutil_socket_t fd, short ev);
	void destroy();

public:
	void setConnTimeout(int s);
	void setRecvTimeout(int s);
	void setSendTimeout(int s);

public:
	void setHost(const std::string& host);
	std::string getHost();

	void setPort(int port);
	int getPort();

public:
	bool connected();

public:
	virtual std::string debug() override;

public:
	lw_int32 sendData(lw_int32 cmd, void* object, lw_int32 objectSize);
	lw_int32 sendData(lw_int32 cmd, void* object, lw_int32 objectSize, lw_int32 recvcmd, SocketCallback cb);

private:
	void onRead();
	void onWrite();
	void onEvent(short ev);
	void onParse(lw_int32 cmd, char* buf, lw_int32 bufsize);

private:
	void reset();

private:
	std::unordered_map<lw_int32, SocketCallback> _cmd_event_map;

private:
	SESSION_TYPE _c;	//session����

private:
	std::string _host;
	int _port;
	bool _connected;

private:
	struct bufferevent* _bev;
	ISocketSessionHanlder * _handler;
};


#endif // !__session_H__