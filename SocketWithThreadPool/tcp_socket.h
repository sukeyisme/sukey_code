#pragma once

#include "socket_base.h"
#include "WinSock.h"
#include "thread_pool.h"
#include <assert.h>
class CTcpSocket : public CSocketBase
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();

	//Create°üÀ¨ÁËBind
	bool		Create();
	bool		Create(int port);
	CTcpSocket	*Accept();
	bool		Accept(CTcpSocket *pSocket);
	bool		Connect(const string sAddr, int port);
	bool		InitSvrSockets(int port, int nThreadNum=0);
	
	int		Send(const void *msg, int len);
	int		Receive(void *buf, int len);
	
public:
	NO_OnConnect
	NO_OnAccept
	NO_OnReceive
	
private:
	bool		m_bConnect;
};
