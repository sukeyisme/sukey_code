#include "StdAfx.h"
#include "UMyNamePipe.h"
#include <assert.h>
#include <List>

#define UMY_NAMEPIPE_NAME	_T("_YiYi_Song_")

namespace Zealink { namespace Common {

UMyNamePipe::UMyNamePipe(void)
:m_hPipe(INVALID_HANDLE_VALUE)
{
	memset(m_szPipeName,0,sizeof(m_szPipeName));
	_tcscpy_s(m_szPipeName,UMY_NAMEPIPE_NAME);
}

UMyNamePipe::~UMyNamePipe(void)
{
}

BOOL UMyNamePipe::Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize, 
						 DWORD dwInBufferSize, DWORD dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	if (IsOpen()) return FALSE;	//已管道已创建了
	if (lpszName==NULL || !*lpszName) return FALSE;	//管道名称不能为空

	TCHAR szPipeName[MAX_PATH]={0};
	wsprintf(szPipeName,_T("\\\\.\\PIPE\\%s"),lpszName);

	m_hPipe=::CreateNamedPipe(szPipeName,dwOpenMode,dwPipeMode,dwMaxInstances,dwOutBufferSize,dwInBufferSize,dwDefaultTimeOut,lpSecurityAttributes);

	return m_hPipe!=INVALID_HANDLE_VALUE;
}

BOOL UMyNamePipe::Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess, 
					   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwFlagsAndAttributes)
{

	TCHAR szPipeName[MAX_PATH]={0};
	wsprintf(szPipeName,_T("\\\\%s\\PIPE\\%s"),lpszServerName,lpszPipeName);

	m_hPipe=::CreateFile(szPipeName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, OPEN_EXISTING, dwFlagsAndAttributes, NULL);

	return m_hPipe!=INVALID_HANDLE_VALUE;
}

BOOL UMyNamePipe::Close()
{
	if (!IsOpen()) return TRUE;

	BOOL bSuccess=::CloseHandle(m_hPipe);

	m_hPipe=INVALID_HANDLE_VALUE;

	return bSuccess;
}

BOOL UMyNamePipe::GetUMyNamedPipeInfo(DWORD& dwFlags) const
{
	dwFlags=0;
	if (!IsOpen()) return FALSE;

	BOOL bSuccess = ::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL);

	return bSuccess;
}

BOOL UMyNamePipe::IsServerPipe(BOOL& bServerPipe) const
{
	DWORD dwFlags=0;
	if (!GetUMyNamedPipeInfo(dwFlags)) return FALSE;

	bServerPipe=(dwFlags & PIPE_SERVER_END)!=0;
	return TRUE;
}

BOOL UMyNamePipe::IsClientPipe(BOOL& bClientPipe) const
{
	DWORD dwFlags=0;
	if (!GetUMyNamedPipeInfo(dwFlags)) return FALSE;

	bClientPipe=(dwFlags & PIPE_CLIENT_END)!=0;
	return TRUE;
}

BOOL UMyNamePipe::ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut)
{
	TCHAR szPipeName[MAX_PATH]={0};
	wsprintf(szPipeName,_T("\\\\%s\\PIPE\\%s"),lpszServerName,lpszPipeName);

	BOOL bSuccess=::WaitNamedPipe(szPipeName,dwTimeOut);

	return bSuccess;
}

BOOL UMyNamePipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	if (!IsOpen()) return FALSE;
	BOOL bSuccess = ::WriteFile(m_hPipe, lpBuffer, dwNumberOfBytesToWrite,&dwNumberOfBytesWritten, lpOverlapped);
	return bSuccess;
}

BOOL UMyNamePipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!IsOpen()) return FALSE;

	BOOL bSuccess = ::WriteFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine);
	return bSuccess;
}

BOOL UMyNamePipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if (!IsOpen()) return FALSE;
	BOOL bSuccess = ::ReadFile(m_hPipe, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, lpOverlapped);
	return bSuccess;
}

