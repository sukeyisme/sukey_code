#include "stdafx.h"
int    g_iTotalConn = 0;
SOCKET g_CliSocketArr[FD_SETSIZE];
SOCKET   g_CliSocketArr2[MAXIMUM_WAIT_OBJECTS];
WSAEVENT g_CliEventArr[MAXIMUM_WAIT_OBJECTS];
LPPER_IO_OPERATION_DATA g_pPerIODataArr[MAXIMUM_WAIT_OBJECTS];
void Server()
{
	printf("选择IO模型. 1:select 2:异步选择(另外代码) 3:事件选择 4:事件通知重叠I/O 5:完成例程方式重叠 6:完成端口\n");
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

	//创建监听
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 绑定
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

	// 监听
	listen(sListen, 3);

	// 创建任务处理线程
	CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId); 

	while (TRUE)
	{
		// 收到一个连接
		sClient = WSAAccept(sListen,(struct sockaddr *)&client, &iaddrSize,ConditionFunc,NULL);
		//sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
		printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		
		// 把收到的连接加入socket数组
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
	FD_ZERO(&fdread);//将fdread初始化空集
	for (i = 0; i < g_iTotalConn; i++)
	{
	  FD_SET(g_CliSocketArr[i], &fdread);//将要检查的套接口加入到集合中
	}

	ret = select(0, &fdread, NULL, NULL, &tv);//每隔一段时间，检查可读性的套接口

	if (ret == 0)
	{
	  // Time expired
	  continue;
	}

	for (i = 0; i < g_iTotalConn; i++)
	{
	  if (FD_ISSET(g_CliSocketArr[i], &fdread))//如果可读
	  {
		// 一个读事件
		memset(szMessage,0,sizeof(szMessage));
		ret = recv(g_CliSocketArr[i], szMessage, MSGSIZE, 0);
		
		if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
		{
		//socket客户端关闭
			printf("Client socket %d closed.\n", g_CliSocketArr);
			closesocket(g_CliSocketArr[i]);
			if (i < g_iTotalConn - 1)
			{           
				g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn];
			}
		}
		else
		{
			//收到消息
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
    //初始化socket库
	WSAStartup(0x0202, &wsd);
 
  // 创建监听
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
  // 绑定
    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	bind(sListen, (struct sockaddr *)&local, sizeof(local));
 
  // 监听
    listen(sListen, 3);
 
    // 设置FD_ACCEPT事件
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
   
    switch (WSAGETSELECTEVENT(lParam))//取低位字节，网络事件
    {
    case FD_ACCEPT:
      // 接受到一个连接
      sClient = accept(wParam, (struct sockaddr *)&client, &iAddrSize);
     
      // 设置FD_READ 和FD_CLOSE事件
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
 
  // 创建监听socket
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // 绑定
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // 监听
  listen(sListen, 3);
 
  // 创建线程
  CreateThread(NULL, 0, WorkerThread2, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // 收到一个连接
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    // 绑定网络事件
    g_CliSocketArr2[g_iTotalConn] = sClient;//接受连接的套接口
	g_CliEventArr[g_iTotalConn] = WSACreateEvent();//返回事件对象句柄
	//在套接口上将一个或多个网络事件与 事件对象关联在一起
    WSAEventSelect(g_CliSocketArr2[g_iTotalConn],//套接口
                   g_CliEventArr[g_iTotalConn],//事件对象
                   FD_READ | FD_CLOSE);//网络事件
    g_iTotalConn++;
  }
}
 
DWORD WINAPI WorkerThread2(LPVOID lpParam)
{
  int              ret, index;
  WSANETWORKEVENTS NetworkEvents;
  char             szMessage[MSGSIZE];
 
  while (TRUE)
  { //返回导致返回的事件对象
	ret = WSAWaitForMultipleEvents(g_iTotalConn,//数组中的句柄数目
	g_CliEventArr,//指向一个事件对象句柄数组的指针
	FALSE, //T，都进才回；F，一进就回
	1000, //超时间隔
	FALSE);//是否执行完成例程
	if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
	{
	  continue;
	}

	index = ret - WSA_WAIT_EVENT_0;
	//在套接口上查询与事件对象关联的网络事件
	WSAEnumNetworkEvents(g_CliSocketArr2[index], g_CliEventArr[index], &NetworkEvents);
	//处理FD-READ网络事件
	if (NetworkEvents.lNetworkEvents & FD_READ)
	{
	  // 从客户端收到消息
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
	//处理FD-CLOSE网络事件
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
 
  // 创建
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // 绑定
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // 监听
  listen(sListen, 3);
 
  // 创建线程
  CreateThread(NULL, 0, WorkerThread3, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // 接受一个连接
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    g_CliSocketArr2[g_iTotalConn] = sClient;
   
    // 分配空间
    g_pPerIODataArr[g_iTotalConn] = (LPPER_IO_OPERATION_DATA)HeapAlloc(
      GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      sizeof(PER_IO_OPERATION_DATA));
    g_pPerIODataArr[g_iTotalConn]->Buffer.len = MSGSIZE;
    g_pPerIODataArr[g_iTotalConn]->Buffer.buf = g_pPerIODataArr[g_iTotalConn]->szMessage;
    g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent = WSACreateEvent();
 
    // 启动一个异步处理
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
      // 客户端断开连接
      Cleanup2(index);
    }
    else
    {
      // g_pPerIODataArr[index]->szMessage 包含收到的信息
      g_pPerIODataArr[index]->szMessage[cbTransferred] = '\0';
      send(g_CliSocketArr2[index], g_pPerIODataArr[index]->szMessage,\
        cbTransferred, 0);
 
      // 启动另一个异步操作
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
 
  // 创建
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // 绑定
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // 监听
  listen(sListen, 3);
 
  // 创建工作线程
  CreateThread(NULL, 0, WorkerThread4, NULL, 0, &dwThreadId);
 
  while (TRUE)
  {
    // 接受一个连接
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
      // 给连接socket启动一个异步处理
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
    // 连接关闭
  closesocket(lpPerIOData->sClient);
  HeapFree(GetProcessHeap(), 0, lpPerIOData);
	}
  else
  {
    lpPerIOData->szMessage[cbTransferred] = '\0';
    send(lpPerIOData->sClient, lpPerIOData->szMessage, cbTransferred, 0);
   
    // 启动另一个异步操作
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

 
  // 创建完成端口
  CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
 
  //创建线程
  GetSystemInfo(&systeminfo);
  for (i = 0; i < systeminfo.dwNumberOfProcessors; i++)
  {
    CreateThread(NULL, 0, WorkerThread5, CompletionPort, 0, &dwThreadId);
  }
 
  // 创建监听socket
  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
  // 绑定
  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
 
  // 监听
  listen(sListen, 3);
 
  while (TRUE)
  {
    // 接受一个连接
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
 
    // 与完成端口绑定
    CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)sClient, 0);
   
    // 启动异步处理
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
        // 连接关闭
        closesocket(sClient);
        HeapFree(GetProcessHeap(), 0, lpPerIOData);       
      }
      else
      {
        lpPerIOData->szMessage[dwBytesTransferred] = '\0';
        send(sClient, lpPerIOData->szMessage, dwBytesTransferred, 0);
       
        // 启动另一个异步处理
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