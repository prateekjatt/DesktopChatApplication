#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

#pragma comment(lib,"Ws2_32.lib")

#define MAX_LENGTH 100
#define WELTEXTLEN 300

struct Client {
	SOCKET sock;
	CHAR ipaddress[MAX_LENGTH];
	CHAR username[MAX_LENGTH];
};

class Socket{
private:
	SOCKET sock;
	CHAR ipaddress[MAX_LENGTH];
	CHAR port[MAX_LENGTH];
	CHAR welcomeText[WELTEXTLEN];
	static std::vector<std::pair<Client, std::thread>> clients;
	static BOOL flag;
public:
	Socket();
	void initServer();
	void acceptClients();
	static void recvUpdates(Client);
	static void sendUpdates(Client,PSTR,int);
	~Socket();

};

