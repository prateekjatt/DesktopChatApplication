#include "MessageSendWindow.h"


ATOM MessageSendWindow::MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MessageSendWindow::WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = CreateSolidBrush(0x001F8A70);
	
	return RegisterClassExW(&wcex);
}

BOOL MessageSendWindow::InitInstance(HINSTANCE hInstance, HWND hWnd) {
	hInst = hInstance;

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	hwnd = CreateWindowW(
		szWindowClass,
		NULL,
		WS_CHILD | WS_BORDER,
		0, rect.bottom-(parentHeight* windowScale),
		parentWidth,parentHeight* windowScale,
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


LRESULT CALLBACK MessageSendWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	MessageSendWindow* ptr;

	if (message == WM_NCCREATE) {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		ptr = (MessageSendWindow*)cs->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptr);

	}
	else ptr = (MessageSendWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (ptr) return ptr->HandleMessage(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT MessageSendWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		messageText.InitInstance(hInst, hWnd);
		messageButton.InitInstance(hInst, hWnd);
		break;
	case WM_SIZE:
		messageText.resize(hWnd);
		messageButton.resize(hWnd);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_SENDBTN) {
			int length = messageText.getTextLength() + 1;
			LPSTR str = new CHAR[length];
			length = messageText.getText(str, length);

			if (length > 0) {
				SendMessage(GetParent(hwnd), PRIVATE_WM_SEND, length, (LPARAM) str);
			}
		}

		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}

BOOL MessageSendWindow::resize(HWND hWnd) {

	RECT rect;
	GetClientRect(hWnd, &rect);
	int parentWidth = (rect.right - rect.left);
	int parentHeight = (rect.bottom - rect.top);
	BOOL f = MoveWindow(
		hwnd,
		0, rect.bottom - (parentHeight * windowScale),
		parentWidth, parentHeight * windowScale,
		TRUE);

	ShowWindow(hWnd, SW_SHOW);
	return f;
}