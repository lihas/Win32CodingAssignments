#include<Windows.h>
#include "resource.h"

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_Dialog1), NULL, DlgProc);
    return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG://dialog proc's equivalent of WM_CREATE
        break;
    default:
        break;
    }
    
    return FALSE;
}