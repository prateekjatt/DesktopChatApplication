// DesktopChatApplication.cpp : Defines the entry point for the application.
//


#include "DesktopChatApplication.h"
#include "MainWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MainWindow mainWindow;

    LoadStringW(hInstance, IDS_APP_TITLE, mainWindow.getTitle(), MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DESKTOPCHATAPPLICATION, mainWindow.getWindowClass(), MAX_LOADSTRING);
    mainWindow.MyRegisterClass(hInstance);

    if (!mainWindow.InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESKTOPCHATAPPLICATION));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


