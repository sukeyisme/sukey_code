#include "Thread.h"

void *ThreadRun(void *pvThread)
{
	CThread *pThread = (CThread*)pvThread;
	pThread->Run();
	ExitThread(0);
}

CThread::CThread()
{
	m_event = CreateEvent(NULL/*安全属性*/,TRUE/*复位方式*/,TRUE/*初始状态*/,NULL/*对象名称*/);
	m_mutex = CreateMutex(NULL/*指向安全属性的指针*/,FALSE/*初始化互斥对象的所有者*/,"mutex_for_readcount"/*指向互斥对象名的指针*/);
}

CThread::~CThread()
{
	Terminate();
	CloseHandle(m_event);
	CloseHandle(m_mutex);
	m_event = NULL;
	m_mutex = NULL;
}

bool CThread::Start()
{
	ERROR_TRY
	
	m_hTimerThread = CreateThread(NULL/*线程安全属性*/, NULL/*堆栈大小*/,(LPTHREAD_START_ROUTINE)ThreadRun/*线程函数*/,(void*)this/*线程参数*/,0/*线程创建属性*/,NULL/*线程ID*/);
	if (!m_hTimerThread) {
		cout<< "create the thread error !!!" << endl;
		return false;
	}
	m_State	= Thread_Hup;
	
	ERROR_UNKNOW_RETURN(CTHREAD, "Start", true)
}

bool CThread::Terminate()
{
	ERROR_TRY
	
	if (TerminateThread(m_hTimerThread,0))
	{		
		CloseHandle(m_hTimerThread);
		m_hTimerThread = NULL;
		m_State	= Thread_Exit;
		return true;
	}
	
	ERROR_UNKNOW_RETURN(CTHREAD, "Terminate", false)
}

bool CThread::Wakeup()
{
	ERROR_TRY

	if (SetEvent(m_event)) 
	{
		m_State	= Thread_Idle;
		return true;
	}
	
	ERROR_UNKNOW_RETURN(CTHREAD, "Wakeup", false)
}

