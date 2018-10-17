#include "AudioAPI.h"
#include <memory>
#include <mmreg.h>
#include <string>

LRESULT CALLBACK AudioWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD MessagePump(LPVOID);
constexpr int UM_INIT_AUDIO = WM_USER + 11;
constexpr int UM_START_RECORDING = WM_USER + 12;
constexpr int UM_STOP_RECORDING = WM_USER + 13;

constexpr int INP_BUFFER_SIZE = 16384;

#define LOG(str) { \
if(m_hwndParent){\
SendMessage(m_hwndParent, UM_LOG,(WPARAM)str,NULL);\
}\
}

HWND m_hwndParent = NULL, m_MyHwnd = NULL;
HRESULT WINAPI InitAudio(HINSTANCE hInstance, HWND hwndParent)
{
    TCHAR szAppName[] = "AudioAPIHiddenWindow";
    WNDCLASSEX wndclass = {};
    wndclass.cbSize = sizeof(wndclass);
    wndclass.lpfnWndProc = AudioWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hInstance = hInstance;

    m_hwndParent = hwndParent;

    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("RegisterClassEx() error"), szAppName, MB_ICONERROR);
        return 0;
    }

    m_MyHwnd = CreateWindowEx(NULL,szAppName, szAppName, NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

    CreateThread(NULL, 0, MessagePump, &m_MyHwnd, 0, NULL);
    
    SendMessage(m_MyHwnd, UM_INIT_AUDIO, NULL, NULL);
    return S_OK;

}

HRESULT StartRecording()
{
    SendMessage(m_MyHwnd, UM_START_RECORDING, NULL, NULL);
    return S_OK;
}

HRESULT StopRecording()
{
    SendMessage(m_MyHwnd, UM_STOP_RECORDING, NULL, NULL);
    return S_OK;
}

HRESULT PlayRecordedData()
{
    SendMessage(m_MyHwnd, UM_PLAY_RECORDED_DATA, NULL, NULL);
    return S_OK;
}


DWORD MessagePump(LPVOID lpParam)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }
    return DWORD();
}

