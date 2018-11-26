#pragma once
#define UNICODE
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

// {8793F988-BCEF-4E89-995C-ECFDDDC2BE10}
static const CLSID CLSID_SumSubtract =
{ 0x8793f988, 0xbcef, 0x4e89, { 0x99, 0x5c, 0xec, 0xfd, 0xdd, 0xc2, 0xbe, 0x10 } };

// {10B51558-D68D-4D2B-AC4C-368BE7758362}
static const IID IID_ISum =
{ 0x10b51558, 0xd68d, 0x4d2b, { 0xac, 0x4c, 0x36, 0x8b, 0xe7, 0x75, 0x83, 0x62 } };

// {85EECD86-EC8C-4941-ACC9-3B034C89AD77}
static const IID IID_ISubtract =
{ 0x85eecd86, 0xec8c, 0x4941, { 0xac, 0xc9, 0x3b, 0x3, 0x4c, 0x89, 0xad, 0x77 } };

