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
    wndclass.cbWndExtra = sizeof(this);

    RegisterClassEx(&wndclass);

    HWND hWnd = CreateWindow(m_className, m_windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(hWnd, iCmdShow);
}


Window::~Window()
{
}

int Window::paint(HDC hDC)
{
    m_world.draw(hDC);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (iMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        pWindow->paint(hDC);
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
