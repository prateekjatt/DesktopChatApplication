#include "Socket.h"


Socket::Socket():sock(INVALID_SOCKET),profileUsername(""),serverIPAddress("127.0.0.1"),serverPort("80") {
	WSADATA wsaData;

	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		OutputDebugStringA(("WSAStartup failed: " + std::to_string(WSAGetLastError()) + "\n").c_str());
	}
}

int Socket::connectToServer() {
	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int res = getaddrinfo(serverIPAddress, serverPort, &hints, &result);

	if (res != 0) {
		OutputDebugStringA(("getaddrinfo failed: "+std::to_string(res)+"\n").c_str());
		return 1;
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		OutputDebugStringA(("socket failed: "+std::to_string(WSAGetLastError())+"\n").c_str());
		freeaddrinfo(result);
		return 1;
	}
	res = connect(sock, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}

	freeaddrinfo(result);
	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("Unable to connect to server!\n");
		return 1;
	}
	else OutputDebugStringA("Connected to server!\n");

	sendMessage(profileUsername, strlen(profileUsername));

	return 0;
}

int Socket::sendMessage(PSTR buf,int len) {

	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("Error: Attempting to send data to INVALID_SOCKET\n");
		return SOCKET_ERROR;
	}

	int res = send(sock, buf, len, 0);
	if (res == SOCKET_ERROR) {
		OutputDebugStringA(("Send failed: "+std::to_string(WSAGetLastError())+"\n").c_str());
	}
	else {
		OutputDebugStringA(("Bytes Sent: "+std::to_string(res)+"\n").c_str());
	}

	return res;
}

int Socket::receiveMessage(PSTR buf,int buflen) {
	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("Error: Attempting to recv data from INVALID_SOCKET\n");
		return SOCKET_ERROR;
	}

	int res = recv(sock, buf, buflen, 0);
	if (res > 0) {
		OutputDebugStringA(("Bytes received: "+std::to_string(res)+"\n").c_str());
	}
	else if (res == 0) OutputDebugStringA("Connection Closed\n");
	else {
		OutputDebugStringA(("recv failed: "+std::to_string(WSAGetLastError())+"\n").c_str());
	}
	return res;
}

int Socket::updateInfo(PSTR username, PSTR ipaddress, PSTR port) {
	bool f = false;
	if (strcmp(username, profileUsername)) {
		strcpy_s(profileUsername, username);
		f = true;
	}
	if (strcmp(ipaddress, serverIPAddress)){
		strcpy_s(serverIPAddress,ipaddress);
		f = true;
	}	
	if (strcmp(port, serverPort)){
		strcpy_s(serverPort,port);
		f = true;
	}	

	return connectToServer();
}

void Socket::disconnect() {

	if (sock != INVALID_SOCKET) {
		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}

	sock = INVALID_SOCKET;
}

Socket::~Socket() {
	WSACleanup();

}