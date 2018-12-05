#include "Common.h"
#include <windowsx.h>
#include "resource.h"
#include <string>
#include "..\UserDataStoreFileIO\FileIO.h"

constexpr TCHAR AppName[] = TEXT("UserDataStore");

//Custom Message
#define WM_saveDlgData (WM_USER+1001) //lParam -> DlgData
#define ID_MONITOR_DLG_TIMER (WM_USER+1002)

//prototypes
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpszClassName = AppName;
    wc.lpszMenuName = nullptr;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "RegisterClassEx() failed", "Error", MB_OK);
    }

    HWND hwnd = CreateWindow(AppName, AppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, nullptr);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //static variables
    static HINSTANCE hInstance;
    static bool userDataPresent = false;
    static DlgData dlgData;
    static int userCount = 0;
    static bool newDataAvailable = false;
    static auto hLib = LoadLibrary("UserDataStoreFileIO.dll");
    static SaveDataToFile_t SaveDataToFile = (SaveDataToFile_t)GetProcAddress(hLib, "SaveDataToFile");

    switch (iMsg)
    {
    case WM_CREATE:
    {
        hInstance = (LPCREATESTRUCT(lParam))->hInstance;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);

        if (!userDataPresent)
        {
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(0, 0, 0));

            DrawText(hdc, "Press Spacebar to continue...", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
        }
        else
        {//Show data entered by user
            SetTextColor(hdc, RGB(0, 255, 0));
            SetBkColor(hdc, RGB(0, 0, 0));
            DrawText(hdc, "Press 'F' to save data to a file", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

            RECT rc2 = rc;
            constexpr int STEP_X = 20, STEP_Y = 120;
            rc2.left += STEP_Y;
            DrawText(hdc, "Name:", -1, &rc, DT_LEFT | DT_SINGLELINE | DT_TOP);
            DrawText(hdc, dlgData.name, -1, &rc2, DT_LEFT | DT_SINGLELINE | DT_TOP);

            rc.top += STEP_X;
            rc2.top = rc.top;
            DrawText(hdc, "Age:", -1, &rc, DT_LEFT | DT_SINGLELINE | DT_TOP);
            DrawText(hdc, std::to_string(dlgData.age).c_str(), -1, &rc2, DT_LEFT | DT_SINGLELINE | DT_TOP);

            rc.top += STEP_X;
            rc2.top = rc.top;
            DrawText(hdc, "Salary:", -1, &rc, DT_LEFT | DT_SINGLELINE | DT_TOP);
            DrawText(hdc, std::to_string(dlgData.salary).c_str(), -1, &rc2, DT_LEFT | DT_SINGLELINE | DT_TOP);

            std::string str;
            switch (dlgData.gender)
            {
            case Gender::MALE:
            {
                str = "Male";
            }
            break;
            case Gender::FEMALE:
            {
                str = "Female";
            }
            break;
            case Gender::OTHER:
            {
                str = "Other";
            }
            break;
            default:
            {
                MessageBoxA(NULL, "Unexpected gender", "Error", MB_OK | MB_ICONERROR);
            }
                break;
            }
            rc.top += STEP_X;
            rc2.top = rc.top;
            DrawText(hdc, "Gender:", -1, &rc, DT_LEFT | DT_SINGLELINE | DT_TOP);
            DrawText(hdc, str.c_str(), -1, &rc2, DT_LEFT | DT_SINGLELINE | DT_TOP);

            rc.top += STEP_X;
            rc2.top = rc.top;
            DrawText(hdc, "Marital Status:", -1, &rc, DT_LEFT | DT_SINGLELINE | DT_TOP);
            DrawText(hdc, dlgData.isMarried?"Married":"Unmarried", -1, &rc2, DT_LEFT | DT_SINGLELINE | DT_TOP);
        }

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_SPACE:
        {
            //Modal
            userCount++;
            auto res = DialogBox(hInstance, "UserDataStore", hwnd, DlgProc);
            if (res == IDOK)//User clicked OK
            {

            }
        }
        break;
        case VK_ESCAPE:
        {
            DestroyWindow(hwnd);
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }
        break;
        default:
            break;
        }
    }
    break;
    case WM_CHAR:
    {
        auto keyPressed = wParam;
        if (keyPressed == 'f' || keyPressed == 'F')
        {
            //save data
            if (newDataAvailable)
            {
                auto path = SaveDataToFile(dlgData, userCount);
                newDataAvailable = false;
                MessageBoxA(hwnd, path.c_str(), "Data saved successfully to file", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBoxA(hwnd, "File containing your information has already been saved", "Error", MB_OK | MB_ICONERROR);
            }
        }
    }
    break;
    case WM_saveDlgData:
    {
        dlgData = *(DlgData*)(lParam);//copy
        userDataPresent = true;
        newDataAvailable = true;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        break;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void resetDlg(HWND hDlg)
{
    CheckRadioButton(hDlg, IDC_GENDER_MALE, IDC_GENDER_MALE, IDC_GENDER_MALE);
    CheckRadioButton(hDlg, IDC_MARY_Y, IDC_MARY_Y, IDC_MARY_Y);
    SetWindowText(hDlg, "");
    SetDlgItemText(hDlg, IDC_NAME, "");
    SetDlgItemText(hDlg, IDC_AGE, "");
    SetDlgItemText(hDlg, IDC_SALARY, "");

    EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
    {
        resetDlg(hwnd);
        SetTimer(hwnd, ID_MONITOR_DLG_TIMER, 200, NULL);
        return TRUE;
    }
    break;
    case WM_TIMER:
    {
        switch (wParam)
        {
        case ID_MONITOR_DLG_TIMER:
        {
            bool bEnableOk = true;
            TCHAR str[MAX_PATH];
            GetDlgItemText(hwnd, IDC_NAME, str, MAX_PATH);
            if(str[0])
            {
                bEnableOk = bEnableOk && true;
            }
            else
            {
                bEnableOk = bEnableOk && false;
            }

            GetDlgItemText(hwnd, IDC_AGE, str, MAX_PATH);
            if (str[0])
            {
                bEnableOk = bEnableOk && true;
            }
            else
            {
                bEnableOk = bEnableOk && false;
            }

            GetDlgItemText(hwnd, IDC_SALARY, str, MAX_PATH);
            if (str[0])
            {
                bEnableOk = bEnableOk && true;
            }
            else
            {
                bEnableOk = bEnableOk && false;
            }

            if (bEnableOk)
            {
                EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
            }
            else
            {
                EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
            }
        }
        break;
        default:
            break;
        }
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
        {
            KillTimer(hwnd, ID_MONITOR_DLG_TIMER);
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
        case IDOK:
        {
            DlgData dlgData;
            GetDlgItemText(hwnd, IDC_NAME, dlgData.name, MAX_PATH);
            dlgData.age = GetDlgItemInt(hwnd, IDC_AGE, nullptr, FALSE);
            dlgData.salary = GetDlgItemInt(hwnd, IDC_SALARY, nullptr, FALSE);
            
            //Gender
            int isMale = Button_GetCheck(GetDlgItem(hwnd, IDC_GENDER_MALE));
            int isFemale = Button_GetCheck(GetDlgItem(hwnd, IDC_GENDER_FEMALE));
            int isOther = Button_GetCheck(GetDlgItem(hwnd, IDC_GENDER_OTHER));

            if (isMale + isFemale + isOther == 1)
            {//only one selected
                if (isMale == 1)
                {
                    dlgData.gender = Gender::MALE;
                }
                else if (isFemale == 1)
                {
                    dlgData.gender = Gender::FEMALE;
                }
                else if (isOther == 1)
                {
                    dlgData.gender = Gender::OTHER;
                }
                else
                {
                    MessageBoxA(NULL, "1. Gender has to be selected, cannot select more than 1 - bug in code", "Error", MB_OK | MB_ICONERROR);
                    EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
                }

            }
            else
            {
                MessageBoxA(NULL, "2. Gender has to be selected, cannot select more than 1 - bug in code", "Error", MB_OK | MB_ICONERROR);
                EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
            }


            //Marital status
            bool isMarried = Button_GetCheck(GetDlgItem(hwnd, IDC_MARY_Y));
            bool isunMarried = Button_GetCheck(GetDlgItem(hwnd, IDC_MARY_N));

            if (isMarried ^ isunMarried)
            {//only one selected
                if (isMarried)
                {
                    dlgData.isMarried = true;
                }
                else if (isunMarried)
                {
                    dlgData.isMarried = false;
                }
                else
                {
                    MessageBoxA(NULL, "1. Marital status has to be selected, cannot select more than 1 - bug in code", "Error", MB_OK | MB_ICONERROR);
                    EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
                }

            }
            else
            {
                MessageBoxA(NULL, "1. Marital status has, cannot select more than 1 - bug in code", "Error", MB_OK | MB_ICONERROR);
                EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
            }

            //send data
            auto hParent = GetParent(hwnd);
            SendMessage(hParent, WM_saveDlgData, (WPARAM)hwnd, (LPARAM)&dlgData);
            EndDialog(hwnd, IDOK);
        }
        break;
        case IDRESET:
        {
            resetDlg(hwnd);
            return TRUE;
        }
        break;
        default:
            break;
        }
    }
    break;
    default:
        break;
    }
    return FALSE;
}
