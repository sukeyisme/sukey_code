#include "stdafx.h"
#define SERVER_ADDRESS "127.0.0.1"
#define PORT 5150
#define MSGSIZE	1024
void Client()
{
	SOCKET sClient;
	SOCKADDR_IN server;
	char szMessage[1024]={0};
	int ret(0);
	//�����׽���
	sClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET!=sClient)
		printf("�׽��ִ����ɹ���\n");
	//���ӷ�����
	memset(&server,0,sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);
    server.sin_port = htons(PORT);

	if(-1!=connect(sClient, (struct sockaddr *)&server, sizeof(SOCKADDR_IN)))
		printf("�ɹ����ӷ�������\n");

	while (TRUE)
	{
		printf("Send:");
		scanf("%s",szMessage);
		
		// ����
		send(sClient, szMessage, strlen(szMessage), 0);

		// �յ�
		ret = recv(sClient, szMessage, MSGSIZE, 0);
		szMessage[ret] = '\0';

		printf("Received [%d bytes]: '%s'\n", ret, szMessage);
	}

	// ����
	closesocket(sClient);
	WSACleanup();
}