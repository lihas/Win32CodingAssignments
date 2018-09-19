#define UNICODE
#include<Windows.h>
#include "..\ClassFactoryDllServerWithRegFile\ClassFactoryDllServerWithRegFile.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass = {};
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("ComClient");
    HRESULT hr;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library cannot be initialized.\n Program will now exit"), TEXT("Program Error"), MB_OK);
        exit(0);
    }

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.hInstance = hInstance;
    wndclass.lpszClassName = AppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    //register
    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(AppName, TEXT("Client of COM Dll Server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();

    return (int)msg.wParam;
}

//window proc
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void SafeInterfaceRelease();

    //variable declarations
    HRESULT hr;
    int iNum1, iNum2, iSum;
    TCHAR str[255];

    switch (iMsg) {
    case WM_CREATE:
    {
        hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);

        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("ISum interface cannot be obtained"), TEXT("ERROR"), MB_OK);
            DestroyWindow(hwnd);
        }
        iNum1 = 55;
        iNum2 = 45;

        //call SumOfTwoIntegers
        pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);

        //display result
        wsprintf(str, TEXT("Sum of %d and %d = %d"), iNum1, iNum2, iSum);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        //call query =interface on ISum to get ISubtract
        hr = pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);

        if (FAILED(hr))
        {
            MessageBoxA(hwnd, "ISubtract interface cannot be obtained", "Error", MB_OK);
            DestroyWindow(hwnd);
        }

        //ISum not needed anymore, release it
        pISum->Release();
        pISum = NULL;

        iNum1 = 155;
        iNum2 = 145;

        pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iSum);

        //ISubtract no longer needed, release it
        pISubtract->Release();
        pISubtract = NULL;

        wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iSum);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        //exit application
        DestroyWindow(hwnd);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void SafeInterfaceRelelease()
{
    if (pISum)
    {
        pISum->Release();
        pISum = NULL;
    }

    if (pISubtract)
    {
        pISubtract->Release();
        pISubtract = NULL;
    }
}