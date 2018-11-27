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

    wc.cbClsExtra = sizeof(wc);
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
}
