#pragma once
#include "UMyNamePipe.h"

#define UMS_NAME_PIPE_MESSAGE	0x03001

enum UMS_REMOTE_COMMAND_ID
{
	CMD_OPEN_SYSTEM_SPACE_ID=1,	//��ϵͳ������
	CMD_POP_SYSTEM_SPACE_ID=2,	//����ϵͳ������
	CMD_LOAD_CUSTOM_BLOCK_ID,	//���Զ�����
	CMD_SHOWWINDOW_ID,			//��ʾ������
	CMD_HIDEWINDOW_ID,
	CMD_CLOSE_APP_ID,			//�رճ���
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
