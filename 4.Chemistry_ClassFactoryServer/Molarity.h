#pragma once
#include <Windows.h>

class IMolarity : public IUnknown
{
public:
    virtual HRESULT __stdcall  CalculateMolarity(unsigned int moles, unsigned int volumeLitre, float* molarity) = 0;
};

// {07B477A7-FDA9-44B7-8990-6C195741FCFF}
static const IID IID_IMolarity =
{ 0x7b477a7, 0xfda9, 0x44b7, { 0x89, 0x90, 0x6c, 0x19, 0x57, 0x41, 0xfc, 0xff } };

// {07B477A7-FDA9-44B7-8990-6C195741FCFF}
static const CLSID CLSID_Molarity =
{ 0x7b477a7, 0xfda9, 0x44b7, { 0x89, 0x90, 0x6c, 0x19, 0x57, 0x41, 0xfc, 0xff } };
