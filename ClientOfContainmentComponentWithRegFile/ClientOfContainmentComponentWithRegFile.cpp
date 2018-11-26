#define UNICODE
#include <Windows.h>
#include<process.h>

#include "..\ContainmentOuterComponentWithRegFile\HeaderForClientOfContainmentComponentWithRegFile.h"
//function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//global variables
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;
IMultiplication *pIMultiplication = NULL;
IDivision *pIDivision = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("ComClient");
    HRESULT hr;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library Cannot be initilized.\nProgram will now exit."), TEXT("Error"), MB_OK);
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
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("RegisterClassEx() error"), TEXT("Error"), MB_OK);
        exit(0);
    }

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declaration
    void SafeInterfaceRelease();

    //variable declarations
    HRESULT hr;
    int iNum1, iNum2, iSum, iSubtraction, iMultiplication, iDivision;
    TCHAR str[255];

    switch (iMsg)
    {
    case WM_CREATE:
    {
        hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&pISum);
        if (FAILED(hr))
        {
            MessageBox(NULL, TEXT("ISum interface cannot be obtained"), TEXT("Error"), MB_OK);
            DestroyWindow(hwnd);
            break;
        }

        iNum1 = 65;
        iNum2 = 45;
        pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);
        wsprintf(str, TEXT("Sum of %d, and %d = %d"), iNum1, iNum2, iSum);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);
        hr = pISum->QueryInterface(IID_ISubtract, (void **)&pISubtract);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("ISubtract interface cannot be obtained"), TEXT("Error"), MB_OK);
            DestroyWindow(hwnd);
            break;
        }

        pISum->Release();
        pISum = NULL;
        
        iNum1 = 155;
        iNum2 = 55;

        pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iSubtraction);
        wsprintf(str, TEXT("Subtraction of %d, and %d = %d"), iNum1, iNum2, iSubtraction);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);
        pISubtract->QueryInterface(IID_IMultiplication, (void**)&pIMultiplication);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("IMultiplication interface cannot be obtained"), TEXT("Error"), MB_OK);
            DestroyWindow(hwnd);
            break;
        }

        pISubtract->Release();
        pISubtract = NULL;

        iNum1 = 30;
        iNum2 = 25;
        pIMultiplication->MultiplicationOfTwoIntegers(iNum1, iNum2, &iMultiplication);
        wsprintf(str, TEXT("Multiplication of %d, and %d = %d"), iNum1, iNum2, iMultiplication);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        hr = pIMultiplication->QueryInterface(IID_IDivision, (void**)&pIDivision);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("IDivision interface cannot be obtained"), TEXT("Error"), MB_OK);
            DestroyWindow(hwnd);
            break;
        }

        pIMultiplication->Release();
        pIMultiplication = NULL;

        iNum1 = 200;
        iNum2 = 25;
        pIDivision->DivisionOfTwoIntegers(iNum1, iNum2, &iDivision);

        wsprintf(str, TEXT("Division of %d, and %d = %d"), iNum1, iNum2, iDivision);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        pIDivision->Release();
        pIDivision = NULL;
        DestroyWindow(hwnd);
    }
    break;
    case WM_DESTROY:
    {
        SafeInterfaceRelease();
        PostQuitMessage(0);
    }
    break;
    default:
        break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void SafeInterfaceRelease()
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
    if (pIMultiplication)
    {
        pIMultiplication->Release();
        pIMultiplication = NULL;
    }
    if (pIDivision)
    {
        pIDivision->Release();
        pIDivision = NULL;
    }
}