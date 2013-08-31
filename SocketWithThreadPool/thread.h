#pragma once
#include "error_log.h"
#include <windows.h>

enum Thread_State
{
	Thread_Idle	= 0,	//����
	Thread_Busy	= 1,	//æµ
	Thread_Hup	= 2,	//����
	Thread_Exit	= 3	//��ֹ
};
#define CTHREAD "CThread"
class CThread:public CErrorLog
{
public:
	CThread();
	virtual ~CThread();
	
	bool    	Terminate(void);    	//��ֹ
	bool    	Start(void);        	//����
	//void    	Wait(void);		//�ȴ�
	bool    	Wakeup(void);		//����
	void		SetState(Thread_State state) {m_State = state;}
	
	HANDLE	GetThreadHandle() const {return m_hTimerThread;}

	virtual	void	Run(void) = 0;
	
	HANDLE m_mutex;		//���������
	HANDLE 	m_event;	//�¼����
	HANDLE	m_hTimerThread;//�߳̾��
	Thread_State	m_State;		//�߳�״̬
	
};
