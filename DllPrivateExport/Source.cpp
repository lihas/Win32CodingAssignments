#include<windows.h>

extern "C" int sum(int a, int b)
{
    return a + b;
}

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    return TRUE;
}