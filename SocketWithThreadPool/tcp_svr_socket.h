#pragma once
#include "tcp_socket.h"
class CTcpSvrSocket :public CTcpSocket
{
public:
	CTcpSvrSocket();
	virtual ~CTcpSvrSocket(); 

	virtual bool	OnReceive();
	virtual bool	OnAccept();
	NO_OnConnect	
};