BOOL UMyNamePipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!IsOpen()) return FALSE;

	BOOL bSuccess = ::ReadFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToRead,lpOverlapped, lpCompletionRoutine);
	return bSuccess;
}

BOOL UMyNamePipe::ConnectClient(OVERLAPPED& overlapped)
{
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;

	if (!IsOpen()) return FALSE;//Pipe must be open
	BOOL bServerPipe=FALSE;
	if (!IsServerPipe(bServerPipe) || !bServerPipe) return FALSE;

	BOOL bResult = ::ConnectNamedPipe(m_hPipe, &overlapped);
	if (bResult)
	{
		return FALSE;
	}

	switch (GetLastError()) 
	{ 
		// The overlapped connection in progress. 
		case ERROR_IO_PENDING: 
		return TRUE;

		// Client is already connected, so signal an event. 
		case ERROR_PIPE_CONNECTED: 
		if (SetEvent(overlapped.hEvent)) 
			break; 

		// If an error occurs during the connect operation... 
		default: 
		{
			//printf("ConnectNamedPipe failed with %d.\n", GetLastError());
			return FALSE;
		}
	}

	return bResult;
}

BOOL UMyNamePipe::DisconnectClient()
{
	if (!IsOpen()) return FALSE;//Pipe must be open
	BOOL bServerPipe=FALSE;
	if (!IsServerPipe(bServerPipe) || !bServerPipe) return FALSE;

	BOOL bSuccess = ::DisconnectNamedPipe(m_hPipe);
	return bSuccess;
}

BOOL UMyNamePipe::GetClientUserName(LPTSTR lpName,long lLen) const
{
	if (!IsOpen()) return FALSE;//Pipe must be open
	if (!lpName || lLen<=0) return FALSE;
	
	BOOL bSuccess = ::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, lpName, lLen);

	return bSuccess;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UMyNamePipeThread::Impl
{
	typedef std::list<PPIPE_HEADER> SEND_DATA;

	SEND_DATA	_arySendData;

	void Clear()
	{
		SEND_DATA::iterator iter=_arySendData.begin();
		for(;iter!=_arySendData.end();++iter)
		{
			if (*iter)
			{
				delete[] *iter;
				*iter=NULL;
			}
			
		}

		_arySendData.clear();
	}

	PPIPE_HEADER PopSendData()
	{
		if (_arySendData.empty()) return NULL;

		PPIPE_HEADER pItem=(*_arySendData.begin());
		_arySendData.pop_front();

		return pItem;
	}

	BOOL PushSendData(PPIPE_HEADER pItem)
	{
		if (pItem==NULL || pItem->_lPackSize<=0 || pItem->_lCommandID<0) return FALSE;

		char* pData=new char[pItem->_lPackSize+5];
		PPIPE_HEADER pNewItem=(PPIPE_HEADER)pData;
		memset(pData,0,sizeof(pItem->_lPackSize+5));
		memcpy(pData,pItem,pItem->_lPackSize);

		_arySendData.push_back(pNewItem);

		return TRUE;
	}
};

UMyNamePipeThread::UMyNamePipeThread()
:m_hThread(NULL),m_dwThreadId(0),m_pImpl(new Impl)
{
	m_hExitEvent=::CreateEvent(NULL,TRUE,TRUE,NULL);
	::ResetEvent(m_hExitEvent);

	m_hPipeEvent=::CreateEvent(NULL,TRUE,TRUE,NULL);
	::ResetEvent(m_hPipeEvent);

	memset(m_buffer,0,sizeof(m_buffer));
}


UMyNamePipeThread::~UMyNamePipeThread()
{
	Stop();

	if (m_hExitEvent)
	{
		CloseHandle(m_hExitEvent);
		m_hExitEvent=NULL;
	}

	if (m_hPipeEvent)
	{
		CloseHandle(m_hPipeEvent);
		m_hPipeEvent=NULL;
	}

	if (m_pImpl)
	{
		delete m_pImpl;
		m_pImpl=NULL;
	}
}

