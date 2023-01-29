#include "MessageWindow.h"


ATOM MessageWindow::MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MessageWindow::WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = CreateSolidBrush(0xDDDDDD);

	return RegisterClassExW(&wcex);
}

BOOL MessageWindow::InitInstance(HINSTANCE hInstance,HWND hWnd) {
	hInst = hInstance;

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowW(
		szWindowClass,
		NULL,
		WS_CHILD,
		(int)((parentWidth * (1.0 - windowScale)) / 2.0), (int)((parentHeight * (1.0 - windowScale)) / 2.0),
		(int)(windowScale * parentWidth), (int)(windowScale * parentHeight),
		hWnd,
		(HMENU)(int)(1),
		hInst,
		this);

	if (!hwnd) {
		return FALSE;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}

LRESULT CALLBACK MessageWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	MessageWindow* ptr;

	if (message == WM_NCCREATE) {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		ptr = (MessageWindow*)cs->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptr);

	}
	else ptr = (MessageWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (ptr) return ptr->HandleMessage(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
} 

LRESULT MessageWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		messageDisplay.InitInstance(hInst, hWnd);
		messageSend.MyRegisterClass(hInst);
		messageSend.InitInstance(hInst, hWnd);
		return 0;
		
	case WM_SIZE:
		messageDisplay.resize(hWnd);
		messageSend.resize(hWnd);
		return 0;
	case PRIVATE_WM_SEND:
		SendMessage(GetParent(hwnd), PRIVATE_WM_SEND, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void MessageWindow::updateText(LPSTR str, int length) {
	messageDisplay.updateText(str, length);
}

BOOL MessageWindow::resize(HWND hWnd) {
	
	RECT rect;
	GetClientRect(hWnd,&rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		(int)( (parentWidth* (1.0 - windowScale))/2.0), (int)((parentHeight*(1.0-windowScale))/2.0),
		(int)(windowScale*parentWidth), (int)(windowScale*parentHeight),
		TRUE);
	
	ShowWindow(hWnd, SW_SHOW);
	return f;

}
