#include "Socket.h"

std::vector<Client> Socket::clients = {};
std::vector<std::thread> Socket::clientThread = {};
BOOL Socket::flag = TRUE;

Socket::Socket():sock(INVALID_SOCKET),port("80"),ipaddress("127.0.0.1"),welcomeText("\r\n========================================================================\r\n"
	"Welcome To Desktop Chat Application.\r\nThis is a basic chatting application developed using Win32API/C++ for Windows OS\r\n"
	"========================================================================\r\n") {
	WSADATA wsaData;

	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		std::cout << "WSAStartup failed: " << res << '\n';
	}
}

void Socket::initServer() {
	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int res = getaddrinfo(NULL, port, &hints, &result);
	if (res != 0) {
		std::cout << "getaddrinfo failed: " << res << '\n';
		WSACleanup();
		return;
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError()<<"\n";
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	res = bind(sock, result->ai_addr, result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		std::cout << "Bind Failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed with error: " << WSAGetLastError() << '\n';
		closesocket(sock);
		WSACleanup();
		return;
	}

	inet_pton(AF_INET, ipaddress, &((struct sockaddr_in*)result->ai_addr)->sin_addr);

	std::cout << "Server Listening: " << ipaddress << ":" << ntohs(((struct sockaddr_in*)result->ai_addr)->sin_port) << '\n';


}

void Socket::acceptClients() {

	struct sockaddr_in sa = {0};
	socklen_t sl = sizeof(sa);
	do {
		SOCKET client = accept(sock,(struct sockaddr*) & sa, &sl);
		if (client == INVALID_SOCKET) {
			std::cout << "Accept failed: " << WSAGetLastError() << '\n';
			closesocket(sock);
			WSACleanup();
			return;
		}
		Client cl;
		cl.sock = client;
		inet_ntop(AF_INET,&(sa.sin_addr),cl.ipaddress,MAX_LENGTH);
		clients.push_back(cl);
		std::cout << "New Client Connected: " << cl.ipaddress << '\n';
		send(cl.sock, welcomeText, WELTEXTLEN, 0);

		clientThread.push_back(std::thread(recvUpdates,cl));
	} while (flag);

}

void Socket::recvUpdates(Client client) {		
	int len = 1024;
	PCHAR buf = new CHAR[len];
	PCHAR sendbuf = new CHAR[2*len];
	do {
		int res = recv(client.sock, buf, len, 0);
		if (res > 0) {
			if (res < len) buf[res] = '\0';
			std::cout << "Bytes received from " << client.ipaddress << ": " << res << '\n';
			std::cout << "Buffer: " << buf << '\n';

			for (const auto& cl : Socket::clients) {
				if (strcmp(cl.ipaddress, client.ipaddress) == 0) {
					strcpy_s(sendbuf, 2 * len, "You: \0");
				}
				else {
					strcpy_s(sendbuf, 2 * len, client.ipaddress + '\0');
				}

				res += (strlen(sendbuf) + 1);
				strcat_s(sendbuf, res, buf);
				res = send(cl.sock, sendbuf, res, 0);
			}
		}
		else if (res == 0) break;
		else std::cout << "Error: " << WSAGetLastError() << "\n";
	} while (flag);
}


Socket::~Socket() {
	flag = FALSE;
	for (auto &th : clientThread) {
		th.join();
	}
	closesocket(sock);
	WSACleanup();
}
