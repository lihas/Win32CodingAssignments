//Headers
#include "stdafx.h"
#include <windows.h>
#include <exception>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{

    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");

    //code
    //Initialization of WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = nullptr;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register above class
    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(szAppName, TEXT("My Application"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //code
    switch (iMsg)
    {
    case WM_LBUTTONDOWN:
    {
        break;
        auto hdc = GetDC(hwnd);
        RECT rc;
        auto err = GetClientRect(hwnd, &rc);
        if (!err)
        {
            throw std::exception{ "GetClientRect error" };
        }
        auto hbr = CreateSolidBrush(RGB(255, 0, 0));
        auto oldBr = SelectObject(hdc, hbr);
        auto hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
        auto hOldPen = SelectObject(hdc, hPen);
        
        SetTextColor(hdc, RGB(0, 255, 0));

        DrawTextA(hdc, "LBUTTON DOWN", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

        MoveToEx(hdc, 0, 0, nullptr);
        LineTo(hdc, rc.right, rc.bottom);
        Rectangle(hdc, rc.left + 100, rc.top + 100, rc.left + 200, rc.top + 200);
        

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, oldBr);
        DeleteObject(hbr);
        DeleteObject(hOldPen);
        ReleaseDC(hwnd, hdc);

    }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hwnd, &ps);
        RECT rc = {};
        auto err = GetClientRect(hwnd, &rc);
        if (!err)
        {
            throw std::exception{ "GetClientRect error" };
        }
        DrawTextA(hdc, "Hello World!", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
    //return ERROR_SUCCESS;
}
