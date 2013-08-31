#pragma once

namespace Zealink { namespace Common {

class UMyNamePipe
{
public:
	UMyNamePipe(void);
	virtual ~UMyNamePipe(void);

	BOOL Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize, 
			DWORD dwInBufferSize, DWORD dwDefaultTimeOut=1, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	BOOL Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess, 
			DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwFlagsAndAttributes = 0);
	BOOL Close();

	BOOL ConnectClient(OVERLAPPED& overlapped);
	BOOL DisconnectClient();

	inline void SetPipeName(LPCTSTR lpName) { _tcscpy_s(m_szPipeName,lpName); }
	inline LPCTSTR GetPipeName() const { return m_szPipeName; }
	
	BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,DWORD& dwNumberOfBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
	BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);	

	BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,DWORD& dwNumberOfBytesRead, LPOVERLAPPED lpOverlapped = NULL);
	BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

	//属性
	inline BOOL	IsOpen() const { return (m_hPipe != INVALID_HANDLE_VALUE); }
	BOOL IsServerPipe(BOOL& bServerPipe) const;
	BOOL IsClientPipe(BOOL& bClientPipe) const;
	BOOL GetClientUserName(LPTSTR lpName,long lLen) const;
	inline operator HANDLE() const { return m_hPipe; };

	static BOOL ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut=1000);

protected:
	BOOL GetUMyNamedPipeInfo(DWORD& dwFlags) const;

	TCHAR	m_szPipeName[32];
	HANDLE	m_hPipe;
};


class UMyNamePipeThread : public UMyNamePipe
{
public:

#pragma  pack(1)	//结构体全部1字节对齐

	typedef struct tagPipeHeader
	{
		long _lCommandID;
		long _lPackSize;	//包大小
		long _lExtendData[3];
	}PIPE_HEADER,*PPIPE_HEADER;

#pragma  pack()

	enum STATE_ID { CONNECTING_STATE=0,READING_STATE,WRITING_STATE,WAIT_CONNECTING_STATE,RECONNECTED_STATE,WAIT_READING_STATE };

	UMyNamePipeThread();
	virtual ~UMyNamePipeThread();

	virtual BOOL Initialize(size_t stackSize = 0);
	virtual void Stop();

	BOOL SendData(PPIPE_HEADER pItem);
	

protected:
	virtual DWORD Execute() = 0;

	HANDLE	m_hThread,m_hExitEvent,m_hPipeEvent;
	DWORD	m_dwThreadId;
	

	char	m_buffer[1024];

	struct Impl;
	Impl*	m_pImpl;

private:
	static DWORD WINAPI ThreadFunction(void* pContext);

	DWORD ThreadMethod();
};


class UMyNamePipeServer : public UMyNamePipeThread
{
public:
	UMyNamePipeServer();
	virtual ~UMyNamePipeServer();

protected:
	void DisconnectAndReconnect();
	virtual DWORD Execute();

	virtual BOOL OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize);

	OVERLAPPED	m_overlapped;
	DWORD	m_dwState;
	BOOL	m_bWaitConnect;
	TCHAR	m_szDebugLog[256];
};

class UMyNamePipeClient : public UMyNamePipeThread
{
public:
	UMyNamePipeClient();
	virtual ~UMyNamePipeClient();

protected:
	virtual DWORD Execute();

	virtual BOOL OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize);

	DWORD		m_dwState;
	TCHAR		m_szDebugLog[256];
};

}}
