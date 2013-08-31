#include "stdafx.h"
int    g_iTotalConn = 0;
SOCKET g_CliSocketArr[FD_SETSIZE];
SOCKET   g_CliSocketArr2[MAXIMUM_WAIT_OBJECTS];
WSAEVENT g_CliEventArr[MAXIMUM_WAIT_OBJECTS];
LPPER_IO_OPERATION_DATA g_pPerIODataArr[MAXIMUM_WAIT_OBJECTS];
void Server()
{
	printf("ѡ��IOģ��. 1:select 2:�첽ѡ��(�������) 3:�¼�ѡ�� 4:�¼�֪ͨ�ص�I/O 5:������̷�ʽ�ص� 6:��ɶ˿�\n");
	int a=1;
	scanf_s("%d",&a);
	switch(a)
	{
	case 1:
		Select();
		break;
	case 3:
		EventSelect();
	case 4:
		OverlappedEvent();
	case 5:
		OverlappedCompletion();
	case 6:
		IOCP();
	//case 7:
	//	ThreadPool();
	//case 8:
	//	Distributed();
	}
}
int CALLBACK ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData)
{
	if (g_iTotalConn < FD_SETSIZE)
	  return CF_ACCEPT;
	else
	  return CF_REJECT;
}
void Select()
{
	SOCKET      sListen, sClient;
	SOCKADDR_IN local, client;
	int         iaddrSize = sizeof(SOCKADDR_IN);
	DWORD       dwThreadId;

	//��������
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// ��
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

	// ����
	listen(sListen, 3);

	// �����������߳�
	CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId); 

	while (TRUE)
	{
		// �յ�һ������
		sClient = WSAAccept(sListen,(struct sockaddr *)&client, &iaddrSize,ConditionFunc,NULL);
		//sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
		printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		
		// ���յ������Ӽ���socket����
		g_CliSocketArr[g_iTotalConn++] = sClient;
	}
}


DWORD WINAPI WorkerThread(LPVOID lpParam)
{
  int            i;
  fd_set         fdread;
  int            ret;
  struct timeval tv = {1, 0};
  char           szMessage[MSGSIZE]={0};
 
  while (TRUE)
  {
	FD_ZERO(&fdread);//��fdread��ʼ���ռ�
	for (i = 0; i < g_iTotalConn; i++)
	{
	  FD_SET(g_CliSocketArr[i], &fdread);//��Ҫ�����׽ӿڼ��뵽������
	}

	ret = select(0, &fdread, NULL, NULL, &tv);//ÿ��һ��ʱ�䣬���ɶ��Ե��׽ӿ�

	if (ret == 0)
	{
	  // Time expired
	  continue;
	}

	for (i = 0; i < g_iTotalConn; i++)
	{
	  if (FD_ISSET(g_CliSocketArr[i], &fdread))//����ɶ�
	  {
		// һ�����¼�
		memset(szMessage,0,sizeof(szMessage));
		ret = recv(g_CliSocketArr[i], szMessage, MSGSIZE, 0);
		
		if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
		{
		//socket�ͻ��˹ر�
			printf("Client socket %d closed.\n", g_CliSocketArr);
			closesocket(g_CliSocketArr[i]);
			if (i < g_iTotalConn - 1)
			{           
				g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn];
			}
		}
		else
		{
			//�յ���Ϣ
			if(szMessage[0])
				printf("%s",szMessage);
			szMessage[ret] = '\0';
			send(g_CliSocketArr[i], szMessage, strlen(szMessage), 0);
		}
	  }
	}
 }
}
//////////////////////////////////////////////////////////////////////////////////////
#define WM_SOCKET WM_USER+0
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  static TCHAR szAppName[] = _T("AsyncSelect Model");
  HWND         hwnd ;
  MSG          msg ;
  WNDCLASS     wndclass ;
 
  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
  wndclass.lpfnWndProc   = WndProc ;
  wndclass.cbClsExtra    = 0 ;
  wndclass.cbWndExtra    = 0 ;
  wndclass.hInstance     = hInstance ;
  wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
  wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
  wndclass.lpszMenuName  = NULL ;
  wndclass.lpszClassName = szAppName ;
 
  if (!RegisterClass(&wndclass))
  {
    MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
    return 0 ;
  }
 
  hwnd = CreateWindow (szAppName,                  // window class name
                       TEXT ("AsyncSelect Model"), // window caption
                       WS_OVERLAPPEDWINDOW,        // window style
                       CW_USEDEFAULT,              // initial x position
                       CW_USEDEFAULT,              // initial y position
                       CW_USEDEFAULT,              // initial x size
                       CW_USEDEFAULT,              // initial y size
                       NULL,                       // parent window handle
                       NULL,                       // window menu handle
                       hInstance,                  // program instance handle
                       NULL) ;                     // creation parameters
 
  ShowWindow(hwnd, iCmdShow);
  UpdateWindow(hwnd);
 
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg) ;
    DispatchMessage(&msg) ;
  }
 
  return msg.wParam;
}
 
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  WSADATA       wsd;
  static SOCKET sListen;
  SOCKET        sClient;
  SOCKADDR_IN   local, client;
  int           ret, iAddrSize = sizeof(client);
  char          szMessage[MSGSIZE];
 
