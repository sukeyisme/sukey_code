#include "stdafx.h"
void main(int argc,char *argv[])
{
	int err;
	WSADATA wsaData={0};
	err = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(!err)
		printf("��ʼ���ɹ���\n");
	printf("�Ƿ��������ǿͻ��ˣ�1:������ 2:�ͻ��� Ĭ��Ϊ2\n");
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