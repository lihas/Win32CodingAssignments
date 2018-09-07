#pragma once
#include "World.h"

class Window
{
    TCHAR m_className[MAX_PATH];// = TEXT("Gravity");
    TCHAR m_windowName[MAX_PATH];// = TEXT("Gravity");
    World m_world;
public:
    Window(HINSTANCE hInstance, int iCmdShow);
    ~Window();
    int MessageLoop();
    int paint(HDC hDC);
};

