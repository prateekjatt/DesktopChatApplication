#pragma once

#include "framework.h"
#include "MessageSendButtonWindow.h"
#include "MessageSendTextWindow.h"
#include<CommCtrl.h>
#include "resource.h"
#define MAX_STRING 100

class MessageSendWindow{
private:
	HWND hwnd;
	HINSTANCE hInst;
	MessageSendTextWindow messageText;
	MessageSendButtonWindow messageButton;
	double windowScale;
	WCHAR szWindowClass[MAX_STRING];
	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

public:
	MessageSendWindow() :hwnd(NULL), hInst(NULL), szWindowClass(L"MESSAGESENDWINDOW"),windowScale(0.1f) {}
	BOOL InitInstance(HINSTANCE, HWND);
	ATOM MyRegisterClass(HINSTANCE);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL resize(HWND);
	PWCHAR getWindowClass() { return szWindowClass; }
};

