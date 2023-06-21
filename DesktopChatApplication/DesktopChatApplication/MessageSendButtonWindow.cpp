#include "MessageSendButtonWindow.h"


BOOL MessageSendButtonWindow::InitInstance(HINSTANCE hInstance, HWND hWnd) {
	
	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowW(
		L"BUTTON",
		L"Send",
		WS_CHILD,
		rect.right - (parentWidth * windowWidthScale), rect.bottom - (parentHeight * windowHeightScale),
		parentWidth * windowWidthScale, parentHeight*windowHeightScale,
		hWnd,
		(HMENU)ID_SENDBTN,
		hInstance,
		this);

	if (!hwnd) {
		return FALSE;
	}


	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}

BOOL MessageSendButtonWindow::resize(HWND hWnd) {

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		rect.right - (parentWidth*windowWidthScale), rect.bottom - (parentHeight * windowHeightScale),
		parentWidth*windowWidthScale, parentHeight*windowHeightScale,
		TRUE);

	ShowWindow(hWnd, SW_SHOW);
	return f;
}