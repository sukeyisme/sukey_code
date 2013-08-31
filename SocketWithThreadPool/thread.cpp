#include "Thread.h"

void *ThreadRun(void *pvThread)
{
	CThread *pThread = (CThread*)pvThread;
	pThread->Run();
	ExitThread(0);
}

CThread::CThread()
{
	m_event = CreateEvent(NULL/*��ȫ����*/,TRUE/*��λ��ʽ*/,TRUE/*��ʼ״̬*/,NULL/*��������*/);
	m_mutex = CreateMutex(NULL/*ָ��ȫ���Ե�ָ��*/,FALSE/*��ʼ����������������*/,"mutex_for_readcount"/*ָ�򻥳��������ָ��*/);
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
	
	m_hTimerThread = CreateThread(NULL/*�̰߳�ȫ����*/, NULL/*��ջ��С*/,(LPTHREAD_START_ROUTINE)ThreadRun/*�̺߳���*/,(void*)this/*�̲߳���*/,0/*�̴߳�������*/,NULL/*�߳�ID*/);
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

