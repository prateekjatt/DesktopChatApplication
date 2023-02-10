#pragma once

#include "framework.h"
#define MAX_STRING 100

class MessageDisplayWindow{
private:
	HWND hwnd;
	HINSTANCE hInst;
	double windowScale;
	WCHAR szWindowClass[MAX_STRING];

public:
	MessageDisplayWindow():hwnd(NULL),hInst(NULL),szWindowClass(L"MESSAGEDISPLAYWINDOW"),windowScale(0.9f){}
	BOOL InitInstance(HINSTANCE, HWND);
	BOOL resize(HWND);
	BOOL updateText(LPSTR,int);
	PWCHAR getWindowClass() { return szWindowClass; }

};