BOOL UMyNamePipeThread::Initialize(size_t stackSize)
{
    if (m_hThread)
	{
		//JS_ERROR(_T("m_hThread is already initialized"));
		return FALSE;
	}

	m_hThread = ::CreateThread(NULL, stackSize, UMyNamePipeThread::ThreadFunction, reinterpret_cast<void*>(this), 0, &m_dwThreadId);
	
	if (m_hThread==NULL)
	{
		//JS_ERROR(_T("Error: CreateThread() failed. Error=%d"),::GetLastError());
		return FALSE;
	}

	return TRUE;
}

void UMyNamePipeThread::Stop()
{
	::SetEvent(m_hExitEvent);
	
	Close();

	if (m_hThread) 
	{ 
		::WaitForSingleObject(m_hThread, 10000);
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}

BOOL UMyNamePipeThread::SendData(PPIPE_HEADER pItem)
{
	if (m_pImpl==NULL) return FALSE;
	return m_pImpl->PushSendData(pItem);
}

DWORD WINAPI UMyNamePipeThread::ThreadFunction(void* pContext)
{
	return reinterpret_cast<UMyNamePipeThread*>(pContext)->ThreadMethod();
}

DWORD UMyNamePipeThread::ThreadMethod()
{
	DWORD dwResult=0;
	__try
	{
		dwResult=Execute();
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
	return dwResult;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
UMyNamePipeServer::UMyNamePipeServer()
:m_dwState(CONNECTING_STATE)
{
	memset(&m_overlapped,0,sizeof(m_overlapped));
	memset(m_szDebugLog,0,sizeof(m_szDebugLog));
}

UMyNamePipeServer::~UMyNamePipeServer()
{
}

DWORD UMyNamePipeServer::Execute()
{
	if (IsOpen()) Close();
	Create(m_szPipeName,PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 4096, 4096, 1, NULL);
	memset(&m_overlapped,0,sizeof(m_overlapped));
	::ResetEvent(m_hPipeEvent);
	m_overlapped.hEvent=m_hPipeEvent;

	DWORD dwBytesTransferred=0,dwErr=0;
	DWORD dwBytesRead=0,dwBytesWrite=0;
	
	
	PPIPE_HEADER pSendItem=NULL;

	BOOL bSuccess=TRUE;

	m_bWaitConnect=ConnectClient(m_overlapped);
	m_dwState=m_bWaitConnect? WAIT_CONNECTING_STATE : RECONNECTED_STATE;
	
	while(::WaitForSingleObject(m_hExitEvent, 500)!=WAIT_OBJECT_0)
	{
		if (m_dwState==WAIT_CONNECTING_STATE)
		{
			bSuccess=::GetOverlappedResult(m_hPipe,&m_overlapped,&dwBytesTransferred,TRUE);
			if (!bSuccess) m_dwState=RECONNECTED_STATE;
			else 
			{
				OutputDebugString(_T("[UMyNamePipeServer::Execute] Client connected.\r\n"));
				m_dwState=READING_STATE;
			}
		}
		else if (m_dwState==RECONNECTED_STATE)
		{
			DisconnectAndReconnect();
		}
		else if (m_dwState==READING_STATE)
		{
			bSuccess=Read(m_buffer,sizeof(m_buffer)-1, dwBytesRead, &m_overlapped);
			// The read operation completed successfully. 
			if (bSuccess && dwBytesRead != 0) 
			{ 
				wsprintf(m_szDebugLog,_T("[UMyNamePipeServer::Execute] Read pipe data. dwByteRead=%d\r\n"),dwBytesRead);
				OutputDebugString(m_szDebugLog);
				m_dwState = WRITING_STATE; 
				continue; 
			} 
			// The read operation is still pending. 
			dwErr = GetLastError(); 
			if (!bSuccess && (dwErr == ERROR_IO_PENDING)) 
			{ 
				m_dwState=WAIT_READING_STATE;
				continue; 
			} 
		}
		else if (m_dwState==WRITING_STATE)
		{
			pSendItem=m_pImpl->PopSendData();
			if (pSendItem!=NULL) 
			{
				bSuccess=Write(pSendItem,pSendItem->_lPackSize,dwBytesWrite,NULL);
				if (bSuccess && dwBytesWrite == pSendItem->_lPackSize) 
				{ 
				   m_dwState = READING_STATE; 
				}
				else
				{
					m_dwState=RECONNECTED_STATE;
				}

				delete[] pSendItem;
			}
			else
			{
				m_dwState = READING_STATE;
			}
		}
		else if (m_dwState==WAIT_READING_STATE)
		{
			OutputDebugString(_T("[UMyNamePipeServer::Execute] Wait client send data .....\r\n"));
			bSuccess=::GetOverlappedResult(m_hPipe,&m_overlapped,&dwBytesTransferred,TRUE);
			if (bSuccess && dwBytesTransferred>0)
			{
				dwBytesRead = dwBytesTransferred;
				OnRecvData((PPIPE_HEADER)m_buffer,dwBytesRead);
				m_dwState = WRITING_STATE; 
			}
			else
			{
				m_dwState=RECONNECTED_STATE;
			}
		}

	}

	OutputDebugString(_T("[UMyNamePipeServer::Execute] Exit. \r\n"));
	return 1;
}

BOOL UMyNamePipeServer::OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize)
{
	wsprintf(m_szDebugLog,_T("[UMyNamePipeServer::OnRecvData] Recv data. lPackSize=%d,_lCommandID=%d. \r\n"),pRecvItem->_lPackSize,pRecvItem->_lCommandID);
	OutputDebugString(m_szDebugLog);
	return TRUE;
}

void UMyNamePipeServer::DisconnectAndReconnect()
{
	//Disconnect the pipe instance. 
	if (!DisconnectClient()) 
	{
		//printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
	}

	// Call a subroutine to connect to the new client. 
	BOOL bSuccess= ConnectClient(m_overlapped);
	
	m_dwState = bSuccess ? WAIT_CONNECTING_STATE : RECONNECTED_STATE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UMyNamePipeClient::UMyNamePipeClient()
{
	memset(m_szDebugLog,0,sizeof(m_szDebugLog));
}

UMyNamePipeClient::~UMyNamePipeClient()
{
}

DWORD UMyNamePipeClient::Execute()
{
	DWORD dwBytesRead=0,dwBytesWrite=0,dwErr=0,dwBytesTransferred=0;
	BOOL bSuccess=TRUE;
	PPIPE_HEADER pSendItem=NULL;

	while(::WaitForSingleObject(m_hExitEvent, 500)!=WAIT_OBJECT_0)
	{
		if (!IsOpen())
		{
			if (!Open(_T("."),m_szPipeName,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, 0)) continue;
			DWORD dwMode=PIPE_NOWAIT;
			::SetNamedPipeHandleState(m_hPipe,&dwMode,NULL,NULL);
			OutputDebugString(_T("[UMyNamePipeClient::Execute] Connected server."));
			m_dwState=WRITING_STATE;
		}

		bSuccess=Read(m_buffer,sizeof(m_buffer)-1, dwBytesRead, NULL);
		if (bSuccess && dwBytesRead>0)
		{
			OnRecvData((PPIPE_HEADER)m_buffer,dwBytesRead);
		}
		else
		{
			pSendItem=m_pImpl->PopSendData();
			if (pSendItem!=NULL) 
			{
				bSuccess=Write(pSendItem,pSendItem->_lPackSize,dwBytesWrite,NULL);
				if (!bSuccess || dwBytesWrite != pSendItem->_lPackSize) 
					Close();

				delete[] pSendItem;
			}
		}
	}

	return 1;
}

BOOL UMyNamePipeClient::OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize)
{
	wsprintf(m_szDebugLog,_T("[UMyNamePipeClient::OnRecvData] Recv data. lPackSize=%d,_lCommandID=%d."),pRecvItem->_lPackSize,pRecvItem->_lCommandID);
	OutputDebugString(m_szDebugLog);
	return TRUE;
}



}}
