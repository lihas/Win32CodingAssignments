#include <stdio.h>
#include "..\AutomationServerWithRegFile\AutomationServer.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("IDispatchClient");

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

    //register window class
    RegisterClassEx(&wndclass);

    //create window
    hwnd = CreateWindow(AppName, TEXT("Client of server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void ComErrorDescriptionString(HWND, HRESULT);

    //variables
    IDispatch* pIDispatch = NULL;
    HRESULT hr;
    DISPID dispid;

    OLECHAR* szFunctionName1 = SysAllocString(L"SumOfTwoIntegers"); //IMP to use SysAllocString: See https://hownot2code.com/2016/07/12/guileful-bstr-strings/
    OLECHAR* szFunctionName2 = SysAllocString(L"SubtractionOfTwoIntegers"); //IMP to use SysAllocString: See https://hownot2code.com/2016/07/12/guileful-bstr-strings/
    VARIANT vArg[2], vRet;
    DISPPARAMS param = { vArg, 0, 2, NULL };
    int n1, n2;
    TCHAR str[255];

    switch (iMsg)
    {
    case WM_CREATE:
    {
        //initialize com library
        hr = CoInitialize(NULL);

        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(hwnd, TEXT("COM library cannot be initialized"), TEXT("COM Error"), MB_OK);
            DestroyWindow(hwnd);
            exit(0);
        }

        //getISum interface
        hr = CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pIDispatch);

        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(hwnd, TEXT("Component cannot be inialized"), TEXT("COM Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
            DestroyWindow(hwnd);
            exit(0);
        }

        //Common code for both IMyMat->SumOfTwoIntegers(), and IMyMath->SubtractionOfTwoIntegers()
        n1 = 75;
        n2 = 25;

        //as DISPARAMS rgvarg member receives parameters in reverse order
        VariantInit(vArg);
        vArg[0].vt = VT_INT;
        vArg[0].intVal = n2;
        vArg[1].vt = VT_INT;
        vArg[1].intVal = n1;
        param.cArgs = 2;
        param.cNamedArgs = 0;
        param.rgdispidNamedArgs = NULL;
        //reverse order of parameters
        param.rgvarg = vArg;

        //return value
        VariantInit(&vRet);
        hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName1, 1, GetUserDefaultLCID(), &dispid);

        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(NULL, TEXT("Cannot get ID for SumOfTwoIntegers()"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
            pIDispatch->Release();
            DestroyWindow(hwnd);
        }

        hr = pIDispatch->Invoke(dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &param, &vRet, NULL, NULL);

        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(NULL, TEXT("Cannot invoke function"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
            pIDispatch->Release();
            DestroyWindow(hwnd);
        }
        else
        {
            sprintf_s(str, TEXT("Sum of %d and %d is %d"), n1, n2, vRet.lVal);
            MessageBox(hwnd, str, TEXT("SumOfTwoIntegers"), MB_OK);
        }

        //code for IMyMath->SubtractionOfTwoIntegers()
        hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName2, 1, GetUserDefaultLCID(), &dispid);
        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(NULL, TEXT("Cannot get ID for SubtractionOfTwoIntegers()"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
            pIDispatch->Release();
            DestroyWindow(hwnd);
        }

        //Invoke for IMyMath->SubtractionOfTwoIntegers();
        hr = pIDispatch->Invoke(dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &param, &vRet, NULL, NULL);
        if (FAILED(hr))
        {
            ComErrorDescriptionString(hwnd, hr);
            MessageBox(NULL, TEXT("Cannot invoke function"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
            pIDispatch->Release();
            DestroyWindow(hwnd);
        }
        else
        {
            sprintf_s(str, TEXT("Subtraction of %d and %d is %d"), n1, n2, vRet.lVal);
            MessageBox(hwnd, str, TEXT("SubtractionOfTwoIntegers"), MB_OK);
        }

        //clean-up
        VariantClear(vArg);
        VariantClear(&vRet);
        pIDispatch->Release();
        pIDispatch = NULL;
        DestroyWindow(hwnd);

    }
    break;
    case WM_DESTROY:
    {
        CoUninitialize();
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

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&szErrorMessage, 0, NULL) != 0)
    {
        sprintf_s(str, TEXT("%s"), szErrorMessage);
        LocalFree(szErrorMessage);
    }
    else
    {
        sprintf_s(str, TEXT("[Could not find a description for error # %#x.]\n"), hr);
    }
    MessageBox(hwnd, str, TEXT("COM Error"), MB_OK);
}