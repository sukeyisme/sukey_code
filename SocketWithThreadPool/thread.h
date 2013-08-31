#pragma once
#include "error_log.h"
#include <windows.h>

enum Thread_State
{
	Thread_Idle	= 0,	//¿ÕÏĞ
	Thread_Busy	= 1,	//Ã¦Âµ
	Thread_Hup	= 2,	//¹ÒÆğ
	Thread_Exit	= 3	//ÖÕÖ¹
};
#define CTHREAD "CThread"
class CThread:public CErrorLog
{
public:
	CThread();
	virtual ~CThread();
	
	bool    	Terminate(void);    	//ÖÕÖ¹
	bool    	Start(void);        	//Æô¶¯
	//void    	Wait(void);		//µÈ´ı
	bool    	Wakeup(void);		//ËÕĞÑ
	void		SetState(Thread_State state) {m_State = state;}
	
	HANDLE	GetThreadHandle() const {return m_hTimerThread;}

	virtual	void	Run(void) = 0;
	
	HANDLE m_mutex;		//»¥³âËø¾ä±ú
	HANDLE 	m_event;	//ÊÂ¼ş¾ä±ú
	HANDLE	m_hTimerThread;//Ïß³Ì¾ä±ú
	Thread_State	m_State;		//Ïß³Ì×´Ì¬
	
};
