#pragma once
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


// {07F9961F-C419-4619-9700-036936E5E5C7}
static const CLSID CLSID_MultiplicationDivision =
{ 0x7f9961f, 0xc419, 0x4619, { 0x97, 0x0, 0x3, 0x69, 0x36, 0xe5, 0xe5, 0xc7 } };

// {60C73EBF-839A-4433-BC73-3796D0118FC8}
static const IID IID_IMultiplication =
{ 0x60c73ebf, 0x839a, 0x4433, { 0xbc, 0x73, 0x37, 0x96, 0xd0, 0x11, 0x8f, 0xc8 } };

// {D1C0968C-3F33-40B8-8C75-69A49FD6ADA5}
static const IID IID_IDivision =
{ 0xd1c0968c, 0x3f33, 0x40b8, { 0x8c, 0x75, 0x69, 0xa4, 0x9f, 0xd6, 0xad, 0xa5 } };
