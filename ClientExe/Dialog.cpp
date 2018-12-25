#include <Windows.h>
#include "resource.h"
#include "Dialog.h"
#include "..\2.PhysicsDll1_implicitLink\Physics.h"
#include <string>
#include<fstream>
#include "..\4.Chemistry_ClassFactoryServer\Molarity.h"

#define PSCROLL_MIN 1
#define PSCROLL_MAX 23

#define VSCROLL_MIN 1
#define VSCROLL_MAX 24

#define TSCROLL_MIN 1
#define TSCROLL_MAX 100

#define DIALOG_SCROLL_MIN 0
#define DIALOG_SCROLL_MAX 500
#define DIALOG_SCROLL_DELTA 20

float boyles_P = 0, boyles_V = 0, boyles_T = 273;

typedef double(*calc_area_circle_type)(int radius);
calc_area_circle_type calc_area_circle;

namespace dialog
{

    void Run(HINSTANCE hInstance)
    {
        DialogBox(hInstance, MAKEINTRESOURCE(ProjectDialog), NULL, DialogProc);
    }

    INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static bool initDialogDone = false;
        static unsigned int IDC_PS_pos=0, IDC_VS_pos=0, IDC_PS2_pos=0, IDC_VS2_pos=0;
        static IMolarity* pMolarity = nullptr;

