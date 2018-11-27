#include "SplashScreen.h"
#include <iostream>
#include <windows.h>
#include "..\2.PhysicsDll1_implicitLink\Physics.h"
#include "Dialog.h"

//#pragma comment(lib, "2.PhysicsDll1_implicitLink.lib")

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    SplashScreen splashScreen(hInstance);
    splashScreen.ShowWindow();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    auto hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library cannot be initialized.\n Program will now exit"), TEXT("Program Error"), MB_OK);
        exit(0);
    }

    dialog::Run(hInstance);

    CoUninitialize();

    return (int)msg.wParam;
}