#include "tcp_socket.h"
CTcpSocket::CTcpSocket():m_bConnect(false)
{
}

CTcpSocket::~CTcpSocket()
{
}

bool CTcpSocket::InitSvrSockets(int port, int nThreadNum)
{
	if(!Create(port))
		return false;
	if(!Listen())
		return false;
	int n(1);
	setsockopt(GetSocketId(),SOL_SOCKET,SO_REUSEADDR,(char*)n,sizeof(int));

	if(!nThreadNum)
		return true;

	return InitThreadPool(nThreadNum);
}

bool CTcpSocket::Create(int port)
{
	if(CSocketBase::Create(AF_INET,SOCK_STREAM/*TCP*/))//SOCK_DGRAM是UDP
		return Bind(port);
	return false;
}

bool CTcpSocket::Create()
{
	if (CSocketBase::Create(AF_INET, SOCK_STREAM))
		return Bind();
	
	return false;
}

CTcpSocket *CTcpSocket::Accept()
{
	CTcpSocket *pSocket = new CTcpSocket;
	if (!Accept(pSocket)) 
	{
		delete pSocket;
		pSocket	= NULL;
		return 	NULL;
	}
	
	return pSocket;
}

bool CTcpSocket::Accept(CTcpSocket *pSocket)
{
	assert(pSocket);
	//对方地址
	sockaddr_in	addr;
	int addr_len	= sizeof(addr);
	int	sockfd	= accept(GetSocketId(), (sockaddr* )GetSocketAddr(), (int *)&addr_len);
	if (sockfd == SOCKET_ERROR) 
		return false;
	
	//回应Client
	char sendBuf[50];
	sprintf(sendBuf,"welcome %s to threadpool",inet_ntoa(GetSocketAddr()->sin_addr));//找对对应的IP并且将这行字打印到那里
	send(sockfd,sendBuf,strlen(sendBuf)+1,0);	//发送欢迎信息

	pSocket->Attach(sockfd);
	pSocket->SetConnected(addr);
	pSocket->AddAcceptInList();
		
	return true;
}

bool CTcpSocket::Connect(const string sAddr, int port)
{
	struct sockaddr_in 	addr;	//目的地址
	addr.sin_family 	= AF_INET; 
	addr.sin_port 		= htons(0);
	addr.sin_addr.s_addr 	= inet_addr(INADDR_ANY); 
	memset(&(addr.sin_zero),0, 8);
	
	return connect(GetSocketId(), (struct sockaddr *)&addr, sizeof(struct sockaddr))
		== 0;
}
	
int CTcpSocket::Send(const void *msg, int len)
{
	if (len <= 0)
		len	= MAXSEND;
		
	return send(GetSocketId(), (char*)msg, len, 0);
}

int CTcpSocket::Receive(void *buf, int len)
{
	if (len <= 0)
		len	= MAXRECV;
		
	return recv(GetSocketId(), (char*)buf, len, 0);
}