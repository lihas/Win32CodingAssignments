#include <Windows.h>
#include "Window.h"


LRESULT WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

Window::Window()
{
    WNDCLASSEX wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.lpszClassName = m_className;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassEx(&wndclass);

    //HWND hWnd = CreateWindow(m_className)
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
