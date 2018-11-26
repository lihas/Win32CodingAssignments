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
    volatile auto r = PtoV(10, 1, 100);

    dialog::Run(hInstance);

    return (int)msg.wParam;
}