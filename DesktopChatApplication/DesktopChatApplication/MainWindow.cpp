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

    DialogBox(hInst, MAKEINTRESOURCE(IDD_PROFILEBOX), hwnd, &Profile);
    DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVERBOX), hwnd, &Server);
    recvThread = std::thread(connectToServer, this);

    return TRUE;
}



LRESULT MainWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message)
    {
    case WM_CREATE:
        messageDisplay.InitInstance(hInst, hWnd);
        messageSend.InitInstance(hInst, hWnd);
        messageText.InitInstance(hInst, hWnd);
        
        break;
    case WM_COMMAND: {

        if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_SENDBTN) {
            if (sendThread.joinable()) sendThread.join();
            sendThread = std::thread(sendMessage,this);
            break;
        }

        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, &About);
            break;
        case IDM_PROFILE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_PROFILEBOX), hWnd, &Profile);
            disconnectToServer(this);
            recvThread = std::thread(connectToServer, this);
            break;
        case IDM_SERVER:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVERBOX), hWnd, &Server);
            disconnectToServer(this);
            recvThread = std::thread(connectToServer, this);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }

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
        messageText.resize(hWnd);
        messageSend.resize(hWnd);
        messageDisplay.resize(hWnd);
        break;
    case WM_DESTROY:
        disconnectToServer(this);
        PostQuitMessage(0);
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
                if (!GetDlgItemTextA(hDlg, IDC_PORT, serverPort, MAX_LOADSTRING)) {
                    MessageBox(hDlg, L"Could Not Get Port!", L"Error", MB_OK);
                }
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void MainWindow::sendMessage(MainWindow* ptr) {
    int length = ptr->messageText.getTextLength() + 1;
    LPSTR str = new CHAR[length];
    length = ptr->messageText.getText(str, length);

    if (length > 0) {
        // Use Socket To send message to server.
        int res = ptr->socket.sendMessage(str, length);

        if (res == SOCKET_ERROR) {
            disconnectToServer(ptr);
            ptr->messageDisplay.updateText((LPSTR)"Server Disconnected\r\n", 23);
        }
    }
    ptr->messageText.setText((LPSTR)"");
}

void MainWindow::receiveMessage(MainWindow* ptr) {

    flag = TRUE;
    int res = 0;
    const int MAX_LEN = 1024;
    CHAR buf[MAX_LEN];
    do {
        res = ptr->socket.receiveMessage(buf,MAX_LEN);
        if (res > 0) {
            strcat_s(buf, MAX_LEN, "\r\n");
            ptr->messageDisplay.updateText(buf, res);
        }
        else if(flag != FALSE){
            flag = FALSE;
            ptr->messageDisplay.updateText((LPSTR)"Server Disconnected\r\n", 23);

        }
    } while (flag);



}

void MainWindow::connectToServer(MainWindow* ptr) {
    
    WSADATA wsaData;

    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        OutputDebugStringA(("WSAStartup failed: "+std::to_string(WSAGetLastError())+"\n").c_str());
    }

    int len = 25 + strlen(serverIPAddress) + strlen(serverPort) + 1;
    LPSTR str = new CHAR[len];
    strcpy_s(str, len, "Connecting to Server[");
    strcat_s(str, len, serverIPAddress);
    strcat_s(str, len, ":");
    strcat_s(str, len, serverPort);
    strcat_s(str, len, "]\r\n");
    ptr->messageDisplay.updateText(str, strlen(str));
    if ((ptr->socket.updateInfo(profileUsername, serverIPAddress, serverPort)) == 0) {
        ptr->messageDisplay.updateText((LPSTR)"Successfully Connected to Server.\r\n",40);
        ptr->receiveMessage(ptr);
    }
    else {
        ptr->messageDisplay.updateText((LPSTR)"Unable to Connect with Server.\r\n",40);
    }

    
    WSACleanup();
}

void MainWindow::disconnectToServer(MainWindow* ptr) {
    flag = FALSE;
    ptr->socket.disconnect();
    
    if(ptr->recvThread.joinable()) ptr->recvThread.join();

}