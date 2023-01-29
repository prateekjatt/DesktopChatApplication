#pragma once

#include "framework.h"
#include "MessageDisplayWindow.h"
#include "MessageSendWindow.h"
#define MAX_STRING 100

class MessageWindow{
private:
	HWND hwnd;
	double windowScale;
	HINSTANCE hInst;
	WCHAR szWindowClass[MAX_STRING];
	MessageDisplayWindow messageDisplay;
	MessageSendWindow messageSend;
	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);
public:
	MessageWindow() :hwnd(NULL), windowScale(0.9f), hInst(NULL), szWindowClass(L"MESSAGEWINDOW") {}
	BOOL InitInstance(HINSTANCE,HWND);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL resize(HWND);
	void updateText(LPSTR, int);
	PWCHAR getWindowClass() { return szWindowClass; }


};

