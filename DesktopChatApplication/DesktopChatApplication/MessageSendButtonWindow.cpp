#include "MessageSendButtonWindow.h"


BOOL MessageSendButtonWindow::InitInstance(HINSTANCE hInstance, HWND hWnd) {
	hInst = hInstance;

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowW(
		L"BUTTON",
		L"Send",
		WS_CHILD,
		rect.right - (parentWidth * windowScale), 0,
		parentWidth * windowScale, parentHeight,
		hWnd,
		(HMENU)ID_SENDBTN,
		hInst,
		this);

	if (!hwnd) {
		return FALSE;
	}


	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}


//LRESULT CALLBACK MessageSendButtonWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//
//	MessageSendButtonWindow* ptr;
//
//	if (message == WM_NCCREATE) {
//		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
//		ptr = (MessageSendButtonWindow*)cs->lpCreateParams;
//
//		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptr);
//
//	}
//	else ptr = (MessageSendButtonWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
//
//	if (ptr) return ptr->HandleMessage(hWnd, message, wParam, lParam);
//
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//LRESULT MessageSendButtonWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//	return DefWindowProc(hWnd, message, wParam, lParam);
//
//}

BOOL MessageSendButtonWindow::resize(HWND hWnd) {

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		rect.right - (parentWidth*windowScale), 0,
		parentWidth*windowScale, parentHeight,
		TRUE);

	ShowWindow(hWnd, SW_SHOW);
	return f;
}