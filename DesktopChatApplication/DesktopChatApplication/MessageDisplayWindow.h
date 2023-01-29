#pragma once

#include "framework.h"
#define MAX_STRING 100

class MessageDisplayWindow{
private:
	HWND hwnd;
	HINSTANCE hInst;
	double windowScale;
	WCHAR szWindowClass[MAX_STRING];
	//LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

public:
	MessageDisplayWindow():hwnd(NULL),hInst(NULL),szWindowClass(L"MESSAGEDISPLAYWINDOW"),windowScale(0.9f){}
	BOOL InitInstance(HINSTANCE, HWND);
	//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL resize(HWND);
	BOOL updateText(LPSTR,int);
	PWCHAR getWindowClass() { return szWindowClass; }

};

