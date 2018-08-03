//Headers
#include <windows.h>
#include <exception>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBRUSH hGrDefaultBr;
HBRUSH hRBr;
HBRUSH hBBr;
HBRUSH hGBr;
HBRUSH hCBr;
HBRUSH hMBr;
HBRUSH hYBr;
HBRUSH hWBr;
HBRUSH hKBr;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    hGrDefaultBr = CreateSolidBrush(RGB(128, 128, 128));
    hRBr = CreateSolidBrush(RGB(255, 0, 0));
    hBBr = CreateSolidBrush(RGB(0, 0, 255));
    hGBr = CreateSolidBrush(RGB(0, 255, 0));
    hCBr = CreateSolidBrush(RGB(0, 255, 255));
    hMBr = CreateSolidBrush(RGB(255, 0, 255));
    hYBr = CreateSolidBrush(RGB(255, 255, 0));
    hWBr = CreateSolidBrush(RGB(255, 255, 255));
    hKBr = CreateSolidBrush(RGB(0, 0, 0));

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
    wndclass.hbrBackground = hGrDefaultBr; //(HBRUSH)GetStockObject(WHITE_BRUSH);
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

    DeleteObject(hGrDefaultBr);
    DeleteObject(hRBr);
    DeleteObject(hBBr);
    DeleteObject(hGBr);
    DeleteObject(hCBr);
    DeleteObject(hMBr);
    DeleteObject(hYBr);
    DeleteObject(hWBr);
    DeleteObject(hKBr);

    return (int)msg.wParam;

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //code
    static int keyPressed = -1;
    switch (iMsg)
    {
    case WM_CHAR:
    {
        keyPressed = wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        InvalidateRect(hwnd, &rc, FALSE);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        HBRUSH hbr = hGrDefaultBr;

        switch (keyPressed)
        {
        case 'r':
            hbr = hRBr;
            break;
        case 'b':
            hbr = hBBr;
            break;
        case 'g':
            hbr = hGBr;
            break;
        case 'c':
            hbr = hCBr;
            break;
        case 'm':
            hbr = hMBr;
            break;
        case 'y':
            hbr = hYBr;
            break;
        case 'w':
            hbr = hWBr;
            break;
        case 'k':
            hbr = hKBr;
            break;
        default:
            break;
        }
        FillRect(hdc, &rc, hbr);
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
