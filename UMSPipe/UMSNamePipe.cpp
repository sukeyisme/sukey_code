#include "StdAfx.h"
#include "UMSNamePipe.h"

UMSNamePipeServer g_UMSNamePipe;
UMSNamePipeClient g_AddinNamePipe;

UMSNamePipeServer::UMSNamePipeServer(void)
:m_hWnd(NULL)
{
	
}

UMSNamePipeServer::~UMSNamePipeServer(void)
{
}


BOOL UMSNamePipeServer::OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize)
{
	if (m_hWnd) 
		::SendMessage(m_hWnd,UMS_NAME_PIPE_MESSAGE,(WPARAM)pRecvItem,0);

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
UMSNamePipeClient::UMSNamePipeClient(void)
{

}

UMSNamePipeClient::~UMSNamePipeClient(void)
{
	
}

