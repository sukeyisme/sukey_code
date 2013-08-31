#include "stdafx.h"
void main(int argc,char *argv[])
{
	int err;
	WSADATA wsaData={0};
	err = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(!err)
		printf("初始化成功！\n");
	printf("是服务器还是客户端？1:服务器 2:客户端 默认为2\n");
	int a=2;
	scanf_s("%d",&a);
	switch(a)
	{
	case 1:
		Server();
		break;
	case 2:
		Client();
		break;
	default:
		Client();
	}
}