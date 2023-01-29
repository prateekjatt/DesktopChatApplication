#pragma once

#include "resource.h"
#include "framework.h"
#include "MessageWindow.h"
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
	static CHAR profilePassword[MAX_LOADSTRING];
	static CHAR serverIPAddress[MAX_LOADSTRING];
	static CHAR serverPort[MAX_LOADSTRING];

	static BOOL flag;
	std::thread updates;
	
	MessageWindow messageWindow;

public:
	MainWindow():hwnd(NULL),hInst(NULL),messageWindow(),szTitle(L""),szWindowClass(L""),socket(){}
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE, int);
	
	static void receiveMessage(MainWindow*);
	
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK Profile(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK Server(HWND, UINT, WPARAM, LPARAM);
	
	PWCHAR getTitle() { return szTitle; }
	PWCHAR getWindowClass() { return szWindowClass; }
	HINSTANCE getInstance() { return hInst; }
	HWND getHwnd() { return hwnd; }
};

