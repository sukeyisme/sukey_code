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
//socket����
#define SOCKET_ERROR	(-1)
//ShutDown�������
#define SOCK_SEND	0
#define SOCK_RECV	1
#define SOCK_CLOSE	2

//Ĭ�Ͻ�������ֽ�
#define	MAXRECV	1024
//Ĭ����󷢳��ֽ�
#define	MAXSEND	1024

#define CSOCKETBASE "CSocketBase"
class CSocketBase:CErrorLog
{
public:
	CSocketBase();
	virtual ~CSocketBase();
	//�����׽���
	bool Create(const int domain, const int type);
	//ÿ������������Ҫ��������
	bool AddSockInList();
	bool AddAcceptInList();
	//�˿ڹ���
	bool Bind(int port);
	bool Bind();//ֱ������˿�
	bool Bind(const int port,sockaddr *addr);
	//����
	bool Listen(int backlog = 5);//Ĭ�����������г���Ϊ5
	//��·���ô���
	void		SelectMsg();
	//�׽����Ƿ���Ч
	bool Is_valid() const {return m_sock!=-1;}

	bool		IsBusy() const {return m_bBusy;}
	void		SetBusy(const bool b) {m_bBusy = b;}

	//����׽���
	bool		Attach(int &sockfd);
	int		GetSocketId() const {return m_sock;}
	sockaddr_in	*GetSocketAddr() {return &m_addr;}
	bool		Gethostname(char *hostname, int size);
	bool		GetPeerName(char *fromaddr, UNIT &port);
	void		SetConnected(sockaddr_in &addr);

	//�ر��׽���
	bool		Close();
	bool		ShutDown(const int how);

	//���������������
	void		Set_non_blocking(const bool b/*true������*/);

	//��ʼ���̳߳�
	bool InitThreadPool(int nThreadNum);
	void		SetThreadPool(CThreadPool *pThread) {m_pThreadPool = pThread;}
	CThreadPool	*GetThreadPool() const {return m_pThreadPool;}

	CSocketBase 	&operator = (const CSocketBase &sock);
	bool 		operator == (const CSocketBase &sock) const;
	virtual bool	OnReceive() = 0;
	virtual bool	OnAccept() = 0;
	virtual bool	OnConnect() = 0;

protected:
	//�����߳̿�ʼ�ػ��׽���
	void	BeginThread();
	//��ͣ�߳�
	void	StopThread();

	//�ж��׽����Ƿ��ж�������
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
	//�׽��ֱ�ʾ��
	int m_sock;
	sockaddr_in	m_addr;
	bool		m_bBusy;

	//����������
	typedef map<int, CSocketBase*>	t_mapSocket;
	typedef list<int>	t_listSocket;
	typedef t_mapSocket::iterator	t_imapSocket;
	typedef t_listSocket::iterator	t_iListSocket;

	//���洴���׽��ֵ�������
	static	t_listSocket	m_listSock;
	static	t_mapSocket	m_mapSock;

	//����Accept֮���������
	static	t_listSocket	m_listAccept;
	static	t_mapSocket	m_mapAccept;
	
	//����������������fd_set
	static	fd_set	m_rfd;

	//�̳߳�ָ��
	static 	CThreadPool *m_pThreadPool;
};