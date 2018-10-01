#pragma once
#include <Windows.h>
class ISum : IUnknown
{
public:
    virtual HRESULT __stdcall  sumOfTwoIntegers(int, int, int*)=0;

};

class ISub : IUnknown
{
public:
    virtual HRESULT __stdcall subtractionOfTwoIntegers(int, int, int*)=0;
};

// {8CACD05B-72EB-4F3D-85FB-9C6ABE0BAFE0}
static const CLSID CLSID_CSUbSubtract =
{ 0x8cacd05b, 0x72eb, 0x4f3d, { 0x85, 0xfb, 0x9c, 0x6a, 0xbe, 0xb, 0xaf, 0xe0 } };

// {E7BC54FD-FF0D-4929-9BB2-3015B31AF599}
static const IID IID_ISum =
{ 0xe7bc54fd, 0xff0d, 0x4929, { 0x9b, 0xb2, 0x30, 0x15, 0xb3, 0x1a, 0xf5, 0x99 } };

// {ED47A6C8-02EA-4CE2-A4EC-2D87D9E6466F}
static const IID IID_ISum =
{ 0xed47a6c8, 0x2ea, 0x4ce2, { 0xa4, 0xec, 0x2d, 0x87, 0xd9, 0xe6, 0x46, 0x6f } };
