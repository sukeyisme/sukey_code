#include "socket_thread.h"
#include "socket_base.h"
CSocketThread::CSocketThread():m_pSocket(NULL), m_pThreadPool(NULL)
{
	
}
CSocketThread::~CSocketThread()
{
}

void CSocketThread::Run()
{
	SocketThread();
}

void CSocketThread::SocketThread()
{
	while(1)
	{
		WaitForSingleObject(m_mutex,INFINITE);
		WaitForSingleObject(m_event,INFINITE);

		m_pSocket->SelectMsg();
		
		m_pThreadPool->MoveToIdleList(this);
		
		SetState(Thread_Idle);
		ReleaseMutex(m_mutex);
	}
}

void CSocketThread::SetNewSocket(CSocketBase *pSocket)
{
	m_pSocket = pSocket;
	Wakeup();
}