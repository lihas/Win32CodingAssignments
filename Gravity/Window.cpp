#include <Windows.h>
#include "Window.h"


LRESULT WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE hInstance, int iCmdShow):m_className{TEXT("Gravity")}, m_windowName{TEXT("Gravity")}
{
    WNDCLASSEX wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.lpszClassName = m_className;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassEx(&wndclass);

    HWND hWnd = CreateWindow(m_className, m_windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
}


Window::~Window()
{
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}


int Window::MessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
