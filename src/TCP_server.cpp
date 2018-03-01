#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	//init
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//creat socket
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//set up IP and port
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8080);//the port number
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//start listening
	if (listen(slisten, 5) == SOCKET_ERROR)//the maximum is 128
	{
		printf("listen error !");
		return 0;
	}

	//init
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	printf("Waiting for connection\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);//accepts the socket connection for parameter slisten
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		/*continue;*/
	}
	char sendBuf[20] = { '\0' };
	printf("Accept a connection£º%s \r\n", inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16));
	while (true)
	{
		//receive data
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//send data
		char * sendData = "[data]\n";
		send(sClient, sendData, strlen(sendData), 0);
		//closesocket(sClient);//close
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}