switch (message)
{
case WM_CREATE:
    //��ʼ��socket��
	WSAStartup(0x0202, &wsd);
 
  // ��������
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
  // ��
    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	bind(sListen, (struct sockaddr *)&local, sizeof(local));
 
  // ����
    listen(sListen, 3);
 
    // ����FD_ACCEPT�¼�
	 WSAAsyncSelect(sListen, hwnd, WM_SOCKET, FD_ACCEPT);
	 return 0;
 
  case WM_DESTROY:
    closesocket(sListen);
    WSACleanup();
    PostQuitMessage(0);
    return 0;
 
  case WM_SOCKET:
    if (WSAGETSELECTERROR(lParam))
    {
      closesocket(wParam);
      break;
    }
   
    switch (WSAGETSELECTEVENT(lParam))//ȡ��λ�ֽڣ������¼�
    {
    case FD_ACCEPT:
      // ���ܵ�һ������
      sClient = accept(wParam, (struct sockaddr *)&client, &iAddrSize);
     
      // ����FD_READ ��FD_CLOSE�¼�
      WSAAsyncSelect(sClient, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
      break;
 
    case FD_READ:
      ret = recv(wParam, szMessage, MSGSIZE, 0);
 
      if (ret == 0 || ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
      {
        closesocket(wParam);
      }
      else
      {
        szMessage[ret] = '\0';
        send(wParam, szMessage, strlen(szMessage), 0);
      }
      break;
     
    case FD_CLOSE:
      closesocket(wParam);     
      break;
    }
    return 0;
  }
 
  return DefWindowProc(hwnd, message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////////
void EventSelect()
{
  SOCKET      sListen, sClient;
  SOCKADDR_IN local, client;
  DWORD       dwThreadId;
  int         iaddrSize = sizeof(SOCKADDR_IN);
 
  // ��������socket
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // ��
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // ����
  listen(sListen, 3);
 
  // �����߳�
  CreateThread(NULL, 0, WorkerThread2, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // �յ�һ������
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    // �������¼�
    g_CliSocketArr2[g_iTotalConn] = sClient;//�������ӵ��׽ӿ�
	g_CliEventArr[g_iTotalConn] = WSACreateEvent();//�����¼�������
	//���׽ӿ��Ͻ�һ�����������¼��� �¼����������һ��
    WSAEventSelect(g_CliSocketArr2[g_iTotalConn],//�׽ӿ�
                   g_CliEventArr[g_iTotalConn],//�¼�����
                   FD_READ | FD_CLOSE);//�����¼�
    g_iTotalConn++;
  }
}
 
DWORD WINAPI WorkerThread2(LPVOID lpParam)
{
  int              ret, index;
  WSANETWORKEVENTS NetworkEvents;
  char             szMessage[MSGSIZE];
 
  while (TRUE)
  { //���ص��·��ص��¼�����
	ret = WSAWaitForMultipleEvents(g_iTotalConn,//�����еľ����Ŀ
	g_CliEventArr,//ָ��һ���¼������������ָ��
	FALSE, //T�������Żأ�F��һ���ͻ�
	1000, //��ʱ���
	FALSE);//�Ƿ�ִ���������
	if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
	{
	  continue;
	}

	index = ret - WSA_WAIT_EVENT_0;
	//���׽ӿ��ϲ�ѯ���¼���������������¼�
	WSAEnumNetworkEvents(g_CliSocketArr2[index], g_CliEventArr[index], &NetworkEvents);
	//����FD-READ�����¼�
	if (NetworkEvents.lNetworkEvents & FD_READ)
	{
	  // �ӿͻ����յ���Ϣ
	  ret = recv(g_CliSocketArr2[index], szMessage, MSGSIZE, 0);
	  if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
	  {
		Cleanup(index);
	  }
	  else
	  {
		szMessage[ret] = '\0';
		send(g_CliSocketArr2[index], szMessage, strlen(szMessage), 0);
	  }
	}
	//����FD-CLOSE�����¼�
	if (NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			Cleanup(index);
		}
	}
	return 0;
}
 
void Cleanup(int index)
{
	closesocket(g_CliSocketArr2[index]);
	WSACloseEvent(g_CliEventArr[index]);
 
	if (index < g_iTotalConn - 1)
	{
	  g_CliSocketArr2[index] = g_CliSocketArr2[g_iTotalConn - 1];
	  g_CliEventArr[index] = g_CliEventArr[g_iTotalConn - 1];
	}
	g_iTotalConn--;
}
///////////////////////////////////////////////////////////////////////////////
 
void OverlappedEvent()
{
  SOCKET      sListen, sClient;
  SOCKADDR_IN local, client;
  DWORD       dwThreadId;
  int         iaddrSize = sizeof(SOCKADDR_IN);
 
  // ����
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // ��
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // ����
  listen(sListen, 3);
 
  // �����߳�
  CreateThread(NULL, 0, WorkerThread3, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // ����һ������
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    g_CliSocketArr2[g_iTotalConn] = sClient;
   
    // ����ռ�
    g_pPerIODataArr[g_iTotalConn] = (LPPER_IO_OPERATION_DATA)HeapAlloc(
      GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      sizeof(PER_IO_OPERATION_DATA));
    g_pPerIODataArr[g_iTotalConn]->Buffer.len = MSGSIZE;
    g_pPerIODataArr[g_iTotalConn]->Buffer.buf = g_pPerIODataArr[g_iTotalConn]->szMessage;
    g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent = WSACreateEvent();
 
    // ����һ���첽����
    WSARecv(
      g_CliSocketArr2[g_iTotalConn],
      &g_pPerIODataArr[g_iTotalConn]->Buffer,
      1,
      &g_pPerIODataArr[g_iTotalConn]->NumberOfBytesRecvd,
      &g_pPerIODataArr[g_iTotalConn]->Flags,
      &g_pPerIODataArr[g_iTotalConn]->overlap,
      NULL);
   
    g_iTotalConn++;
  }
 
  closesocket(sListen);
  WSACleanup();
}
 
DWORD WINAPI WorkerThread3(LPVOID lpParam)
{
  int   ret, index;
  DWORD cbTransferred;
 
  while (TRUE)
  {
    ret = WSAWaitForMultipleEvents(g_iTotalConn, g_CliEventArr, FALSE, 1000, FALSE);
    if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
    {
      continue;
    }
 
    index = ret - WSA_WAIT_EVENT_0;
    WSAResetEvent(g_CliEventArr[index]);
 
    WSAGetOverlappedResult(
      g_CliSocketArr2[index],
      &g_pPerIODataArr[index]->overlap,
      &cbTransferred,
      TRUE,
      &g_pPerIODataArr[g_iTotalConn]->Flags);
 
    if (cbTransferred == 0)
    {
      // �ͻ��˶Ͽ�����
      Cleanup2(index);
    }
    else
    {
      // g_pPerIODataArr[index]->szMessage �����յ�����Ϣ
      g_pPerIODataArr[index]->szMessage[cbTransferred] = '\0';
      send(g_CliSocketArr2[index], g_pPerIODataArr[index]->szMessage,\
        cbTransferred, 0);
 
      // ������һ���첽����
      WSARecv(
        g_CliSocketArr2[index],
        &g_pPerIODataArr[index]->Buffer,
        1,
        &g_pPerIODataArr[index]->NumberOfBytesRecvd,
        &g_pPerIODataArr[index]->Flags,
        &g_pPerIODataArr[index]->overlap,
        NULL);
    }
  }
 
  return 0;
}
 
void Cleanup2(int index)
{
  closesocket(g_CliSocketArr2[index]);
  WSACloseEvent(g_CliEventArr[index]);
  HeapFree(GetProcessHeap(), 0, g_pPerIODataArr[index]);
 
  if (index < g_iTotalConn - 1)
  {
    g_CliSocketArr2[index] = g_CliSocketArr2[g_iTotalConn - 1];
    g_CliEventArr[index] = g_CliEventArr[g_iTotalConn - 1];
    g_pPerIODataArr[index] = g_pPerIODataArr[g_iTotalConn - 1];
  }
 
  g_pPerIODataArr[--g_iTotalConn] = NULL;
}

//////////////////////////////////////////////////////////////////////////////
void CALLBACK CompletionROUTINE(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
 
SOCKET g_sNewClientConnection;
BOOL   g_bNewConnectionArrived = FALSE;
 
void OverlappedCompletion()
{
  SOCKET      sListen;
  SOCKADDR_IN local, client;
  DWORD       dwThreadId;
  int         iaddrSize = sizeof(SOCKADDR_IN);
 
  // ����
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // ��
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // ����
  listen(sListen, 3);
 
  // ���������߳�
  CreateThread(NULL, 0, WorkerThread4, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // ����һ������
    g_sNewClientConnection = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    g_bNewConnectionArrived = TRUE;
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
  }
}
 
DWORD WINAPI WorkerThread4(LPVOID lpParam)
{
LPPER_IO_OPERATION_DATA2 lpPerIOData = NULL;
 
  while (TRUE)
  {
    if (g_bNewConnectionArrived)
    {
      // ������socket����һ���첽����
      lpPerIOData = (LPPER_IO_OPERATION_DATA2)HeapAlloc(
        GetProcessHeap(),
        HEAP_ZERO_MEMORY,
        sizeof(PER_IO_OPERATION_DATA));
      lpPerIOData->Buffer.len = MSGSIZE;
      lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
      lpPerIOData->sClient = g_sNewClientConnection;
     
      WSARecv(lpPerIOData->sClient,
        &lpPerIOData->Buffer,
        1,
        &lpPerIOData->NumberOfBytesRecvd,
        &lpPerIOData->Flags,
        &lpPerIOData->overlap,
        CompletionROUTINE);     
     
      g_bNewConnectionArrived = FALSE;
    }
 
    SleepEx(1000, TRUE);
  }
  return 0;
}
 
void CALLBACK CompletionROUTINE(DWORD dwError,
                                DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped,
                                DWORD dwFlags)
{
  LPPER_IO_OPERATION_DATA2 lpPerIOData = (LPPER_IO_OPERATION_DATA2)lpOverlapped;
 
  if (dwError != 0 || cbTransferred == 0)
	{
    // ���ӹر�
  closesocket(lpPerIOData->sClient);
  HeapFree(GetProcessHeap(), 0, lpPerIOData);
	}
  else
  {
    lpPerIOData->szMessage[cbTransferred] = '\0';
    send(lpPerIOData->sClient, lpPerIOData->szMessage, cbTransferred, 0);
   
    // ������һ���첽����
    memset(&lpPerIOData->overlap, 0, sizeof(WSAOVERLAPPED));
    lpPerIOData->Buffer.len = MSGSIZE;
    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;   
 
    WSARecv(lpPerIOData->sClient,
      &lpPerIOData->Buffer,
      1,
      &lpPerIOData->NumberOfBytesRecvd,
      &lpPerIOData->Flags,
      &lpPerIOData->overlap,
      CompletionROUTINE);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void IOCP()
{
  SOCKET                  sListen, sClient;
  SOCKADDR_IN             local, client;
  DWORD                   i, dwThreadId;
  int                     iaddrSize = sizeof(SOCKADDR_IN);
  HANDLE                  CompletionPort = INVALID_HANDLE_VALUE;
  SYSTEM_INFO             systeminfo;
  LPPER_IO_OPERATION_DATA3 lpPerIOData = NULL;

 
  // ������ɶ˿�
  CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
 
  //�����߳�
  GetSystemInfo(&systeminfo);
  for (i = 0; i < systeminfo.dwNumberOfProcessors; i++)
  {
    CreateThread(NULL, 0, WorkerThread5, CompletionPort, 0, &dwThreadId);
  }
 
  // ��������socket
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // ��
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // ����
  listen(sListen, 3);
 
  while (TRUE)
  {
    // ����һ������
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    // ����ɶ˿ڰ�
    CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)sClient, 0);
   
    // �����첽����
    lpPerIOData = (LPPER_IO_OPERATION_DATA3)HeapAlloc(
      GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      sizeof(PER_IO_OPERATION_DATA3));
    lpPerIOData->Buffer.len = MSGSIZE;
    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
    lpPerIOData->OperationType = RECV_POSTED;
    WSARecv(sClient,
      &lpPerIOData->Buffer,
      1,
      &lpPerIOData->NumberOfBytesRecvd,
      &lpPerIOData->Flags,
      &lpPerIOData->overlap,
      NULL);
  }
 
	PostQueuedCompletionStatus(CompletionPort, 0xFFFFFFFF, 0, NULL);
	CloseHandle(CompletionPort);
	closesocket(sListen);
}

DWORD WINAPI WorkerThread5(LPVOID CompletionPortID)
{
  HANDLE                  CompletionPort=(HANDLE)CompletionPortID;
  DWORD                   dwBytesTransferred;
  SOCKET                  sClient;
  LPPER_IO_OPERATION_DATA3 lpPerIOData = NULL;
 
  while (TRUE)
  {
    GetQueuedCompletionStatus(
      CompletionPort,
      &dwBytesTransferred,
      (PULONG_PTR)&sClient,
      (LPOVERLAPPED *)&lpPerIOData,
      INFINITE);
    if (dwBytesTransferred == 0xFFFFFFFF)
    {
      return 0;
    }
   
    if (lpPerIOData->OperationType == RECV_POSTED)
    {
      if (dwBytesTransferred == 0)
      {
        // ���ӹر�
        closesocket(sClient);
        HeapFree(GetProcessHeap(), 0, lpPerIOData);       
      }
      else
      {
        lpPerIOData->szMessage[dwBytesTransferred] = '\0';
        send(sClient, lpPerIOData->szMessage, dwBytesTransferred, 0);
       
        // ������һ���첽����
        memset(lpPerIOData, 0, sizeof(PER_IO_OPERATION_DATA3));
        lpPerIOData->Buffer.len = MSGSIZE;
        lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
        lpPerIOData->OperationType = RECV_POSTED;
        WSARecv(sClient,
          &lpPerIOData->Buffer,
          1,
          &lpPerIOData->NumberOfBytesRecvd,
          &lpPerIOData->Flags,
          &lpPerIOData->overlap,
          NULL);
      }
    }
  }
return 0;
}