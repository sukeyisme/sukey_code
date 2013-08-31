#pragma once

#include <map>
#include <list>
#include "error_log.h"
#include "thread_pool.h"
#include "WinSock.h"

#pragma comment(lib, "Ws2_32.lib")

#define NO_OnConnect	virtual bool OnConnect() {return true;}
#define NO_OnAccept	virtual bool OnAccept() {return true;}
#define NO_OnReceive	virtual bool OnReceive() {return true;}
#define NO_OnClose	virtual bool OnClose() {return true;}
#define NO_OnSend	virtual bool OnSend() {return true;}

typedef unsigned short int    UNIT;
//socket出错
#define SOCKET_ERROR	(-1)
//ShutDown输入参数
#define SOCK_SEND	0
#define SOCK_RECV	1
#define SOCK_CLOSE	2

//默认接受最大字节
#define	MAXRECV	1024
//默认最大发出字节
#define	MAXSEND	1024

#define CSOCKETBASE "CSocketBase"
class CSocketBase:CErrorLog
{
public:
	CSocketBase();
	virtual ~CSocketBase();
	//创建套接字
	bool Create(const int domain, const int type);
	//每个新描述符都要加入链表
	bool AddSockInList();
	bool AddAcceptInList();
	//端口关联
	bool Bind(int port);
	bool Bind();//直接随机端口
	bool Bind(const int port,sockaddr *addr);
	//监听
	bool Listen(int backlog = 5);//默认最大请求队列长度为5
	//多路复用处理
	void		SelectMsg();
	//套接字是否有效
	bool Is_valid() const {return m_sock!=-1;}

	bool		IsBusy() const {return m_bBusy;}
	void		SetBusy(const bool b) {m_bBusy = b;}

	//获得套接字
	bool		Attach(int &sockfd);
	int		GetSocketId() const {return m_sock;}
	sockaddr_in	*GetSocketAddr() {return &m_addr;}
	bool		Gethostname(char *hostname, int size);
	bool		GetPeerName(char *fromaddr, UNIT &port);
	void		SetConnected(sockaddr_in &addr);

	//关闭套接字
	bool		Close();
	bool		ShutDown(const int how);

	//设置阻塞或非阻塞
	void		Set_non_blocking(const bool b/*true非阻塞*/);

	//初始化线程池
	bool InitThreadPool(int nThreadNum);
	void		SetThreadPool(CThreadPool *pThread) {m_pThreadPool = pThread;}
	CThreadPool	*GetThreadPool() const {return m_pThreadPool;}

	CSocketBase 	&operator = (const CSocketBase &sock);
	bool 		operator == (const CSocketBase &sock) const;
	virtual bool	OnReceive() = 0;
	virtual bool	OnAccept() = 0;
	virtual bool	OnConnect() = 0;

protected:
	//启动线程开始守护套接字
	void	BeginThread();
	//暂停线程
	void	StopThread();

	//判断套接字是否有东西到来
	class SocketIsSet
	{
		friend class CSocketBase;
	public:
		SocketIsSet(fd_set &rfd) : m_rfd(&rfd) {}
		~SocketIsSet(){}
		
		bool	operator ()(int &sockfd)
			{return FD_ISSET(sockfd, m_rfd);}
			
	private:
		fd_set 	*m_rfd;
		
	};
private:
	//套接字标示符
	int m_sock;
	sockaddr_in	m_addr;
	bool		m_bBusy;

	//描述符链表
	typedef map<int, CSocketBase*>	t_mapSocket;
	typedef list<int>	t_listSocket;
	typedef t_mapSocket::iterator	t_imapSocket;
	typedef t_listSocket::iterator	t_iListSocket;

	//保存创建套接字的描述符
	static	t_listSocket	m_listSock;
	static	t_mapSocket	m_mapSock;

	//保存Accept之后的描述符
	static	t_listSocket	m_listAccept;
	static	t_mapSocket	m_mapAccept;
	
	//保存所有描述符的fd_set
	static	fd_set	m_rfd;

	//线程池指针
	static 	CThreadPool *m_pThreadPool;
};