#include "MainWindow.h"

CHAR MainWindow::profileUsername[MAX_LOADSTRING] = "";
CHAR MainWindow::serverIPAddress[MAX_LOADSTRING] = "127.0.0.1";
CHAR MainWindow::serverPort[MAX_LOADSTRING] = "80";
BOOL MainWindow::flag = TRUE;

ATOM MainWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPCHATAPPLICATION));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(COLORREF(RGB(29, 78, 7)));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DESKTOPCHATAPPLICATION);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, this);

    if (!hwnd)
    {
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return TRUE;
}

void MainWindow::receiveMessage(MainWindow* ptr) {
    WSADATA wsaData;

    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        OutputDebugStringA("WSAStartup failed: ");
        OutputDebugStringA((LPSTR)std::to_string(WSAGetLastError()).c_str());
        OutputDebugStringA("\n");
    }

    int len = 0;
    const int MAX_LEN = 1024;
    CHAR buf[MAX_LEN];
    do {
        len = ptr->socket.receiveMessage(buf,MAX_LEN);
        if (len > 0) {
            strcat_s(buf, MAX_LEN, "\r\n");
            ptr->messageWindow.updateText(buf, len);
        }
    } while (flag);

    WSACleanup();

}


LRESULT MainWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message)
    {
    case WM_CREATE:
        messageWindow.MyRegisterClass(hInst);
        messageWindow.InitInstance(hInst, hWnd);
        socket.connectToServer();
        updates = std::thread(MainWindow::receiveMessage, (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA));
        
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, &About);
            break;
        case IDM_PROFILE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_PROFILEBOX), hWnd, &Profile);
            socket.updateInfo(profileUsername,serverIPAddress,serverPort);
            break;
        case IDM_SERVER:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVERBOX), hWnd, &Server);
            socket.updateInfo(profileUsername,serverIPAddress,serverPort);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_GETMINMAXINFO: {
        LPMINMAXINFO info = (LPMINMAXINFO)lParam;
        info->ptMinTrackSize = { 600,400 };
        break;
    }
    case WM_SIZE:
        messageWindow.resize(hWnd);
        break;
    case WM_DESTROY:
        flag = FALSE;
        updates.join();
        socket.disconnect();
        PostQuitMessage(0);
        break;
    case PRIVATE_WM_SEND:
        // Use Socket To send message to server.
        socket.sendMessage((PSTR)lParam,wParam);
        

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    MainWindow* ptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        ptr = (MainWindow*)cs->lpCreateParams;

        SetLastError(0);
        if (SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptr) == 0) {
            if (GetLastError() != 0) {
                return FALSE;
            }
        }
    }
    else ptr = (MainWindow*) GetWindowLongPtr(hWnd,GWLP_USERDATA);

    if (ptr) return ptr->HandleMessage(hWnd,message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}


INT_PTR CALLBACK MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainWindow::Profile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            if (LOWORD(wParam) == IDOK) {
                if (!GetDlgItemTextA(hDlg, IDC_USERNAME, profileUsername, MAX_LOADSTRING)) {
                    MessageBox(hDlg, L"Could Not Get Username!", L"Error", MB_OK);
                }
                else {
                    MessageBoxA(hDlg,profileUsername, "Username", MB_OK);
                }
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainWindow::Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
            if (LOWORD(wParam) == IDOK) {
                if (!GetDlgItemTextA(hDlg, IDC_IPADDRESS, serverIPAddress, MAX_LOADSTRING)) {
                    MessageBox(hDlg, L"Could Not Get IP Address!", L"Error", MB_OK);
                }
                else {
                    MessageBoxA(hDlg, serverIPAddress, "IP Address", MB_OK);
                }
                if (!GetDlgItemTextA(hDlg, IDC_PORT, serverPort, MAX_LOADSTRING)) {
                    MessageBox(hDlg, L"Could Not Get Port!", L"Error", MB_OK);
                }
                else {
                    MessageBoxA(hDlg, serverPort, "Port", MB_OK);
                }
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
