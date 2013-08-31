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
		cout<<"�Ѵ��׽���"<<endl;
	}
	else
	{
		cout<<"�׽���δ�򿪣�"<<endl;
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
		printf("�Ѵ��׽���\n");
	} 
	else
	{
		//��һ�����׽���
		printf("Ƕ����δ��!");
		return;
	}
	CTcpSvrSocket serSocket;//�����˿�ʶ���׽���
	//��Ҫ�󶨵Ĳ���
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
	err=WSAStartup(versionRequired,&wsaData);//Э���İ汾��Ϣ
	if (!err)
	{
		printf("�ͻ���Ƕ�����Ѿ���!\n");
	}
	else
	{
		printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
		return;//����
	}
	clientSocket=socket(AF_INET,SOCK_STREAM,0);
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(ip_addr);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(9100);
	connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//��ʼ����
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
	err=WSAStartup(versionRequired,&wsaData);//Э���İ汾��Ϣ
	if (!err)
	{
		printf("�ͻ���Ƕ�����Ѿ���!\n");
	}
	else
	{
		printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
		return;//����
	}
	clientSocket=socket(AF_INET,SOCK_STREAM,0);
	/*printf("����������IP��\n");
	scanf("%s",ip_addr);*/
	//���ӷ�����
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(ip_addr);
	clientsock_in.sin_family=AF_INET;
	clientsock_in.sin_port=htons(6000);
	connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//��ʼ����
	recv(clientSocket,receiveBuf,101,0);
	printf("%s\n",receiveBuf); 
	char path[100]={"0"};
	gets(path);
	int i=0;
	do
	{
	   printf("�������ļ���ַ��\n"); 
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
		printf("�ļ�δ��\n");
	   }
	}while(i); 
	fseek(fp,0L,SEEK_END);
		length=ftell(fp);
	printf("�������ļ���С�� %d\n",length);
	printf("�ļ��Ѿ��� �ȴ�������Ϣ......\n");
	//�õ�������ʼ������Ϣ
	recv(clientSocket,receiveBuf,101,0);
	printf("%s\n",receiveBuf);
	if(strcmp(receiveBuf,"��ʼ����")==0)
	{
	   //�����ļ�����
	   char sendBuf[20];
	   ltoa(length,sendBuf,10);
	   send(clientSocket,sendBuf,21,0);
	   fseek(fp,0L,SEEK_SET);
	   //�����ļ�
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
		 printf("�ѷ��ͣ� %4.2f��\n",cent);
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
	printf("�ļ��������\n");
	system("pause");

}

void CServer()
{
	//�����׽���
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest=MAKEWORD(1,1);
	int err;
	err=WSAStartup(myVersionRequest,&wsaData);
	if (!err)
	{
		printf("�Ѵ��׽���\n");
	} 
	else
	{
		//��һ�����׽���
		printf("Ƕ����δ��!");
		return;
	}
	SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);//�����˿�ʶ���׽���
	//��Ҫ�󶨵Ĳ���
	SOCKADDR_IN addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip��ַ
	addr.sin_port=htons(6000);//�󶨶˿�
	bind(serSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR));//�����
	listen(serSocket,5);//���еڶ������������ܹ����յ�����������
	//////////////////////////////////////////////////////////////////////////
	//��ʼ���м���
	//////////////////////////////////////////////////////////////////////////
	SOCKADDR_IN clientsocket;
	int len=sizeof(SOCKADDR);
	SOCKET serConn;
	char sendBuf[50];
	serConn=accept(serSocket,(SOCKADDR*)&clientsocket,&len);//������ﲻ��accept����conection�Ļ������ͻ᲻�ϵļ���
	sprintf(sendBuf,"welcome %s to bejing",inet_ntoa(clientsocket.sin_addr));//�ҶԶ�Ӧ��IP���ҽ������ִ�ӡ������
	//���ͻ�ӭ��Ϣ
	send(serConn,sendBuf,strlen(sendBuf)+1,0);

	FILE *fp;
	lable1: printf("�ͻ����Ѵ�\n���������ļ���ַ��\n");
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
	   printf("�ļ�δ��\n");
	   goto lable1;
	}
	else
	{
	   printf("�ļ��Ѵ� ��ʼ�ļ�����......\n");
	   //���Ϳ�ʼ��־
	   send(serConn,"��ʼ����",strlen("��ʼ����")+1,0); 
	   //�õ��ļ���С
	   char datalength[20];
	   long int length=0;
	   recv(serConn,datalength,21,0);
	   length=atol(datalength);
	   printf("�õ��ļ���С�� %d\n",length);
	   //��ʼ����
	   double cent=0.0;
	   char receiveBuf[SIZE];
	   long int x=0;
	   while (1)
	   {
		x=x+SIZE;
		if(x<length)
		{
		 cent=(double)x*100.0/(double)length;
		 printf("�ѽ��գ� %4.2f��\n",cent);
		 recv(serConn,receiveBuf,SIZE+1,0);
		 fwrite(receiveBuf,1,SIZE,fp);
		}
		else
		{
		 recv(serConn,receiveBuf,length+SIZE-x+1,0);
		 printf("�ļ��������\n");
		 fwrite(receiveBuf,1,length+SIZE-x,fp);
		 fclose(fp);
		 break;
		}
	   }
	}
	closesocket(serConn);//�ر�
	WSACleanup();//�ͷ���Դ�Ĳ���
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