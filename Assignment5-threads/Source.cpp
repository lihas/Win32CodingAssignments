#define UNICODE
#include<Windows.h>
#include<iostream>
#include<string>
using namespace std;

constexpr int MAX_INT = (unsigned int)(-1) >> 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE hThread1, hThread2;

DWORD WINAPI thread1Function(LPVOID lpParameter)
{
    static int i = MAX_INT;

    HWND hWnd = *(HWND*)lpParameter;
    RECT wndRect;
    WCHAR str[MAX_PATH];

    auto hDc = GetDC(hWnd);
    while (i>=0)
    {
        wsprintfW(str, L"%020d\0", i);
        GetClientRect(hWnd, &wndRect);
        wndRect.bottom *= 0.5;
        DrawTextW(hDc, str, -1, &wndRect, DT_LEFT| DT_VCENTER | DT_SINGLELINE);
        i--;
    }
    ReleaseDC(hWnd, hDc);
    
    return 0;
}
DWORD WINAPI  thread2Function(LPVOID lpParameter)
{
    static int i = 0;

    HWND hWnd = *(HWND*)lpParameter;
    WCHAR str[MAX_PATH];

    RECT wndRect;
    auto hDc = GetDC(hWnd);
    while (i<= MAX_INT)
    {
        wsprintfW(str, L"%020d\0", i);
        GetClientRect(hWnd, &wndRect);
        wndRect.top += wndRect.bottom * 0.5;
        DrawTextW(hDc, str, -1, &wndRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        i++;
    }
    ReleaseDC(hWnd, hDc);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    static HWND hWindow;
    TCHAR className[] = TEXT("multiThreading");
    WNDCLASSEX wndclass;
    memset(&wndclass, 0, sizeof(wndclass));

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = className;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassExW(&wndclass);

    DWORD thread1Id, thread2Id;

    hWindow = CreateWindow(className, TEXT("Multi Threading Demo"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    hThread1 = CreateThread(nullptr, 0, thread1Function, &hWindow, CREATE_SUSPENDED, &thread1Id);
    hThread2 = CreateThread(nullptr, 0, thread2Function, &hWindow, CREATE_SUSPENDED, &thread2Id);
    
    ShowWindow(hWindow, iCmdShow);
    UpdateWindow(hWindow);
    
    //message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_CREATE:
    {
        ResumeThread(hThread1);
        ResumeThread(hThread2);
    }
        break;
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProcW(hWnd, iMsg, wParam, lParam);
}