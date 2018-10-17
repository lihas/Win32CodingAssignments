#include<Windows.h>
#include "..\DFT-DiscreteFourierTransform\DFT.h"
#include "..\AudioAPI\AudioAPI.h"
#include "resource.h"
#include <fstream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow )
{
    TCHAR szAppName[] = TEXT("DFT-Client");
    WNDCLASSEX wndclass = {};
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = DLGWINDOWEXTRA;
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("RegisterClassEx() error"), szAppName, MB_ICONERROR);
        return 0;
    }

    //HWND hwnd = CreateWindow(szAppName, "DF-Client", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    HWND hwnd = CreateDialog(hInstance, szAppName, NULL, NULL);
    ShowWindow(hwnd, iCmdShow);
    InitAudio(hInstance, hwnd);
    

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static PBYTE m_recordedData = nullptr;
    static std::vector<float> m_floatData;
    static DWORD m_recordedDataSize = 0;
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        std::vector<long double> time_epochs;
        std::vector<std::complex<long double>> signal;
        std::vector<std::complex<long double>> signal2;
        std::vector<long double> signal_real, DFT_Real;
        std::tie(time_epochs, signal) = GetSineWave(100);
        std::tie(time_epochs, signal2) = GetSineWave(50);
       
        for (int k = 0; k < signal2.size(); k++)
        {
            signal[k] = signal[k] + signal2[k];
        }

        for (auto val : signal)
        {
            signal_real.push_back(std::abs(val));
        }

        Figure(hwnd, 0);
        Stem(time_epochs, signal_real, 50);
        Figure(hwnd, 1);
        Plot(time_epochs, signal_real, 50);
        auto DFT = GetDFT(signal,1000, 1000);

        for (auto val : DFT)
        {
            DFT_Real.push_back(std::abs(val));
        }
        auto freqSpec = TimeEpochsToFrequencySpectrum(time_epochs, 1000, 1000);
        Figure(hwnd, 2);
        std::vector<long double> halfSpectrumFreq, halfSpectrumDFT;

        for (int i = 0; i < DFT_Real.size()/2; i++)
        {
            halfSpectrumFreq.push_back(freqSpec[i]);
            halfSpectrumDFT.push_back(DFT_Real[i]);
        }

        //Stem(freqSpec, DFT_Real, 50);
        Stem(halfSpectrumFreq, halfSpectrumDFT, 50);

    }
    break;
    case UM_LOG:
    {
        HWND hwLog = GetDlgItem(hwnd, IDC_LOG);
        char str[1024];
        GetWindowText(hwLog, str, 1024);
        sprintf_s(str,1024, "%s \r\n %s", str, (char*)wParam);
        SetWindowText(hwLog, str);
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    case UM_ENABLE_RECORDING:
    {
        EnableWindow(GetDlgItem(hwnd, IDC_RECORD), TRUE);
        EnableWindow(GetDlgItem(hwnd, IDC_RECORDSTOP), FALSE);
        EnableWindow(GetDlgItem(hwnd, IDC_RECORDPLOT), FALSE);
    }
    break;
    case UM_RECORDING_END_DATA:
    {
        m_recordedData = (PBYTE)wParam;
        m_recordedDataSize = (DWORD)lParam;
        float* floatData = (float*)m_recordedData;
        //std::ofstream of("datat.txt", std::ios::trunc);
        for (int i = 0; i < m_recordedDataSize && ((PBYTE)floatData < (m_recordedData + m_recordedDataSize)); i++)
        {
            m_floatData.push_back(*floatData);
            floatData++;
            //of << *floatData << "\n";
        }
        //of.close();
    }
    EnableWindow(GetDlgItem(hwnd, IDC_RECORDPLOT), TRUE);
    EnableWindow(GetDlgItem(hwnd, IDC_PLAYRECORDED), TRUE);
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_RECORD:
        {
            EnableWindow(GetDlgItem(hwnd, IDC_RECORD), FALSE);
            EnableWindow(GetDlgItem(hwnd, IDC_RECORDSTOP), TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_RECORDPLOT), FALSE);
            StartRecording();
        }
        break;
        case IDC_RECORDPLOT:
        {
            std::vector<std::complex<long double>> cmplxData;
            for (auto& dt : m_floatData)
            {
                cmplxData.push_back(std::complex<long double>(dt,0));
            }
            UINT sampling_freq = 2000;
            auto DFT = GetDFT(cmplxData, sampling_freq, cmplxData.size());
            std::vector<long double> DFT_Real;
            long double recFreq = 2000;
            std::vector<long double> time_epochs;

            long double fc = 1;
            for (auto val : DFT)
            {
                DFT_Real.push_back(std::abs(val));
                time_epochs.push_back(fc / recFreq);
                fc ++ ;
            }

            auto fs = TimeEpochsToFrequencySpectrum(time_epochs, sampling_freq, cmplxData.size());

            std::vector<long double> halfSpectrumFreq, halfSpectrumDFT;

            for (int i = 0; i < DFT_Real.size() / 2; i++)
            {
                halfSpectrumFreq.push_back(fs[i]);
                halfSpectrumDFT.push_back(DFT_Real[i]);
            }
            Figure(hwnd, 0);
            Stem(halfSpectrumFreq, halfSpectrumDFT, 100);

        }
        break;
        case IDC_PLAYRECORDED:
        {
            PlayRecordedData();
        }
        break;
        case IDC_RECORDSTOP:
        {
            EnableWindow(GetDlgItem(hwnd, IDC_RECORD), FALSE);
            EnableWindow(GetDlgItem(hwnd, IDC_RECORDSTOP), FALSE);
            EnableWindow(GetDlgItem(hwnd, IDC_RECORDPLOT), FALSE);
            StopRecording();
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

    return DefWindowProc(hwnd, msg, wParam, lParam);
}