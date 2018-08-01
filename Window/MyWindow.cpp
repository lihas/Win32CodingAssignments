//Headers
#include <windows.h>

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
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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

    return (int)msg.wParam; //sahils-debug => what's this?

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //code
    switch (iMsg)
    {
    case WM_KEYDOWN:
        MessageBoxA(hwnd, "", "WM_KEYDOWN", MB_OK);
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'A':
            MessageBoxA(hwnd, "A", "WM_CHAR", MB_OK);
            break;
        case 'a':
            MessageBoxA(hwnd, "a", "WM_CHAR", MB_OK);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
    //return ERROR_SUCCESS;
}
