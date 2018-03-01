#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include<WS2tcpip.h>

#pragma  comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	clock_t time0_old = 0, time0 = 0;
	int fps = 0;

	char recData[255];

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(2040);
	inet_pton(AF_INET, "192.168.1.240", (void*)&serAddr.sin_addr.S_un.S_addr);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	while (1)
	{
		time0 = clock();

		if (time0 - time0_old > 0)
		{
			fps = 1000 / (time0 - time0_old);
			cout << "fps = " << fps << endl;
		}
		time0_old = time0;

		char  sendData[9] = { 0xaa ,01 ,11 ,11, 22 ,22, 33 ,33 ,0xee };
		send(sclient, sendData, 9, 0);

		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			//recData[ret] = 0x00;
			printf(recData);
		}

	}


	closesocket(sclient);
	WSACleanup();
	return 0;
}