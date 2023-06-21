#pragma once

#include "framework.h"
#include<CommCtrl.h>
#include "resource.h"

class MessageSendButtonWindow{
private:
	HWND hwnd;
	double windowWidthScale;
	double windowHeightScale;

public:
	MessageSendButtonWindow() :hwnd(NULL),windowWidthScale(0.1f), windowHeightScale(0.1f) {}
	BOOL InitInstance(HINSTANCE, HWND);
	BOOL resize(HWND);
};

