#pragma once

#include "resource.h"
#include "framework.h"
#include "MessageSendTextWindow.h"
#include "MessageSendButtonWindow.h"
#include "MessageDisplayWindow.h"
#include "Socket.h"
#define MAX_LOADSTRING 100

#include<thread>
class MainWindow {
private:
	HWND hwnd;
	HINSTANCE hInst;
	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];
	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);
	
	Socket socket;
	static CHAR profileUsername[MAX_LOADSTRING];
	static CHAR serverIPAddress[MAX_LOADSTRING];
	static CHAR serverPort[MAX_LOADSTRING];

	static BOOL flag;
	std::thread recvThread;
	std::thread sendThread;
	
	MessageDisplayWindow messageDisplay;
	MessageSendTextWindow messageText;
	MessageSendButtonWindow messageSend;

public:
	MainWindow():hwnd(NULL),hInst(NULL),messageDisplay(), messageText(),messageSend(), szTitle(L"DesktopChatApplication"), szWindowClass(L"DESKTOPCHATAPPLICATION"), socket() {}
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE, int);
	
	static void sendMessage(MainWindow*);
	static void receiveMessage(MainWindow*);
	static void connectToServer(MainWindow*);
	static void disconnectToServer(MainWindow*);

	
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK Profile(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK Server(HWND, UINT, WPARAM, LPARAM);
	
	PWCHAR getTitle() { return szTitle; }
	PWCHAR getWindowClass() { return szWindowClass; }
	HINSTANCE getInstance() { return hInst; }
	HWND getHwnd() { return hwnd; }
};

