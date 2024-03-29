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
	double windowWidthScale;
	double windowHeightScale;

public:
	MessageSendTextWindow() :hwnd(NULL), windowHeightScale(0.1f), windowWidthScale(0.9f) {}
	BOOL InitInstance(HINSTANCE, HWND);
	int getText(LPSTR,int);
	int setText(LPSTR);
	int getTextLength();
	BOOL resize(HWND);
};

