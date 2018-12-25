#import "..\CCW_ComCallableWrapper_DotNetComponent\CalcArea.tlb" no_namespace, raw_interfaces_only
#include "x64\Debug\calcarea.tlh"
#include<iostream>

double calc_area_circle(int radius)
{
    HRESULT hr = CoInitialize(NULL);
    CLSID clsidCalcArea;
    hr = CLSIDFromProgID(L"CalcAreaCCW.CalcArea", &clsidCalcArea);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, "CLSIDFromProgID", "Error", MB_OK);
        return -1;
    }

    _CalcArea *app;
    hr = CoCreateInstance(clsidCalcArea, NULL, CLSCTX_INPROC_SERVER, __uuidof(_CalcArea), (void**)&app);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, "CoCreateInstance", "Error", MB_OK);
        return -2;
    }
    double res;
    hr = app->CalcCircleArea(radius, &res);
    std::cout << res;

    CoUninitialize();

    return res;
}