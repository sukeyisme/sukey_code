
// CAppClientServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCAppClientServerApp:
// �йش����ʵ�֣������ CAppClientServer.cpp
//

class CCAppClientServerApp : public CWinAppEx
{
public:
	CCAppClientServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
protected:
	int m_nMode;
};

extern CCAppClientServerApp theApp;