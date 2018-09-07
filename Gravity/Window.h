#pragma once
#include "World.h"

class Window
{
    TCHAR m_className[MAX_PATH];// = TEXT("Gravity");
    TCHAR m_windowName[MAX_PATH];// = TEXT("Gravity");
    World m_world;
    HBRUSH m_BackgroundBrush;
public:
    HBRUSH GetBackgroundBrush();
    int ScaleDown(HDC hDC);
    Window(HINSTANCE hInstance, int iCmdShow);
    ~Window();
    int MessageLoop();
    int paint(HDC hDC);
};

