#pragma once
namespace dialog {
    INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void Run(HINSTANCE hInstance);
    void enableBoyles(HWND hwnd, BOOL enable = TRUE);
    void enableCharles(HWND hwnd, BOOL enable = TRUE);
    void boylesSetP(HWND hDlg, float P);
    void boylesSetV(HWND hDlg, float V);
    BOOL reset1(HWND hwnd);
}