LRESULT CALLBACK AudioWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static WAVEFORMATEX waveFmt = {};
    static HWAVEIN hWaveIn;
    static HWAVEOUT hWaveOut;
    static PBYTE pSaveBuffer = nullptr;
    static BYTE pBuffer1[INP_BUFFER_SIZE], pBUffer2[INP_BUFFER_SIZE];
    static WAVEHDR waveHdr1, waveHdr2;
    static bool bRecording = false, bEnding = false;
    static DWORD cbBytesSaved = 0;

    switch (msg)
    {
    case UM_STOP_RECORDING:
    {
        bEnding = true;
        waveInReset(hWaveIn);
    }
    break;
    case UM_INIT_AUDIO:
    {
        waveFmt.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
        waveFmt.wBitsPerSample = 32;
        waveFmt.nSamplesPerSec = 2000;
        waveFmt.nChannels = 1;
        waveFmt.cbSize = 0;
        waveFmt.nBlockAlign = (waveFmt.nChannels * waveFmt.wBitsPerSample) / 8;//https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx ; Also in petzold
        waveFmt.nAvgBytesPerSec = (waveFmt.nBlockAlign * waveFmt.nSamplesPerSec); //petzold (page 1297)

        auto err = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFmt, (DWORD)hwnd, 0, CALLBACK_WINDOW);
        std::string errString;
        switch (err)
        {
        case MMSYSERR_ALLOCATED:
            errString = "MMSYSERR_ALLOCATED"; break;
        case MMSYSERR_BADDEVICEID:
            errString = "MMSYSERR_BADDEVICEID"; break;
        case MMSYSERR_NODRIVER:
            errString = "MMSYSERR_NODRIVER"; break;
        case MMSYSERR_NOMEM:
            errString = "MMSYSERR_NOMEM"; break;
        case WAVERR_BADFORMAT:
            errString = "WAVERR_BADFORMAT"; break;
        default:
            break;
        }

        if (err != MMSYSERR_NOERROR)
        {
            MessageBoxA(NULL, (std::string("waveInOpen()") + errString).c_str(), "Error", MB_ICONERROR);
        }
        else
        {
            LOG("WaveInOpen() Success");
        }

        //Double buffering
        waveHdr1.lpData = (LPSTR)pBuffer1;
        waveHdr1.dwBufferLength = INP_BUFFER_SIZE;
        waveHdr1.dwBytesRecorded = 0;
        waveHdr1.dwUser = 0;
        waveHdr1.dwFlags = 0;
        waveHdr1.dwLoops = 1;
        waveHdr1.lpNext = NULL;
        waveHdr1.reserved = 0;

        if (waveInPrepareHeader(hWaveIn, &waveHdr1, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
        {
            LOG("waveInPrepareHeader() error");
        }
        else
        {
            LOG("waveInPrepareHeader() success");
        }

        waveHdr2 = waveHdr1;
        waveHdr2.lpData = (LPSTR)pBUffer2;

        if (waveInPrepareHeader(hWaveIn, &waveHdr2, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
        {
            LOG("waveInPrepareHeader() error");
        }
        else
        {
            LOG("waveInPrepareHeader() success");
        }

    }
    break;
    case MM_WIM_OPEN:
    {
        if (m_hwndParent)
        {
            SendMessage(m_hwndParent, UM_ENABLE_RECORDING, NULL, NULL);
        }

        waveInAddBuffer(hWaveIn, &waveHdr1, sizeof(WAVEHDR));
        waveInAddBuffer(hWaveIn, &waveHdr2, sizeof(WAVEHDR));
    }
    break;
    case MM_WOM_OPEN:
    {
        static WAVEHDR wvHdr ;
        wvHdr.lpData = (LPSTR)pSaveBuffer;
        wvHdr.dwBufferLength = cbBytesSaved;
        wvHdr.dwBytesRecorded = 0;
        wvHdr.dwUser = 0;
        wvHdr.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
        wvHdr.dwLoops = 10;
        wvHdr.lpNext = nullptr;
        wvHdr.reserved = NULL;

        waveOutPrepareHeader(hWaveOut, &wvHdr, sizeof(wvHdr));
        waveOutWrite(hWaveOut, &wvHdr, sizeof(wvHdr));
    }
    break;
    case UM_PLAY_RECORDED_DATA:
    {
        WAVEFORMATEX waveFmt;
        waveFmt.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
        waveFmt.wBitsPerSample = 32;
        waveFmt.nSamplesPerSec = 2000;
        waveFmt.nChannels = 1;
        waveFmt.cbSize = 0;
        waveFmt.nBlockAlign = (waveFmt.nChannels * waveFmt.wBitsPerSample) / 8;//https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx ; Also in petzold
        waveFmt.nAvgBytesPerSec = (waveFmt.nBlockAlign * waveFmt.nSamplesPerSec); //petzold (page 1297)

        //auto err = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFmt, (DWORD)hwnd, 0, CALLBACK_WINDOW);
        auto err = waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFmt, (DWORD)hwnd, 0, CALLBACK_WINDOW);
    }
    break;
    case UM_START_RECORDING:
    {
        waveInStart(hWaveIn);
        bRecording = true;
        LOG("Start recording");
    }
    break;
    case MM_WIM_DATA:
    {
        pSaveBuffer = (PBYTE)realloc(pSaveBuffer, cbBytesSaved + ((PWAVEHDR)lParam)->dwBytesRecorded);
        if (pSaveBuffer == nullptr)
        {
            MessageBoxA(NULL, "Realloc Error()", "Audio API", MB_OK);
        }
        CopyMemory(pSaveBuffer + cbBytesSaved, ((PWAVEHDR)lParam)->lpData, ((PWAVEHDR)lParam)->dwBytesRecorded);
        cbBytesSaved += ((PWAVEHDR)lParam)->dwBytesRecorded;

        if (bEnding)
        {
            waveInClose(hWaveIn);
            break;
        }
        waveInAddBuffer(hWaveIn, (PWAVEHDR)lParam, sizeof(WAVEHDR));
    }
    break;
    case MM_WIM_CLOSE:
    {
        waveInUnprepareHeader(hWaveIn, &waveHdr1, sizeof(WAVEHDR));
        waveInUnprepareHeader(hWaveIn, &waveHdr2, sizeof(WAVEHDR));
        bRecording = false;
        if (m_hwndParent)
        {
            SendMessage(m_hwndParent, UM_RECORDING_END_DATA, (WPARAM)pSaveBuffer, (LPARAM)cbBytesSaved);
        }
    }
    break;
    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}