#pragma once
#include "thread.h"
#include "thread_pool.h"
class CSocketBase;
class CSocketThread:public CThread
{
public:
	CSocketThread();
	virtual ~CSocketThread();

	void		SocketThread();
	//�߳����еĽӿ�
	void virtual 	Run();
	//���������
	void		SetNewSocket(CSocketBase *pSocket);
	void		SetThreadPool(CThreadPool *pThreadPool) {m_pThreadPool = pThreadPool;}
	CSocketBase	*GetSocket() const {return m_pSocket;}
	void		SetIdleThList(t_IdleThList *pIdleThList) { m_pIdleThList = pIdleThList;}//����
	void		SetBusyThList(t_BusyThList *pBusyThList) {m_pBusyThList = pBusyThList;}//æµ
	
private:
	CSocketBase	*m_pSocket;
	CThreadPool	*m_pThreadPool;
	t_IdleThList	*m_pIdleThList;//����
	t_BusyThList	*m_pBusyThList;//æµ
};