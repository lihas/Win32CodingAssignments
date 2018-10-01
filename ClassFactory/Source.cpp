#include "Header.h"

class CSumSubtract : ISum, ISub
{
public:
    HRESULT sumOfTwoIntegers(int a, int b, int *c)
    {
        *c = a + b;
        return S_OK;
    }

    HRESULT subtractionOfTwoIntegers(int a, int b, int *c)
    {
        *c = a + b;
        return S_OK;
    }
};