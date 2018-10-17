#pragma once
#include <Windows.h>
constexpr int UM_LOG = WM_USER + 101;
constexpr int UM_ENABLE_RECORDING = WM_USER + 102;
constexpr int UM_RECORDING_END_DATA = WM_USER + 103;
constexpr int UM_PLAY_RECORDED_DATA = WM_USER + 104;
HRESULT WINAPI InitAudio(HINSTANCE hInstance, HWND hwndParent);
HRESULT StartRecording();
HRESULT StopRecording();
HRESULT PlayRecordedData();
