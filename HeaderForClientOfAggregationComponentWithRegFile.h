#include <Windows.h>

class ISum : public IUnknown
{
public:
    virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract : public IUnknown
{
public:
    virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

// {230A97DE-65C8-43BA-A067-6720E546DB4C}
static const CLSID CLSID_SumSubtract =
{ 0x230a97de, 0x65c8, 0x43ba, { 0xa0, 0x67, 0x67, 0x20, 0xe5, 0x46, 0xdb, 0x4c } };

// {A1D9593A-32C6-446D-A193-A325D4315450}
static const IID IID_ISum =
{ 0xa1d9593a, 0x32c6, 0x446d, { 0xa1, 0x93, 0xa3, 0x25, 0xd4, 0x31, 0x54, 0x50 } };

// {2D0BADD0-46CB-4DE3-853A-49A0B04F9E11}
static const IID IID_ISubtract =
{ 0x2d0badd0, 0x46cb, 0x4de3, { 0x85, 0x3a, 0x49, 0xa0, 0xb0, 0x4f, 0x9e, 0x11 } };


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
