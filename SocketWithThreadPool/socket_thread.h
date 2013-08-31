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
	//线程运行的接口
	void virtual 	Run();
	//添加新任务
	void		SetNewSocket(CSocketBase *pSocket);
	void		SetThreadPool(CThreadPool *pThreadPool) {m_pThreadPool = pThreadPool;}
	CSocketBase	*GetSocket() const {return m_pSocket;}
	void		SetIdleThList(t_IdleThList *pIdleThList) { m_pIdleThList = pIdleThList;}//空闲
	void		SetBusyThList(t_BusyThList *pBusyThList) {m_pBusyThList = pBusyThList;}//忙碌
	
private:
	CSocketBase	*m_pSocket;
	CThreadPool	*m_pThreadPool;
	t_IdleThList	*m_pIdleThList;//空闲
	t_BusyThList	*m_pBusyThList;//忙碌
};