#include<Windows.h>
#include"Window.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    Window wnd(hInstance,iCmdShow);
    wnd.MessageLoop();
    return 0;
}