        switch (uMsg)
        {
        case WM_INITDIALOG:
        {
            CheckRadioButton(hwnd, IDC_SUBJECT_RADIO1, IDC_SUBJECT_RADIO1, IDC_SUBJECT_RADIO1);
            dialog::enableCharles(hwnd, FALSE);
            dialog::enableMaths(hwnd, FALSE);
            reset1(hwnd);

            auto hr = CoCreateInstance(CLSID_Molarity, NULL, CLSCTX_INPROC_SERVER, IID_IMolarity, (void**)&pMolarity);
            if (FAILED(hr))
            {
                MessageBox(hwnd, TEXT("IMolarity interface cannot be obtained"), TEXT("ERROR"), MB_OK);
                EndDialog(hwnd, -1);
            }

            //Init CCW
            auto hModule = LoadLibraryA("CCW_NATIVE_DLL.dll");
            if (NULL == hModule)
            {
                MessageBoxA(hwnd, "LoadLibraryA CCW_NATIVE_DLL", "Error", MB_OK);
                EndDialog(hwnd, -1);
            }

            calc_area_circle = (calc_area_circle_type)GetProcAddress(hModule, "calc_area_circle");
            if (NULL == calc_area_circle)
            {
                MessageBoxA(hwnd, "GetProcAddress calc_area_circle", "Error", MB_OK);
                EndDialog(hwnd, -1);
            }

            initDialogDone = true;
            return TRUE;
        }
        break;
        case WM_HSCROLL:
        {
            auto hCtrl = (HWND)lParam;
            
            long ctrlId = ProjectDialog;//hCtrl is null when dialog scrollbar is moved
            int scrlPos;
            if (hCtrl)
            {
                ctrlId = GetWindowLong(hCtrl, GWL_ID);
                scrlPos = GetScrollPos(hCtrl, SB_CTL);
            }
            else
            {
                scrlPos = GetScrollPos(hwnd, SB_HORZ);
            }

            switch (LOWORD(wParam))
            {
            case SB_LINELEFT:
            {
                if (ctrlId == IDC_PS)
                {
                    boyles_P--;
                    if (boyles_P < PSCROLL_MIN)
                    {
                        boyles_P = PSCROLL_MIN;
                    }
                    boylesSetP(hwnd, boyles_P);
                    
                }
                else if (ctrlId == IDC_VS)
                {
                    boyles_V--;
                    if (boyles_V < VSCROLL_MIN)
                    {
                        boyles_V = VSCROLL_MIN;
                    }
                    boylesSetV(hwnd, boyles_V);
                }
                else if (ctrlId == ProjectDialog)
                {//dialog's scroll bar
                    scrlPos -= DIALOG_SCROLL_DELTA;
                    if ((scrlPos >= DIALOG_SCROLL_MIN) && (scrlPos <= DIALOG_SCROLL_MAX))
                    {
                        ScrollWindowEx(hwnd, DIALOG_SCROLL_DELTA, 0, NULL, NULL, NULL, NULL, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
                        SetScrollPos(hwnd, SB_HORZ, scrlPos, FALSE);
                    }
                }
            }
            break;
            case SB_LINERIGHT:
            {
                if (ctrlId == IDC_PS)
                {
                    boyles_P++;
                    boylesSetP(hwnd, boyles_P);

                }
                else if (ctrlId == IDC_VS)
                {
                    boyles_V++;
                    boylesSetV(hwnd, boyles_V);

                }
                else if (ctrlId == ProjectDialog)
                {//dialog's scroll bar
                    scrlPos += DIALOG_SCROLL_DELTA;
                    if ((scrlPos >= DIALOG_SCROLL_MIN) && (scrlPos <= DIALOG_SCROLL_MAX))
                    {
                        ScrollWindowEx(hwnd, -DIALOG_SCROLL_DELTA, 0, NULL, NULL, NULL, NULL, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
                        SetScrollPos(hwnd, SB_HORZ, scrlPos, FALSE);
                    }
                }
            }
            break;
            case SB_LEFT:
            {

            }
            break;
            case SB_RIGHT:
            {

            }
            break;
            case SB_THUMBPOSITION:
                //fallthrough
            case SB_THUMBTRACK:
            {
                scrlPos = HIWORD(wParam);
                if (ctrlId == IDC_PS)
                {
                    boylesSetP(hwnd, scrlPos);

                }
                else if (ctrlId == IDC_VS)
                {
                    boylesSetV(hwnd, scrlPos);

                }
                else if (ctrlId == ProjectDialog)
                {//dialog's scroll bar
                    //SetScrollPos(hwnd, SB_HORZ, scrlPos, FALSE);
                    
                }
            }
            break;
            default:
                break;
            }
        }
        break;
        case WM_CLOSE:
        {
            EndDialog(hwnd, 0);
            if (pMolarity)
            {
                pMolarity->Release();
                pMolarity = nullptr;
            }
            return TRUE;
        }
        break;
        case WM_COMMAND:
        {
            /*if (initDialogDone)
            {
                switch (HIWORD(wParam))
                {
                case EN_CHANGE:
                {
                    int ctrlId = LOWORD(wParam);
                    switch (ctrlId)
                    {
                    case IDC_PE:
                    {
                        TCHAR buff[MAX_PATH];
                        GetDlgItemText(hwnd, ctrlId, buff, MAX_PATH);
                        float Pr = atof(buff);
                        boylesSetP(hwnd, Pr);
                    }
                    break;
                    case IDC_VE:
                    {
                        TCHAR buff[MAX_PATH];
                        GetDlgItemText(hwnd, ctrlId, buff, MAX_PATH);
                        float Vl = atof(buff);
                        boylesSetV(hwnd, Vl);
                    }
                    }
                }
                break;
                }
        }*/
            switch (LOWORD(wParam))
            {
            case IDC_compute3:
            {
                TCHAR buff[MAX_PATH];
                GetDlgItemText(hwnd, IDC_RADIUS, buff, MAX_PATH);
                int radius = atoi(buff);
                double area = calc_area_circle(radius);
                SetDlgItemText(hwnd, IDC_AREA, std::to_string(area).c_str());
            }
            break;

            case IDC_compute2:
            {
                TCHAR buff[MAX_PATH];
                GetDlgItemText(hwnd, IDC_MOLES2, buff, MAX_PATH);
                int moles = atoi(buff);
                GetDlgItemText(hwnd, IDC_VE2, buff, MAX_PATH);
                int volume = atoi(buff);
                if (pMolarity)
                {
                    float molarity;
                    pMolarity->CalculateMolarity(moles, volume, &molarity);
                    SetDlgItemText(hwnd, IDC_MOLARITY2, std::to_string(molarity).c_str());
                }
                else
                {
                    MessageBoxA(NULL, "pMolarity is null", "Error", MB_OK | MB_ICONERROR);
                }
            }
            break;
            case IDC_SAVEFILE:
            {
                std::ofstream of("boyles.txt", std::ios::app);

                TCHAR buff[MAX_PATH];
                GetDlgItemText(hwnd, IDC_MOLES, buff, MAX_PATH);
                int moles = atoi(buff);
                GetDlgItemText(hwnd, IDC_KELVIN, buff, MAX_PATH);
                int temp = atoi(buff);
                GetDlgItemText(hwnd, IDC_PE, buff, MAX_PATH);
                float pressure = atof(buff);
                GetDlgItemText(hwnd, IDC_VE, buff, MAX_PATH);
                float volume = atof(buff);

                of << "========\n";
                of << "Moles:\t" << moles;
                of << "\nTemperature(Kelvin):\t" << temp;
                of << "\nPressure:\t" << pressure;
                of << "\nVolume:\t" << volume;
                of << "\n\n";
                of.close();
                MessageBox(NULL, "Data appended to boyles.txt", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;
            case IDC_RESET:
            {
                return reset1(hwnd);
            }
            break;
            case IDOK:
            {
                OutputDebugString("IDOK");
            }
            break;
            case IDC_SUBJECT_RADIO1:
            {
                if (HIWORD(wParam) == BN_CLICKED)
                {
                    dialog::enableCharles(hwnd, FALSE);
                    dialog::enableBoyles(hwnd, TRUE);
                    dialog::enableMaths(hwnd, FALSE);
             
                }
            }
            break;
            case IDC_SUBJECT_RADIO2:
            {
                if (HIWORD(wParam) == BN_CLICKED)
                {
                    dialog::enableCharles(hwnd, TRUE);
                    dialog::enableBoyles(hwnd, FALSE);
                    dialog::enableMaths(hwnd, FALSE);
                }
            }
            break;
            case IDC_SUBJECT_RADIO3:
            {
                if (HIWORD(wParam) == BN_CLICKED)
                {
                    dialog::enableCharles(hwnd, FALSE);
                    dialog::enableBoyles(hwnd, FALSE);
                    dialog::enableMaths(hwnd, TRUE);

                }
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
    void boylesSetP(HWND hDlg, float P)
    {
        SetDlgItemText(hDlg, IDC_PE, std::to_string(P).c_str());

        TCHAR buff[MAX_PATH];
        GetDlgItemText(hDlg, IDC_MOLES, buff, MAX_PATH);
        float moles = atoi(buff);
        GetDlgItemText(hDlg, IDC_KELVIN, buff, MAX_PATH);
        float temp = atoi(buff);;

        auto V = PtoV( P, moles, temp);
        SetDlgItemText(hDlg, IDC_VE, std::to_string(P).c_str());
        auto pScroll = GetDlgItem(hDlg, IDC_PS);
        auto vScroll = GetDlgItem(hDlg, IDC_VS);

        unsigned int psc_pos = P, vsc_pos = V;
        if (P <= PSCROLL_MIN)
        {
            psc_pos = PSCROLL_MIN;
        }
        else if (P >= PSCROLL_MAX)
        {
            psc_pos = PSCROLL_MAX;
        }

        if (V <= VSCROLL_MIN)
        {
            vsc_pos = VSCROLL_MIN;
        }
        else if (V >= VSCROLL_MAX)
        {
            vsc_pos = VSCROLL_MAX;
        }

        SetScrollPos(pScroll, SB_CTL, psc_pos, TRUE);
        SetScrollPos(vScroll, SB_CTL, vsc_pos, TRUE);
        
        SetDlgItemText(hDlg, IDC_PE, std::to_string(P).c_str());
        SetDlgItemText(hDlg, IDC_VE, std::to_string(V).c_str());

        boyles_V = V;
    }
    void boylesSetV(HWND hDlg, float V)
    {
        SetDlgItemText(hDlg, IDC_VE, std::to_string(V).c_str());

        TCHAR buff[MAX_PATH];
        GetDlgItemText(hDlg, IDC_MOLES, buff, MAX_PATH);
        float moles = atoi(buff);
        GetDlgItemText(hDlg, IDC_KELVIN, buff, MAX_PATH);
        float temp = atoi(buff);;

        auto P = VtoP(V, moles, temp);
        SetDlgItemText(hDlg, IDC_PE, std::to_string(P).c_str());
        auto pScroll = GetDlgItem(hDlg, IDC_PS);
        auto vScroll = GetDlgItem(hDlg, IDC_VS);

        unsigned int psc_pos = P, vsc_pos = V;
        if (P <= PSCROLL_MIN)
        {
            psc_pos = PSCROLL_MIN;
        }
        else if (P >= PSCROLL_MAX)
        {
            psc_pos = PSCROLL_MAX;
        }

        if (V <= VSCROLL_MIN)
        {
            vsc_pos = VSCROLL_MIN;
        }
        else if (V >= VSCROLL_MAX)
        {
            vsc_pos = VSCROLL_MAX;
        }

        SetScrollPos(pScroll, SB_CTL, psc_pos, TRUE);
        SetScrollPos(vScroll, SB_CTL, vsc_pos, TRUE);

        SetDlgItemText(hDlg, IDC_PE, std::to_string(P).c_str());
        SetDlgItemText(hDlg, IDC_VE, std::to_string(V).c_str());

        boyles_P = P;
    }

    void enableCharles(HWND hDlg, BOOL enable)
    {
        for (int i = IDC_MOLES2; i <= IDC_GROUP2; i++)
        {
            auto ctrl = GetDlgItem(hDlg, i);
            EnableWindow(ctrl, enable);
        }
    }

    void enableBoyles(HWND hDlg, BOOL enable)
    {
        for (int i = IDC_GROUP1; i <= IDC_RESET; i++)
        {
            auto ctrl = GetDlgItem(hDlg, i);
            EnableWindow(ctrl, enable);
        }
    }

    void enableMaths(HWND hDlg, BOOL enable)
    {
        for (int i = IDC_GROUP3; i <= IDC_compute3; i++)
        {
            auto ctrl = GetDlgItem(hDlg, i);
            EnableWindow(ctrl, enable);
        }
    }

    BOOL reset1(HWND hwnd)
    {
        HWND hScroll = GetDlgItem(hwnd, IDC_PS);
        SetScrollRange(hScroll, SB_CTL, PSCROLL_MIN, PSCROLL_MAX, FALSE);

        hScroll = GetDlgItem(hwnd, IDC_VS);
        SetScrollRange(hScroll, SB_CTL, VSCROLL_MIN, VSCROLL_MAX, FALSE);

        hScroll = GetDlgItem(hwnd, IDC_KELVIN2);
        SetScrollRange(hScroll, SB_CTL, TSCROLL_MIN, TSCROLL_MAX, FALSE);

        hScroll = GetDlgItem(hwnd, IDC_VS2);
        SetScrollRange(hScroll, SB_CTL, VSCROLL_MIN, VSCROLL_MAX, FALSE);

        SetScrollRange(hwnd, SB_HORZ, DIALOG_SCROLL_MIN, DIALOG_SCROLL_MAX, FALSE);
        SetScrollPos(hwnd, SB_HORZ, DIALOG_SCROLL_MIN, FALSE);

        SetDlgItemText(hwnd, IDC_MOLES, "1");
        SetDlgItemText(hwnd, IDC_KELVIN, "273");
        boylesSetP(hwnd, 1.0);

        return TRUE;
    }
}