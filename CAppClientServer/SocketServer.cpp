// SocketServer.cpp : 实现文件
//

#include "stdafx.h"
#include "CAppClientServer.h"
#include "SocketServer.h"


// CSocketServer 对话框

IMPLEMENT_DYNAMIC(CSocketServer, CDialog)

CSocketServer::CSocketServer(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketServer::IDD, pParent)
	, m_port(_T("2000"))
	, m_numthread(_T("10"))
{

}

CSocketServer::~CSocketServer()
{
}

void CSocketServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_NUM, m_numthread);
}


BEGIN_MESSAGE_MAP(CSocketServer, CDialog)
	ON_BN_CLICKED(IDC_RUN, &CSocketServer::OnBnClickedRun)
END_MESSAGE_MAP()


// CSocketServer 消息处理程序

void CSocketServer::OnBnClickedRun()
{
	//CTcpSvrSocket tcpSocket;
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	/*if(tcpSocket.InitSvrSockets(9100,500))
	{
		while(1);
	}
	else
		cout<<"error"<<endl;*/
	WSACleanup();
}
