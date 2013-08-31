#pragma once
#include "UMyNamePipe.h"

#define UMS_NAME_PIPE_MESSAGE	0x03001

enum UMS_REMOTE_COMMAND_ID
{
	CMD_OPEN_SYSTEM_SPACE_ID=1,	//打开系统工作区
	CMD_POP_SYSTEM_SPACE_ID=2,	//弹出系统工作区
	CMD_LOAD_CUSTOM_BLOCK_ID,	//读自定义板块
	CMD_SHOWWINDOW_ID,			//显示主窗口
	CMD_HIDEWINDOW_ID,
	CMD_CLOSE_APP_ID,			//关闭程序
};

class UMSNamePipeServer : public Zealink::Common::UMyNamePipeServer
{
public:
	UMSNamePipeServer(void);
	virtual ~UMSNamePipeServer(void);

	inline void SetMessageHWND(HWND hWnd) { m_hWnd=hWnd; }

protected:

	virtual BOOL OnRecvData(PPIPE_HEADER pRecvItem,long lDataSize);

	HWND		m_hWnd;
};

class UMSNamePipeClient : public Zealink::Common::UMyNamePipeClient
{
public:
	UMSNamePipeClient(void);
	virtual ~UMSNamePipeClient(void);	
};

extern	UMSNamePipeServer g_UMSNamePipe;
extern	UMSNamePipeClient g_AddinNamePipe;
