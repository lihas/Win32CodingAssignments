#include<Windows.h>
#include<iostream>
#include<string>
using namespace std;

constexpr int MAX_INT = 100;// (unsigned int)(-1) >> 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE hThread1, hThread2;

DWORD WINAPI thread1Function(LPVOID lpParameter)
{
    static int i = MAX_INT;

    HWND hWnd = *(HWND*)lpParameter;
   // Sleep(5000);


    auto hDc = GetDC(hWnd);
    while (i)
    {
        WCHAR str[MAX_PATH];
        wsprintfW(str, L"%d\0", i);
        RECT rc = { 0,0,500,500 };
        
        RECT wndRect;
        GetWindowRect(hWnd, &wndRect);
        int volatile ret;// = DrawTextW(hDc, str, -1, &wndRect, DT_LEFT);
        ret = TextOut(hDc, 10, 10, "askhsk", 6);
        ret;
        
        i--;

        //RECT wndRect;
        GetWindowRect(hWnd, &wndRect);
        //InvalidateRect(hWnd,&wndRect,TRUE);
       // Sleep(100);
    }
    ReleaseDC(hWnd, hDc);
    
    return 0;
}
DWORD WINAPI  thread2Function(LPVOID lpParameter)
{
    return 0;
    static int i = 0;

    HWND hWnd = *(HWND*)lpParameter;

    while (i<= MAX_INT)
    {
        WCHAR str[MAX_PATH];
        wsprintfW(str, L"%d\0", i);
        RECT rc = { 0,51,50,101 };
        auto hDc = GetDC(hWnd);
        DrawTextW(hDc, str, -1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        ReleaseDC(hWnd, hDc);
        i++;

        RECT wndRect;
        GetWindowRect(hWnd, &wndRect);
        InvalidateRect(hWnd, &wndRect, FALSE);
    }
    
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    HWND hWindow;
    TCHAR className[] = TEXT("multiThreading");
    WNDCLASSEX wndclass;
    /*memset(&wndclass, 0, sizeof(wndclass));

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = className;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

    RegisterClassExW(&wndclass);*/

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wndclass.lpszClassName = className;
    wndclass.lpszMenuName = nullptr;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register above class
    RegisterClassEx(&wndclass);

    DWORD thread1Id, thread2Id;


    hWindow = CreateWindow(className, TEXT("Multi Threading Demo"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    hThread1;// = CreateThread(nullptr, CREATE_SUSPENDED, thread1Function, &hWindow, 0, &thread1Id);
    hThread2;// = CreateThread(nullptr, CREATE_SUSPENDED, thread2Function, &hWindow, 0, &thread2Id);
    
    ShowWindow(hWindow, iCmdShow);
    UpdateWindow(hWindow);
    
    //message Pump
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
    case WM_LBUTTONDOWN:
    {
        /*RECT rc;
        GetWindowRect(hWnd, &rc);
        auto hDc = GetDC(hWnd);
        DrawTextW(hDc, L"sahil", -1, &rc, DT_LEFT);
        ReleaseDC(hWnd, hDc);
*/
        auto hDc = GetDC(hWnd);
        RECT wndRect;
        GetWindowRect(hWnd, &wndRect);
        auto volatile ret = DrawTextW(hDc, L"sssslllll", -1, &wndRect, DT_LEFT);
        ret;
        ReleaseDC(hWnd, hDc);
    }
        break;
    case WM_CREATE:
    {
        auto static h = hWnd;
        CreateThread(nullptr, 0, thread1Function, &h, 0,0 );
        CreateThread(nullptr, 0, thread2Function, &h, 0,0);
/*
        RECT rc;
        GetWindowRect(hWnd, &rc);
        auto hDc = GetDC(hWnd);
        DrawTextW(hDc, L"singh", -1, &rc, DT_LEFT);
        ReleaseDC(hWnd, hDc);*/
    }
        break;
    case WM_SHOWWINDOW:
    {
        RECT rc;
        GetWindowRect(hWnd, &rc);
        auto hDc = GetDC(hWnd);
        DrawTextW(hDc, L"shshsh", -1, &rc, DT_LEFT);
        ReleaseDC(hWnd, hDc);
    }
        break;
    case WM_PAINT:
    {
        ResumeThread(hThread1);
        ResumeThread(hThread2);
        
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProcW(hWnd, iMsg, wParam, lParam);
}