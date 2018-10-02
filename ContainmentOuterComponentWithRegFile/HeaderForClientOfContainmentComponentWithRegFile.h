#pragma once
class ISum : public IUnknown
{
public :
    virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int *) = 0;
};

class ISubtract : public IUnknown
{
public:
    virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *) = 0;
};

class IMultiplication : public IUnknown
{
public:
    virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int *) = 0;
};

class IDivision : public IUnknown
{
public:
    virtual HRESULT __stdcall DivisionOfTwoIntegers(int, int, int *) = 0;
};

// CLSID of SumSubtract Component {772B9B2D-46A0-4695-A3B4-A06C7EE952B5}
static const CLSID CLSID_SumSubtract =
{ 0x772b9b2d, 0x46a0, 0x4695, { 0xa3, 0xb4, 0xa0, 0x6c, 0x7e, 0xe9, 0x52, 0xb5 } };

// IID of ISum interface {9DBF3C62-04DB-4C1F-A19F-9F6BF281FCAE}
static const IID IID_ISum =
{ 0x9dbf3c62, 0x4db, 0x4c1f, { 0xa1, 0x9f, 0x9f, 0x6b, 0xf2, 0x81, 0xfc, 0xae } };

// IID of ISubtract interface {A0CC8A9C-6C83-4CCD-BDCC-64FA5B1DB4E8}
static const IID IID_ISubtract =
{ 0xa0cc8a9c, 0x6c83, 0x4ccd, { 0xbd, 0xcc, 0x64, 0xfa, 0x5b, 0x1d, 0xb4, 0xe8 } };

// {60C73EBF-839A-4433-BC73-3796D0118FC8}
static const IID IID_IMultiplication =
{ 0x60c73ebf, 0x839a, 0x4433, { 0xbc, 0x73, 0x37, 0x96, 0xd0, 0x11, 0x8f, 0xc8 } };

// {D1C0968C-3F33-40B8-8C75-69A49FD6ADA5}
static const IID IID_IDivision =
{ 0xd1c0968c, 0x3f33, 0x40b8, { 0x8c, 0x75, 0x69, 0xa4, 0x9f, 0xd6, 0xad, 0xa5 } };


