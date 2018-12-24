#pragma once
#include <Windows.h>

class IMyMath : public IDispatch
{
public:
    //pure virtual
    virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;

    //pure virtual
    virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

// CLSID of MyMath component {8DCE162D-7C38-4AAF-9CFA-6882EF11FD2D}
static const CLSID CLSID_MyMath =
{ 0x8dce162d, 0x7c38, 0x4aaf, { 0x9c, 0xfa, 0x68, 0x82, 0xef, 0x11, 0xfd, 0x2d } };

// IID of ISum interface {A74D05B6-C656-4B8C-BF2B-E9B2E1E1E896}
static const IID IID_IMyMath =
{ 0xa74d05b6, 0xc656, 0x4b8c, { 0xbf, 0x2b, 0xe9, 0xb2, 0xe1, 0xe1, 0xe8, 0x96 } };

