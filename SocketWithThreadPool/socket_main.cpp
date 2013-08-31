#include <iostream>
#include "tcp_svr_socket.h"

#define SIZE 1024*8
void Test()
{
	CTcpSvrSocket tcpSocket;
	WSADATA wsaData = {0};
	int err;
	
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(!err)
	{
		cout<<"已打开套接字"<<endl;
	}
	else
	{
		cout<<"套接字未打开！"<<endl;
		return;
	}
	
	if(tcpSocket.InitSvrSockets(9100,100))
	{
		while(1);
	}
	else
		cout<<"error"<<endl;
	WSACleanup();
}

void TcpServer()
{
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest=MAKEWORD(1,1);
	int err;
	err=WSAStartup(myVersionRequest,&wsaData);
	if (!err)
	{
		printf("已打开套接字\n");
	} 
	else
	{
		//进一步绑定套接字
		printf("嵌套字未打开!");
		return;
	}
	CTcpSvrSocket serSocket;//创建了可识别套接字
	//需要绑定的参数
	if(serSocket.InitSvrSockets(9100,10))
	{
		while(1)
		{
			serSocket.OnAccept();
		}
	}
	WSACleanup();
}

void TcpClient()
{
	FILE *fp;
	int err;
	int length=0;

	SOCKET clientSocket;
	SOCKADDR_IN clientsock_in;
	char receiveBuf[100]={"0"};
	char ip_addr[16]={"127.0.0.1"};
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
	if (!err)
	{
		printf("客户端嵌套字已经打开!\n");
	}
	else
	{
		printf("客户端的嵌套字打开失败!\n");
		return;//结束
	}
	clientSocket=socket(AF_INET,SOCK_STREAM,0);
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(ip_addr);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(9100);
	connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
	recv(clientSocket,receiveBuf,101,0);
	printf("%s\n",receiveBuf); 
	while(1);
	WSACleanup();
}

void Client()
{
	FILE *fp;
	int err;
	int length=0;

	SOCKET clientSocket;
	SOCKADDR_IN clientsock_in;
	char receiveBuf[100]={"0"};
	char ip_addr[16]={"127.0.0.1"};
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
	if (!err)
	{
		printf("客户端嵌套字已经打开!\n");
	}
	else
	{
		printf("客户端的嵌套字打开失败!\n");
		return;//结束
	}
	clientSocket=socket(AF_INET,SOCK_STREAM,0);
	/*printf("请输入主机IP：\n");
	scanf("%s",ip_addr);*/
	//连接服务器
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(ip_addr);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(6000);
	connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
	recv(clientSocket,receiveBuf,101,0);
	printf("%s\n",receiveBuf); 
	char path[100]={"0"};
	gets(path);
	int i=0;
	do
	{
	   printf("请输入文件地址：\n"); 
	   gets(path);
	   while(path[i]!='\0')
	   {
		if(path[i]=='/')
		 path[i]='\\';
		i++;
	   }
	   i=0;
	   if((fp=fopen(path,"rb"))==NULL)
	   {
		i=1;
		printf("文件未打开\n");
	   }
	}while(i); 
	fseek(fp,0L,SEEK_END);
		length=ftell(fp);
	printf("待传送文件大小： %d\n",length);
	printf("文件已经打开 等待主机消息......\n");
	//得到主机开始传送消息
	recv(clientSocket,receiveBuf,101,0);
	printf("%s\n",receiveBuf);
	if(strcmp(receiveBuf,"开始传送")==0)
	{
	   //传送文件长度
	   char sendBuf[20];
	   ltoa(length,sendBuf,10);
	   send(clientSocket,sendBuf,21,0);
	   fseek(fp,0L,SEEK_SET);
	   //传送文件
	   long int y=0;
	   double cent;
	   char trans[SIZE];
	   while(!feof(fp))
	   {
	   
		fread(trans,1,SIZE,fp);
		y=y+SIZE;
		if(y<length)
		{
		 cent=(double)y*100.0/(double)length;
		 printf("已发送： %4.2f％\n",cent);
		 send(clientSocket,trans,SIZE+1,0);
		}
		else
		{
		 send(clientSocket,trans,length+SIZE-y+1,0);
		 closesocket(clientSocket);
		 WSACleanup();
		}
	   }
	   fclose(fp);
	}
	printf("文件发送完毕\n");
	system("pause");

}

void CServer()
{
	//创建套接字
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest=MAKEWORD(1,1);
	int err;
	err=WSAStartup(myVersionRequest,&wsaData);
	if (!err)
	{
		printf("已打开套接字\n");
	} 
	else
	{
		//进一步绑定套接字
		printf("嵌套字未打开!");
		return;
	}
	SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);//创建了可识别套接字
	//需要绑定的参数
	SOCKADDR_IN addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip地址
	addr.sin_port=htons(6000);//绑定端口
	bind(serSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR));//绑定完成
	listen(serSocket,5);//其中第二个参数代表能够接收的最多的连接数
	//////////////////////////////////////////////////////////////////////////
	//开始进行监听
	//////////////////////////////////////////////////////////////////////////
	SOCKADDR_IN clientsocket;
	int len=sizeof(SOCKADDR);
	SOCKET serConn;
	char sendBuf[50];
	serConn=accept(serSocket,(SOCKADDR*)&clientsocket,&len);//如果这里不是accept而是conection的话。。就会不断的监听
	sprintf(sendBuf,"welcome %s to bejing",inet_ntoa(clientsocket.sin_addr));//找对对应的IP并且将这行字打印到那里
	//发送欢迎信息
	send(serConn,sendBuf,strlen(sendBuf)+1,0);

	FILE *fp;
	lable1: printf("客户端已打开\n请输入存放文件地址：\n");
	char path[100]={"0"};
	int i=0;
	while(path[i]!='\0')
	{
	   if(path[i]=='/')
		path[i]='\\';
	   i++;
	}
	gets(path);
	if((fp=fopen(path,"wb+"))==NULL)
	{
	   printf("文件未打开\n");
	   goto lable1;
	}
	else
	{
	   printf("文件已打开 开始文件传输......\n");
	   //发送开始标志
	   send(serConn,"开始传送",strlen("开始传送")+1,0); 
	   //得到文件大小
	   char datalength[20];
	   long int length=0;
	   recv(serConn,datalength,21,0);
	   length=atol(datalength);
	   printf("得到文件大小： %d\n",length);
	   //开始传送
	   double cent=0.0;
	   char receiveBuf[SIZE];
	   long int x=0;
	   while (1)
	   {
		x=x+SIZE;
		if(x<length)
		{
		 cent=(double)x*100.0/(double)length;
		 printf("已接收： %4.2f％\n",cent);
		 recv(serConn,receiveBuf,SIZE+1,0);
		 fwrite(receiveBuf,1,SIZE,fp);
		}
		else
		{
		 recv(serConn,receiveBuf,length+SIZE-x+1,0);
		 printf("文件接收完毕\n");
		 fwrite(receiveBuf,1,length+SIZE-x,fp);
		 fclose(fp);
		 break;
		}
	   }
	}
	closesocket(serConn);//关闭
	WSACleanup();//释放资源的操作
	system("pause");

}

void main(int argc, char *argv[])
{	
	//Test();
	//CServer();
	//Client();
	TcpServer();
	//TcpClient();
}