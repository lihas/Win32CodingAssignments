//Headers
#include <windows.h>
#include <exception>
#include<iostream>
#include <string>

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

    long long i = 0;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        OutputDebugString("in msg loop ");
        OutputDebugString(to_string(msg.message).c_str());
        OutputDebugString(" ");
        OutputDebugString(to_string(i++).c_str());
        OutputDebugString("\n");

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //code
    string msgs;

    switch (iMsg)
    {
    case WM_CREATE:
    {
        msgs = "WM_CREATE";
    }
    break;
    case WM_LBUTTONDOWN:
    {
        msgs = "WM_LBUTTONDOWN";
    }
    break;
    case WM_MOVE:
    {
        msgs = "WM_MOVE";
    }
    break;
    case WM_KEYDOWN:
    {
        msgs = "WM_KEYDOWN";
    }
    break;
    case WM_SIZE:
    {
        msgs = "WM_SIZE";
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    if (!msgs.empty())
    {
        MessageBoxA(NULL, msgs.c_str(), "Message Received is", MB_OK);
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
