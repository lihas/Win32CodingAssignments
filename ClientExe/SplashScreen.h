#pragma once
#include <Windows.h>
class SplashScreen
{
    HWND m_hwnd;
public:
    SplashScreen(HINSTANCE hInstance);
    ~SplashScreen();
    void ShowWindow();
private:
    void StartTimer(int ms);
};

