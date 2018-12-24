#include<Windows.h>
#include<stdio.h>
#include "..\AutomationServerWithRegFile\AutomationServer.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variables
IMyMath *pIMyMath = NULL;

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //variable declaration
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("ComClient");
    HRESULT hr;

    //COM Initialization
    hr = CoInitialize(NULL);

    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library cannot be initialized.\nProgram will now exit"), TEXT("Program error"), MB_OK);
        exit(0);
    }

    //WNDCLASSEX initialization
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

    //Register window class
    RegisterClassEx(&wndclass);
    
    //create window
    hwnd = CreateWindow(AppName, TEXT("Client of COM DLL Server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //COM Uninitialization
    CoUninitialize();
    return (int)msg.wParam;
}


//Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declaration
    void ComErrorDescriptionString(HWND, HRESULT);
    void SafeInterfaceRelease(void);

    //variable declarations
    HRESULT hr;
    int iNum1, iNum2, iSum, iSubtract;
    TCHAR str[255];

    switch (iMsg)
    {
    case WM_CREATE:
    {
        hr = CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER, IID_IMyMath, (void**)&pIMyMath);
        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            DestroyWindow(hwnd);
        }

        //initialize arguments
        iNum1 = 155;
        iNum2 = 145;

        //Call SumOfTwoIntegers
        pIMyMath->SumOfTwoIntegers(iNum1, iNum2, &iSum);
        wsprintf(str, TEXT("Sum of %d and %d is %d"), iNum1, iNum2, iSum);
        MessageBox(hwnd, str, TEXT("SumOftwoIntegers"), MB_OK);

        //Cal SubtractionOfTwoIntegers
        pIMyMath->SubtractionOfTwoIntegers(iNum1, iNum2, &iSubtract);
        wsprintf(str, TEXT("Subtraction of %d and %d is %d"), iNum1, iNum2, iSubtract);
        MessageBox(hwnd, str, TEXT("SubtractionOftwoIntegers"), MB_OK);

        //release
        pIMyMath->Release();
        pIMyMath = NULL;
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

void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
    //variable declarations
    TCHAR* szErrorMessage = NULL;
    TCHAR str[255];

    if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
    {
        hr = HRESULT_CODE(hr);
    }

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrorMessage, 0, NULL) != 0)
    {
        wsprintf(str, TEXT("%s"), szErrorMessage);
        LocalFree(szErrorMessage);
    }
    else
    {
        wsprintf(str, TEXT("[Could not find a description for error # %#x.]\n",), hr);
    }
    MessageBox(hwnd, str, TEXT("COM Error"), MB_OK);
}

void SafeInterfaceRelease()
{
    if (pIMyMath)
    {
        pIMyMath->Release();
        pIMyMath = NULL;
    }
}
