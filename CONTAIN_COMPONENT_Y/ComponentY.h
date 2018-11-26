#pragma once
#include <Windows.h>

class IY : public IUnknown
{
public:
    virtual HRESULT __stdcall Fy() = 0;
};

//CLSID of component Y
// {07623B15-AA91-4D2F-AA92-E0D2C853C67E}
static const CLSID CLSID_CComponentY =
{ 0x7623b15, 0xaa91, 0x4d2f, { 0xaa, 0x92, 0xe0, 0xd2, 0xc8, 0x53, 0xc6, 0x7e } };


//IID of interface IY
// {7335C1EC-F226-4573-82F2-E208C36593AB}
static const IID IID_IY =
{ 0x7335c1ec, 0xf226, 0x4573, { 0x82, 0xf2, 0xe2, 0x8, 0xc3, 0x65, 0x93, 0xab } };

