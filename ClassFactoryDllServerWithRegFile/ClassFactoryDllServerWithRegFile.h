#pragma once
#include<Windows.h>
class ISum : public IUnknown
{
public:
    //ISum specific method declarations
    virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;//pure virtual
};

class ISubtract :public IUnknown
{
public:
    //ISubtract specific method declarations
    virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;//pure virtual
};

//CLSID of SumSubtract Component 
// {51A31E89-0AF6-475A-86DD-2F1FE37DDA2F}
static const CLSID CLSID_SumSubtract =
{ 0x51a31e89, 0xaf6, 0x475a, { 0x86, 0xdd, 0x2f, 0x1f, 0xe3, 0x7d, 0xda, 0x2f } };

//IID of ISum Interface
// {51A31E89-0AF6-475A-86DD-2F1FE37DDA2F}
static const IID IID_ISum =
{ 0x51a31e89, 0xaf6, 0x475a, { 0x86, 0xdd, 0x2f, 0x1f, 0xe3, 0x7d, 0xda, 0x2f } };

//IID of ISubtract Interface
// {A040DFAB-9E6E-4556-9EFE-91185D83328A}
static const IID IID_ISubtract =
{ 0xa040dfab, 0x9e6e, 0x4556, { 0x9e, 0xfe, 0x91, 0x18, 0x5d, 0x83, 0x32, 0x8a } };


