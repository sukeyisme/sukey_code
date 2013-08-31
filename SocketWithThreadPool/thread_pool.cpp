#include "thread_pool.h"
#include "socket_thread.h"
#include "socket_base.h"
CThreadPool::CThreadPool(int num):m_nMaxThreadNum(MAX_THREAD_NUM),
m_nWaitSec(0), m_nWaitNec(30000000), m_nThreadNum(0)
{
	
	m_BusyMutex = CreateMutex(NULL,FALSE,"");
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	m_WaitMutex = CreateMutex(NULL,FALSE,"");
	m_WaitCond = CreateEvent(NULL,TRUE,TRUE,NULL);
	
	if (num > MAX_THREAD_NUM)
		num	= MAX_THREAD_NUM;
		
	for (int i=0; i<num; i++)
	{
		CreateNewThread();
	}
	
	m_nNormalThread		= m_nThreadNum;
	m_nMaxJobsPending	= m_nThreadNum / 2;
	m_nMaxIdleNum	 	= m_nThreadNum / 2;
	m_nMinIdleNum		= m_nThreadNum / 4;
}

CThreadPool::CThreadPool()
{
	ReleaseMutex(m_IdleMutex);
	for (t_iIdleThList iIdle=m_IdleThList.begin(); iIdle!=m_IdleThList.end(); iIdle++)
	{
		delete	*iIdle;
		*iIdle	= NULL;
	}
	ReleaseMutex(m_IdleMutex);
	
	WaitForSingleObject(m_BusyMutex,INFINITE);
	for (t_iBusyThList iBusy=m_BusyThList.begin(); iBusy!=m_BusyThList.end(); iBusy++)
	{
		delete	*iBusy;
		*iBusy	= NULL;
	}
	
	m_BusyThList.clear();
	ReleaseMutex(m_BusyMutex);
	
	CloseHandle(m_BusyMutex);
	CloseHandle(m_IdleMutex);
}

CThreadPool::~CThreadPool()
{
	m_BusyMutex = CreateMutex(NULL,FALSE,"");
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
}

bool CThreadPool::ThreadProc(CSocketBase *pSocket)
{
	ERROR_TRY
	//太多任务并行
	while (m_nMaxJobsPending <= GetBusyNum()) 
	{
		cout << "Wait" << endl;
		Wait();
	}
	CSocketThread *pThread = GetIdleThread();
	
	if (pThread) {
		MoveToBusyList(pThread);
		pThread->SetNewSocket(pSocket);
	}
	
	if (GetIdleNum() < m_nMinIdleNum) {
		//空闲线程不足！
		CreateIdleThread(m_nMinIdleNum - GetIdleNum());
		cout << "空闲线程不足！" << endl;
	}
	//空闲线程太多，需要删除
	else if ((GetIdleNum() > m_nMaxIdleNum) && (m_nThreadNum > m_nNormalThread)) {
		for (int i=0; i< (GetIdleNum()-m_nMaxIdleNum); i++)
			DeleteIdleThread();
			
		cout << "空闲线程太多" << endl;
	}
		
	ERROR_UNKNOW_RETURN(CTHREADPOOL, "ThreadProc", true)
}

bool CThreadPool::CreateNewThread()
{
	CSocketThread *pThread = new CSocketThread;
	if (pThread->Start()) 
	{
		m_IdleMutex = CreateMutex(NULL,FALSE,"");
		pThread->SetIdleThList(&m_IdleThList);
		pThread->SetBusyThList(&m_BusyThList);
		pThread->SetThreadPool(this);
		m_IdleThList.push_back(pThread);
		m_nThreadNum++;
		ReleaseMutex(m_IdleMutex);
		return true;
	}
	else 
	{
		delete pThread;
		return false;
	}
}

CSocketThread *CThreadPool::GetIdleThread()
{
	if (GetIdleNum() > 0) {
		m_IdleMutex = CreateMutex(NULL,FALSE,"");
		CSocketThread	*pThread = (CSocketThread *)m_IdleThList.front();
		ReleaseMutex(m_IdleMutex);
		return 	pThread;
	}
	
	return NULL;
}

void CThreadPool::MoveToIdleList(CSocketThread *pThread)
{
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	m_IdleThList.push_back(pThread);
	ReleaseMutex(m_IdleMutex);
	
	if (GetBusyNum() <= 0) 
		return;
		
	m_BusyMutex = CreateMutex(NULL,FALSE,"");
	m_BusyThList.remove(pThread);
	ReleaseMutex(m_BusyMutex);
}

void CThreadPool::MoveToBusyList(CSocketThread *pThread)
{
	m_BusyMutex = CreateMutex(NULL,FALSE,"");
	m_BusyThList.push_back(pThread);
	ReleaseMutex(m_BusyMutex);
	
	if (GetBusyNum() <= 0)
		return;
		
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	m_IdleThList.remove(pThread);
	ReleaseMutex(m_IdleMutex);
}

int CThreadPool::GetBusyNum() 
{
	m_BusyMutex = CreateMutex(NULL,FALSE,"");
	int num = m_BusyThList.size();
	ReleaseMutex(m_BusyMutex);
	return num;
}

int CThreadPool::GetIdleNum()
{
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	int num = m_IdleThList.size();
	ReleaseMutex(m_IdleMutex);
	return num;
}

void CThreadPool::CreateIdleThread(const int num)
{
	ERROR_TRY
	
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	if (m_nThreadNum == m_nMaxThreadNum)
		return;
	if (m_nThreadNum + num > m_nMaxThreadNum) {
		CreateIdleThread(m_nMaxThreadNum - m_nThreadNum);
	}
	ReleaseMutex(m_IdleMutex);
	
	ERROR_UNKNOW(CTHREADPOOL, "CreateIdleThread")
}

void CThreadPool::DeleteIdleThread()
{
	ERROR_TRY
	
	m_IdleMutex = CreateMutex(NULL,FALSE,"");
	m_IdleThList.pop_back();
	ReleaseMutex(m_IdleMutex);
	
	ERROR_UNKNOW(CTHREADPOOL, "DeleteIdleThread")
}

void CThreadPool::Wait()
{
	ERROR_TRY

	m_WaitMutex = CreateMutex(NULL,FALSE,"");
	HANDLE h[2] = {m_WaitCond,m_WaitMutex};
	WaitForMultipleObjects(2, h,FALSE,m_nWaitSec);
	ReleaseMutex(m_WaitMutex);
	
	ERROR_UNKNOW(CTHREADPOOL, "Wait")
}

void CThreadPool::GetWaitTime(int &nWaitSec, long &nWaitNec)
{
	nWaitSec	= m_nWaitSec;
	nWaitNec	= m_nWaitNec;
}

void CThreadPool::SetWaitTime(const int nWaitSec, const long nWaitNec)
{
	m_nWaitSec	= nWaitSec;
	m_nWaitNec	= nWaitNec;
}