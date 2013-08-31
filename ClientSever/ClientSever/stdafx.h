#include <stdio.h>
#include "TCHAR.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

void Client();
void Server();
void Select();//选择模型
void EventSelect();//事件选择
void OverlappedEvent();//事件通知重叠IO
void OverlappedCompletion();//完成例程重叠IO
void IOCP();//完成端口
void ThreadPool();//线程池
void Distributed();//分布式

DWORD WINAPI WorkerThread(LPVOID lpParameter);
DWORD WINAPI WorkerThread2(LPVOID lpParameter);
DWORD WINAPI WorkerThread3(LPVOID lpParameter);
DWORD WINAPI WorkerThread4(LPVOID lpParameter);
DWORD WINAPI WorkerThread5(LPVOID lpParameter);
void Cleanup(int index);
void Cleanup2(int index);

#define PORT       5150
#define MSGSIZE    1024

typedef struct
{
  WSAOVERLAPPED overlap;
  WSABUF        Buffer;
  char          szMessage[MSGSIZE];
  DWORD         NumberOfBytesRecvd;
  DWORD         Flags;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct
{
WSAOVERLAPPED overlap;
WSABUF        Buffer;
  char          szMessage[MSGSIZE];
DWORD         NumberOfBytesRecvd;
DWORD         Flags;
SOCKET        sClient;
}PER_IO_OPERATION_DATA2, *LPPER_IO_OPERATION_DATA2;

typedef enum
{
  RECV_POSTED
}OPERATION_TYPE;
 
typedef struct
{
WSAOVERLAPPED  overlap;
WSABUF         Buffer;
  char           szMessage[MSGSIZE];
DWORD          NumberOfBytesRecvd;
DWORD          Flags;
OPERATION_TYPE OperationType;
}PER_IO_OPERATION_DATA3, *LPPER_IO_OPERATION_DATA3;