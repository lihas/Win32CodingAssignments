#pragma once
class Window
{
    TCHAR m_className[MAX_PATH];// = TEXT("Gravity");
    TCHAR m_windowName[MAX_PATH];// = TEXT("Gravity");
public:
    Window(HINSTANCE, int);
    ~Window();
    int MessageLoop();
};

