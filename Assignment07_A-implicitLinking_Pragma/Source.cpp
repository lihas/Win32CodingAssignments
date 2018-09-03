#include <Windows.h>
#include <iostream>
#include <string>
#include "..\DLL_Math\MyMath.h"

#pragma comment(lib,"DLL_Math.lib")

using namespace std;

LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    WNDCLASSEX wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    TCHAR className[] = TEXT("MyWindow");


    wndclass.cbSize = sizeof(wndclass);
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = className;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassEx(&wndclass);

    HWND hWnd = CreateWindow(className, TEXT("Using Math DLL"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
 
    return (int)msg.wParam;
}

LRESULT WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_CHAR:
    {
        string s;
        s.append((char*)&wParam);
        char c = '\0';
        s.append(&c);
        int res = MySquare((int)(wParam - '0'));
        MessageBoxA(NULL, (s+string(" square is ")+to_string(res)).c_str(), "WM_CHAR", MB_OK);

    }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hDc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetWindowRect(hWnd, &rc);
        GetWindowRect(hWnd, &rc);
        DrawText(hDc, "Press nums 1..9 on keyboard...", -1, &rc, DT_CENTER | DT_VCENTER);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}