#include <Windows.h>
#include "resource.h"
#include "Dialog.h"
#include "..\2.PhysicsDll1_implicitLink\Physics.h"
#include <string>
#include<fstream>

#define PSCROLL_MIN 1
#define PSCROLL_MAX 23

#define VSCROLL_MIN 1
#define VSCROLL_MAX 24

#define TSCROLL_MIN 1
#define TSCROLL_MAX 100

float boyles_P = 0, boyles_V = 0, boyles_T = 273;

namespace dialog
{

    void Run(HINSTANCE hInstance)
    {
        DialogBox(hInstance, TEXT("ProjectDialog"), NULL, DialogProc);
    }

    INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static unsigned int IDC_PS_pos=0, IDC_VS_pos=0, IDC_PS2_pos=0, IDC_VS2_pos=0;
        switch (uMsg)
        {
        case WM_INITDIALOG:
        {
            CheckRadioButton(hwnd, IDC_SUBJECT_RADIO1, IDC_SUBJECT_RADIO1, IDC_SUBJECT_RADIO1);
            dialog::enableCharles(hwnd, FALSE);
            return reset1(hwnd);
        }
        break;
        case WM_HSCROLL:
        {
            auto hCtrl = (HWND)lParam;
            auto ctrlId = GetWindowLong(hCtrl, GWL_ID);
            int scrlPos = GetScrollPos(hCtrl, SB_CTL);
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
            return TRUE;
        }
        break;
        case WM_COMMAND:
        {
            /*switch (HIWORD(wParam))
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
            }*/

            switch (LOWORD(wParam))
            {
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
             
                }
            }
            break;
            case IDC_SUBJECT_RADIO2:
            {
                if (HIWORD(wParam) == BN_CLICKED)
                {
                    dialog::enableCharles(hwnd, TRUE);
                    dialog::enableBoyles(hwnd, FALSE);
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

        SetDlgItemText(hwnd, IDC_MOLES, "1");
        SetDlgItemText(hwnd, IDC_KELVIN, "273");
        boylesSetP(hwnd, 1.0);

        return TRUE;
    }
}