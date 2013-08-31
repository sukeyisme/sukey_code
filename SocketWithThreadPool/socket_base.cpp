#include "socket_base.h"
#include <algorithm>
#include <fcntl.h>
CSocketBase::t_mapSocket CSocketBase::m_mapSock;
CSocketBase::t_mapSocket CSocketBase::m_mapAccept;
CSocketBase::t_listSocket CSocketBase::m_listAccept;
CSocketBase::t_listSocket CSocketBase::m_listSock;
fd_set CSocketBase::m_rfd;
CThreadPool *CSocketBase::m_pThreadPool = NULL;
CSocketBase::CSocketBase():m_sock(-1), m_bBusy(false)
{
	
}

CSocketBase::~CSocketBase()
{
	Close();
}

bool CSocketBase::Create(const int domain, const int type)
{
	m_sock = socket(domain,type,0);
	
	AddSockInList();
	
	return (m_sock != INVALID_SOCKET);
}

bool CSocketBase::AddSockInList()
{
	if(m_sock <=0)
		return false;
	m_listSock.push_back(m_sock);
	m_mapSock[m_sock] = this;
	FD_SET(m_sock,&m_rfd);

	return TRUE;
}

bool CSocketBase::AddAcceptInList()
{
	if (m_sock <= 0)
		return false;
		
	m_listAccept.push_back(m_sock);
	m_mapAccept[m_sock] = this;
	FD_SET(m_sock,&m_rfd);
	
	return true;
}

bool CSocketBase::Attach(int &sockfd)
{
	ERROR_TRY
	
	if (sockfd <=0 )
		return false;
		
	m_sock	= sockfd;
	
	ERROR_UNKNOW_RETURN(CSOCKETBASE, "Attach", true)
}

bool CSocketBase::Gethostname(char *hostname, int size)
{
	return gethostname(hostname, size) != SOCKET_ERROR;
}

bool CSocketBase::GetPeerName(char *fromaddr, UNIT &port)
{
	ERROR_TRY
	
	if (m_sock <= 0)
		return false;

	struct 	sockaddr_in addr;
	int len	= sizeof(struct	sockaddr_in);
	if (getpeername(m_sock, (sockaddr *)&addr, &len) 
		== SOCKET_ERROR) 
		return false;
		
	fromaddr = inet_ntoa(addr.sin_addr);
	port	 = ntohs(addr.sin_port);
	
	ERROR_UNKNOW_RETURN(CSOCKETBASE, "GetPeerName", true)
}

void CSocketBase::SetConnected(sockaddr_in &addr)
{
	m_addr	= addr;
}

void CSocketBase::Set_non_blocking(const bool b)
{
	ERROR_TRY
	int ret(0); 
	u_long iMode;
	if(b)
	{
		iMode = 1;
		ret = ioctlsocket(m_sock, FIONBIO,&iMode);
	}
	else
	{
		iMode = 0;
		ret = ioctlsocket(m_sock, FIONBIO,&iMode);
	}

	if (ret==SOCKET_ERROR)
	{
		cout << "ioctlsocket failed" << endl;
		return;
	}
	
	
	ERROR_UNKNOW(CSOCKETBASE, "Set_non_blocking")
}

bool CSocketBase::Close()
{
	ERROR_TRY
	
	if (Is_valid()) {
		FD_CLR(m_sock, &m_rfd);
		m_listSock.remove(m_sock);
		m_listAccept.remove(m_sock);
		return closesocket(m_sock) != SOCKET_ERROR;
	}
		
	ERROR_UNKNOW_RETURN(CSOCKETBASE, "Close", false)
}

bool CSocketBase::ShutDown(const int how)
{
	if (Is_valid())
		return shutdown(m_sock, how) != SOCKET_ERROR;
		
	return	false;
}

bool CSocketBase::Bind(const int port, struct sockaddr *addr)
{
	if (m_sock < 0) 
		return false;
		
	if (port < 1024)
		return false;
		
	int ret	= bind(m_sock, addr, sizeof(struct sockaddr));
	
	return ret != SOCKET_ERROR;
}

bool CSocketBase::Bind(int port)
{
	//套接口使用的通讯协议
	m_addr.sin_family = AF_INET;
	//如果port=0,随即选择一个没有用使用过的端口
	m_addr.sin_port = htons(port);
	//本机所有IP
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(m_addr.sin_zero),0,8);
	return bind(m_sock,(sockaddr*)&m_addr,sizeof(sockaddr))!=SOCKET_ERROR;
}

bool CSocketBase::Bind()
{
	return Bind(0);
}
bool CSocketBase::Listen(int backlog)
{
	if(!Is_valid())
		return false;
	if(backlog <=0)
		backlog = 5;
	int ret = listen(m_sock,backlog);
	return ret != SOCKET_ERROR;
}

bool CSocketBase::InitThreadPool(int nThreadNum)
{
	ERROR_TRY
	if(m_pThreadPool)
		return false;
	m_pThreadPool = new CThreadPool(nThreadNum);

	if(m_pThreadPool)
		BeginThread();
	else
		return false;

	ERROR_UNKNOW_RETURN(CSOCKETBASE, "InitThreadPool", true)
	return false;
}

bool CSocketBase::operator == (const CSocketBase &sock) const
{
	return m_sock == sock.GetSocketId();
}

void CSocketBase::BeginThread()
{
	m_pThreadPool->ThreadProc(this);
}

void CSocketBase::StopThread()
{
	//暂时没有，任务只要一进入线程池就无法退出，只能等到任务结束
}

void CSocketBase::SelectMsg()
{
	fd_set	Readrfd;
	FD_ZERO(&Readrfd);
	Readrfd	= m_rfd;
	t_iListSocket	ilist;
	t_imapSocket	imap;
	CSocketBase	*pSocket;
	struct timeval 	timeOut;
	timeOut.tv_sec 	= 1;
    	timeOut.tv_usec = 0;
	int	ret	= 0;
  	
	ret	= select(0, &Readrfd, NULL, NULL, &timeOut);
	//启动另一个线程
	BeginThread();
	if (ret <= 0)
		return;
	
	ilist = find_if(m_listSock.begin(), m_listSock.end(), SocketIsSet(Readrfd));
	//TCP服务端的ACCEPT
	if (ilist != m_listSock.end()) {
		imap	= m_mapSock.find(*ilist);
		pSocket	= imap->second;
		if (pSocket->IsBusy())
			return;
		pSocket->SetBusy(true);
		pSocket->OnAccept();
		pSocket->SetBusy(false);
		
		return;
	}
	
	ilist = find_if(m_listAccept.begin(), m_listAccept.end(), SocketIsSet(Readrfd));
	//接收
	if (ilist != m_listAccept.end()) {
		imap	= m_mapAccept.find(*ilist);
		pSocket	= imap->second;
		if (pSocket->IsBusy())
			return;
		pSocket->SetBusy(true);
		pSocket->OnReceive();
		pSocket->SetBusy(false);
	}
}