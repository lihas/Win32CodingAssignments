#pragma once
#include<Windows.h>

class IX : public IUnknown
{
public:
    virtual HRESULT __stdcall Fx() = 0;
};

// {AFAAF762-1D43-40F2-BD0C-8ED6B3FFDE07}
static const CLSID CLSID_CA =
{ 0xafaaf762, 0x1d43, 0x40f2, { 0xbd, 0xc, 0x8e, 0xd6, 0xb3, 0xff, 0xde, 0x7 } };

// {C4BBC6E7-ACEF-4681-A146-DBC33F379CBE}
static const IID IID_IX =
{ 0xc4bbc6e7, 0xacef, 0x4681, { 0xa1, 0x46, 0xdb, 0xc3, 0x3f, 0x37, 0x9c, 0xbe } };

