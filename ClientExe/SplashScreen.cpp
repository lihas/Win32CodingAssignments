#include "SplashScreen.h"
#include "resource.h"
#include <atltypes.h>
#include <iostream>
#include <string>

constexpr char wndClassName[] = "splashScreen";
constexpr char wndName[] = "splashScreen";
constexpr UINT ID_TIMER = 0x55AA;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

SplashScreen::SplashScreen(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpszClassName = wndClassName;
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hInstance = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (0 == RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "RegisterClassEx()", "Error", MB_OK);
    }
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    m_hwnd = CreateWindow(wndClassName, wndName, WS_POPUP, (width-500)/2, (height-500)/2, 500, 500 , NULL, NULL, hInstance, NULL);
}

void SplashScreen::StartTimer(int ms)
{
    SetTimer(m_hwnd, ID_TIMER, ms, NULL);
}
void SplashScreen::ShowWindow()
{
    ::ShowWindow(m_hwnd, SW_SHOWNORMAL);
    ::UpdateWindow(m_hwnd);
    StartTimer(1000);
}


SplashScreen::~SplashScreen()
{
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance = NULL;
    static HBITMAP hBitmap = NULL;

    switch (iMsg)
    {
    case WM_CREATE:
    {
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDR_poohBitmap));
    }
    break;
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_SPACE:
        {
            DestroyWindow(hwnd);
        }
        break;
        default:
            break;
        }
    }
    break;
    case WM_TIMER:
    {
        switch (wParam)
        {
        case ID_TIMER:
        {
            static int ctr = 10;

            if (ctr-- == 0)
            {
                DestroyWindow(hwnd);
            }
            else
            {
                RECT rc2 = {};
                rc2.top = 0;
                rc2.left = 0;
                rc2.right = 10;
                rc2.bottom = 15;
                auto hdc = GetDC(hwnd);
                DrawText(hdc, std::to_string(ctr).c_str(), -1, &rc2, DT_CENTER | DT_VCENTER);
                ReleaseDC(hwnd, hdc);
            }
        }
        break;
        default:
            break;
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hwnd, &ps);
        CRect rc;
        ::GetClientRect(hwnd, &rc);
        auto cdc = CreateCompatibleDC(hdc);

        auto oldBm = SelectObject(cdc, hBitmap);

        BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), cdc, 0, 0, SRCCOPY);

        SelectObject(cdc, oldBm);
        DeleteDC(cdc);

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        DeleteObject(hBitmap);
        PostQuitMessage(0);
    }
    break;
    default:
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}