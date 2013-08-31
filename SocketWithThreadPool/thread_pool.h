#pragma once
#include <list>
#include <vector>
#include "error_log.h"
#include <winsock.h>

#define MAX_THREAD_NUM	1000
#define CTHREADPOOL "CThreadPool"
class CSocketThread;
class CSocketBase;

typedef list<CSocketThread *>	t_IdleThList;
typedef t_IdleThList::iterator	t_iIdleThList;
typedef list<CSocketThread *>	t_BusyThList;
typedef t_BusyThList::iterator	t_iBusyThList;

class CThreadPool:public CErrorLog
{
public:
	CThreadPool();
	CThreadPool(int num);
	virtual ~CThreadPool();

	//创建新的线程
	bool		CreateNewThread();
	void		CreateIdleThread(const int num);
	//删除空闲线程
	void		DeleteIdleThread();
	//获取空闲线程句柄
	CSocketThread	*GetIdleThread();
	void		MoveToIdleList(CSocketThread *pThread);
	void		MoveToBusyList(CSocketThread *pThread);
	//超时等待
	void		Wait();
	
	//线程处理函数
	bool		ThreadProc(CSocketBase *pSocket);
	bool		IsInitialized() const {return m_IdleThList.size() > 0;}
	
	void		GetWaitTime(int &m_nWaitSec, long &m_nWaitNsec);
	void		SetWaitTime(const int nWaitSec, const long nWaitNsec);
	int		GetBusyNum();
	int		GetIdleNum();
	int		GetThreadNum() const {return m_nThreadNum;}
	int		GetMaxJobsPending() const {return m_nMaxJobsPending;}
	int		GetMaxNormalThread() const {return m_nNormalThread;}
	int		GetMaxIdleNum() const {return m_nMaxIdleNum;}
	int		GetMinIdleNum() const {return m_nMinIdleNum;}
	int		GetMaxThreadNum() const {return m_nMaxThreadNum;}
	void		SetMaxThreadNum(const int num) {m_nMaxThreadNum = num;}
	void		SetMaxJobsPending(const int num) {m_nMaxJobsPending = num;}
	void		SetMaxNormalThread(const int num) {m_nNormalThread = num;}
	void		SetMaxIdleNum(const int num) {m_nMaxIdleNum = num;}
	void		SetMinIdleNum(const int num) {m_nMinIdleNum = num;}

private:
	//最大允许线程数
	int		m_nMaxThreadNum;
	//实际线程总数
	int		m_nThreadNum;
	//最大并行任务数
	int		m_nMaxJobsPending;
	//最大空闲线程数
	int		m_nMaxIdleNum;
	//最小空闲线程数
	int		m_nMinIdleNum;
	//正常线程总数
	int		m_nNormalThread;
	//等待时间（秒）
	int		m_nWaitSec;
	//等待时间（纳秒）
	long		m_nWaitNec;
	//正在忙碌的线程链表
	t_BusyThList 	m_BusyThList;
	//空闲线程链表
	t_IdleThList 	m_IdleThList;
	
	HANDLE m_IdleMutex;
	HANDLE m_BusyMutex;
	HANDLE m_WaitMutex;
	HANDLE 	m_WaitCond;
	struct timeval m_delay;
	
};