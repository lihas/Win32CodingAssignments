#pragma once
#include <Windows.h>

class IMultiplication : public IUnknown
{
public:
    virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*) = 0;
};

class IDivision : public IUnknown
{
public:
    virtual HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*) = 0;
};

// {B1DF3824-DD13-483B-90FC-6F4EC9886D06}
static const CLSID CLSID_MultiplicationDivision =
{ 0xb1df3824, 0xdd13, 0x483b, { 0x90, 0xfc, 0x6f, 0x4e, 0xc9, 0x88, 0x6d, 0x6 } };

// {5F215F4E-7B8C-48B0-8FE6-B74B8E546F6B}
static const IID IID_IMultiplication =
{ 0x5f215f4e, 0x7b8c, 0x48b0, { 0x8f, 0xe6, 0xb7, 0x4b, 0x8e, 0x54, 0x6f, 0x6b } };

// {11695344-4256-4916-9708-800BE9A12178}
static const IID IID_IDivision =
{ 0x11695344, 0x4256, 0x4916, { 0x97, 0x8, 0x80, 0xb, 0xe9, 0xa1, 0x21, 0x78 } };


