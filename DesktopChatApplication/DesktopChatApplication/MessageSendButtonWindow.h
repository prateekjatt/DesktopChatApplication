#pragma once

#include "framework.h"
#include<CommCtrl.h>
#include "resource.h"

class MessageSendButtonWindow{
private:
	HWND hwnd;
	HINSTANCE hInst;
	double windowScale;
	//LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

public:
	MessageSendButtonWindow() :hwnd(NULL), hInst(NULL),windowScale(0.1f) {}
	BOOL InitInstance(HINSTANCE, HWND);
	//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL resize(HWND);
};

