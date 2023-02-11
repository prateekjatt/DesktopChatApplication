#include "Socket.h"


Socket::Socket():sock(INVALID_SOCKET),profileUsername(""),serverIPAddress("127.0.0.1"),serverPort("80") {
	WSADATA wsaData;

	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		OutputDebugStringA("WSAStartup failed: ");
		OutputDebugStringA((LPSTR)std::to_string(WSAGetLastError()).c_str());
		OutputDebugStringA("\n");
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
		OutputDebugStringA("getaddrinfo failed: ");
		OutputDebugStringA((LPSTR)std::to_string(res).c_str());
		OutputDebugStringA("\n");
		return 1;
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("socket failed: ");
		OutputDebugStringA((LPSTR)std::to_string(WSAGetLastError()).c_str());
		OutputDebugStringA("\n");
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

void Socket::sendMessage(PSTR buf,int len) {

	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("Error: Attempting to send data to INVALID_SOCKET");
		return;
	}

	int res = send(sock, buf, len, 0);
	if (res == SOCKET_ERROR) {
		OutputDebugStringA("Send failed: ");
		OutputDebugStringA((LPSTR)std::to_string(WSAGetLastError()).c_str());
		OutputDebugStringA("\n");
		closesocket(sock);
		return;
	}

	OutputDebugStringA("Bytes Sent: ");
	OutputDebugStringA((LPSTR)std::to_string(res).c_str());
	OutputDebugStringA("\n");
}

int Socket::receiveMessage(PSTR buf,int buflen) {
	if (sock == INVALID_SOCKET) {
		OutputDebugStringA("Error: Attempting to send data to INVALID_SOCKET");
		return 0;
	}

	int res = recv(sock, buf, buflen, 0);
	if (res > 0) {
		OutputDebugStringA("Bytes received: ");
		OutputDebugStringA((LPSTR)std::to_string(res).c_str());
		OutputDebugStringA("\n");
	}
	else if (res == 0) OutputDebugStringA("Connection Closed\n");
	else {
		OutputDebugStringA("recv failed: ");
		OutputDebugStringA((LPSTR)std::to_string(WSAGetLastError()).c_str());
		OutputDebugStringA("\n");
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
	shutdown(sock, SD_BOTH);
	closesocket(sock);
}

Socket::~Socket() {
	WSACleanup();

}