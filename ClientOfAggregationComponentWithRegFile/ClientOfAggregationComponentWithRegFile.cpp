#include "..\HeaderForClientOfAggregationComponentWithRegFile.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

ISum *pISum = nullptr;
ISubtract* pISubtract = nullptr;
IMultiplication* pIMultiplication = nullptr;
IDivision* pIDivision = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    TCHAR AppName[] = TEXT("ComClient");
    HRESULT hr;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM library can not be initialized.\n Program will now exit"), TEXT("Program Error"), MB_OK);
        exit(0);
    }

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hInstance = hInstance;
    wc.lpszClassName = AppName;
    wc.lpszMenuName = NULL;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
    }

    hwnd = CreateWindow(AppName, TEXT("Client Of COM Dll Server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

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
    void SafeInterfaceRelease();

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
            MessageBox(hwnd, TEXT("ISum interface cannot be obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }

        iNum1 = 65;
        iNum2 = 45;

        pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);
        wsprintf(str, TEXT("Sum of %d and %d = %d"), iNum1, iNum2, iSum);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        //Get ISubtract
        pISum->QueryInterface(IID_ISubtract, (void**)&pISubtract);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("ISubtract interface cannot be obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }

        pISum->Release();
        pISum = nullptr;

        iNum1 = 155;
        iNum2 = 55;
        pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iSubtraction);
        wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iSubtraction);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        hr = pISubtract->QueryInterface(IID_IMultiplication, (void**)&pIMultiplication);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("IMultiplication interface cannot be obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }

        pISubtract->Release();
        pISubtract = nullptr;

        iNum1 = 30;
        iNum2 = 25;

        pIMultiplication->MultiplicationOfTwoIntegers(iNum1, iNum2, &iMultiplication);
        wsprintf(str, TEXT("Multiplication of %d and %d = %d"), iNum1, iNum2, iMultiplication);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);

        hr = pIMultiplication->QueryInterface(IID_IDivision, (void**)&pIDivision);
        if (FAILED(hr))
        {
            MessageBox(hwnd, TEXT("IDivision interface cannot be obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }

        pIMultiplication->Release();
        pIMultiplication = nullptr;

        iNum1 = 200;
        iNum2 = 25;
        pIDivision->DivisionOfTwoIntegers(iNum1, iNum2, &iDivision);
        wsprintf(str, TEXT("Division of %d and %d = %d"), iNum1, iNum2, iDivision);
        MessageBox(hwnd, str, TEXT("Result"), MB_OK);
        pIDivision->Release();
        pIDivision = nullptr;
        DestroyWindow(hwnd);
    }
    break;
    case WM_DESTROY:
        SafeInterfaceRelease();
        PostQuitMessage(0);
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
        pISubtract = nullptr;
    }
    if (pIMultiplication)
    {
        pIMultiplication->Release();
        pIMultiplication = nullptr;
    }
    if (pIDivision)
    {
        pIDivision->Release();
        pIDivision = nullptr;
    }
}