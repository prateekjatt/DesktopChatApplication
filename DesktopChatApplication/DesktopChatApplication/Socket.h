#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
#define MAX_LOADSTRING 100
#pragma comment(lib,"Ws2_32.lib")


class Socket{
private:
	SOCKET sock;
	CHAR profileUsername[MAX_LOADSTRING];
	CHAR serverIPAddress[MAX_LOADSTRING];
	CHAR serverPort[MAX_LOADSTRING];


public:
	Socket();
	int connectToServer();
	void sendMessage(PSTR,int);
	int receiveMessage(PSTR,int);
	void updateInfo(PSTR, PSTR, PSTR);
	void disconnect();
};

