#include "MessageDisplayWindow.h"

BOOL MessageDisplayWindow::InitInstance(HINSTANCE hInstance, HWND hWnd) {
	
	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowEx(WS_EX_PALETTEWINDOW,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_MULTILINE| WS_BORDER | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
		0, 0,
		parentWidth, (int)parentHeight * windowScale,
		hWnd,
		(HMENU)(int)(1),
		hInstance,
		this);

	if (!hwnd) {
		return FALSE;
	}

	RECT rc;
	SendMessageW(hwnd, EM_GETRECT, 0, (LPARAM)&rc);
	rc.left = 8;
	rc.top = 4;
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	SendMessageW(hwnd, EM_SETRECT, 0, (LPARAM)&rc);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}

BOOL MessageDisplayWindow::updateText(LPSTR str,int len) {

	int length = GetWindowTextLengthA(hwnd) + len + 2;
	LPSTR buf = new CHAR[length];
	
	GetWindowTextA(hwnd, buf, length);
	strcat_s(buf,length,str);

	bool f = SetWindowTextA(hwnd, buf);
	delete[] buf;
	return f;
}


BOOL MessageDisplayWindow::resize(HWND hWnd) {


	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		0, 0,
		parentWidth,(int) parentHeight * windowScale,
		TRUE);

	RECT rc;
	SendMessageW(hwnd, EM_GETRECT, 0, (LPARAM)&rc);
	rc.left = 8;
	rc.top = 4;
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	SendMessageW(hwnd, EM_SETRECT, 0, (LPARAM)&rc);

	ShowWindow(hWnd, SW_SHOW);
	return f;
}