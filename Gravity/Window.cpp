#include <Windows.h>
#include "Window.h"


LRESULT WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE hInstance, int iCmdShow):m_className{TEXT("Gravity")}, m_windowName{TEXT("Gravity")}
{ 
    m_BackgroundBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);


    WNDCLASSEX wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.lpszClassName = m_className;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hbrBackground = m_BackgroundBrush;
    wndclass.cbWndExtra = sizeof(this);

    RegisterClassEx(&wndclass);

    HWND hWnd = CreateWindow(m_className, m_windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_world.GetWindowWidth(), m_world.GetWindowHeight(), NULL, NULL, hInstance, NULL);

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

int Window::ScaleDown(HDC hDC)
{
    m_world.ScaleDown(hDC);
    return 0;
}

HBRUSH Window::GetBackgroundBrush()
{
    return m_BackgroundBrush;
}

bool timer = false;
#define ID_TIMER 0x55AA
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (iMsg)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case ID_TIMER:
        {
            HDC hDC = GetDC(hWnd);
            RECT rc;
            GetClientRect(hWnd, &rc);
            FillRect(hDC, &rc, pWindow->GetBackgroundBrush());
            pWindow->paint(hDC);
            ReleaseDC(hWnd, hDC);
        }
        break;
        default:
            break;
        }
    case WM_LBUTTONDOWN:
    {
        HDC hDC = GetDC(hWnd);
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hDC, &rc, pWindow->GetBackgroundBrush());
        pWindow->paint(hDC);
        ReleaseDC(hWnd, hDC);
    }
    break;
    case WM_RBUTTONDOWN:
    {
        HDC hDC = GetDC(hWnd);
        pWindow->ScaleDown(hDC);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        //FillRect(hDC, &rc, pWindow->GetBackgroundBrush());
        pWindow->paint(hDC);
        EndPaint(hWnd, &ps);
        if (!timer) {
            SetTimer(hWnd, ID_TIMER, 100, NULL);
            timer = true;
        }
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
