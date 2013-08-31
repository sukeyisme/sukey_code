#include "tcp_svr_socket.h"

CTcpSvrSocket::CTcpSvrSocket()
{
}

CTcpSvrSocket::~CTcpSvrSocket()
{
}

bool CTcpSvrSocket::OnAccept()
{
	CTcpSvrSocket *pSocket = new CTcpSvrSocket;
	if (!Accept(pSocket)) {
		delete pSocket;
		pSocket	= NULL;
		return 	false;
	}
	
	cout << "accept" << endl;
	return true;
}

bool CTcpSvrSocket::OnReceive()
{
	char	buf[512];
	int	ret = Receive(buf, 512);
	if (0 == ret) {
		Close();
		return false;
	}
	buf[ret] = '\0';
	Send(buf, strlen(buf));
	cout << buf << endl;
	
	return true;
}