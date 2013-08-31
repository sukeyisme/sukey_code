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

	//�����µ��߳�
	bool		CreateNewThread();
	void		CreateIdleThread(const int num);
	//ɾ�������߳�
	void		DeleteIdleThread();
	//��ȡ�����߳̾��
	CSocketThread	*GetIdleThread();
	void		MoveToIdleList(CSocketThread *pThread);
	void		MoveToBusyList(CSocketThread *pThread);
	//��ʱ�ȴ�
	void		Wait();
	
	//�̴߳�����
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
	//��������߳���
	int		m_nMaxThreadNum;
	//ʵ���߳�����
	int		m_nThreadNum;
	//�����������
	int		m_nMaxJobsPending;
	//�������߳���
	int		m_nMaxIdleNum;
	//��С�����߳���
	int		m_nMinIdleNum;
	//�����߳�����
	int		m_nNormalThread;
	//�ȴ�ʱ�䣨�룩
	int		m_nWaitSec;
	//�ȴ�ʱ�䣨���룩
	long		m_nWaitNec;
	//����æµ���߳�����
	t_BusyThList 	m_BusyThList;
	//�����߳�����
	t_IdleThList 	m_IdleThList;
	
	HANDLE m_IdleMutex;
	HANDLE m_BusyMutex;
	HANDLE m_WaitMutex;
	HANDLE 	m_WaitCond;
	struct timeval m_delay;
	
};