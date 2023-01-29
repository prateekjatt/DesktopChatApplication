#include "MessageSendTextWindow.h"

BOOL MessageSendTextWindow::InitInstance(HINSTANCE hInstance, HWND hWnd) {
	hInst = hInstance;

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowW(
		L"EDIT",
		NULL,
		WS_CHILD | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL,
		0, 0,
		parentWidth*windowScale, parentHeight,
		hWnd,
		(HMENU)(int)(1),
		hInst,
		this);

	if (!hwnd) {
		return FALSE;
	}

	Edit_SetCueBannerText(hwnd, L"Enter Message");

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


//LRESULT CALLBACK MessageSendTextWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//
//	MessageSendTextWindow* ptr;
//
//	if (message == WM_NCCREATE) {
//		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
//		ptr = (MessageSendTextWindow*)cs->lpCreateParams;
//
//		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptr);
//
//	}
//	else ptr = (MessageSendTextWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
//
//	if (ptr) return ptr->HandleMessage(hWnd, message, wParam, lParam);
//
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//LRESULT MessageSendTextWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//	return DefWindowProc(hWnd, message, wParam, lParam);
//
//}

int MessageSendTextWindow::getText(LPSTR str,int length) {
	return GetWindowTextA(hwnd, str, length);
}

int MessageSendTextWindow::getTextLength() {
	int len = GetWindowTextLengthA(hwnd);
	return len;
}

BOOL MessageSendTextWindow::resize(HWND hWnd) {


	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		0,0,
		parentWidth*windowScale, parentHeight,
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