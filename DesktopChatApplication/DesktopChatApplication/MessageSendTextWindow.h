#pragma once

#include "framework.h"
#include<CommCtrl.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

class MessageSendTextWindow{
private:
	HWND hwnd;
	HINSTANCE hInst;
	double windowScale;
	//LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

public:
	MessageSendTextWindow() :hwnd(NULL), hInst(NULL),windowScale(0.9f){}
	BOOL InitInstance(HINSTANCE, HWND);
	//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	int getText(LPSTR,int);
	int getTextLength();
	BOOL resize(HWND);
};

