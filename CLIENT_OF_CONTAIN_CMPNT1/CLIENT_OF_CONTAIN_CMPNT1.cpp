// CLIENT_OF_CONTAIN_CMPNT1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include<iostream>
#include<Windows.h>
#include "..\CONTAIN_CMPNT1\HeaderForClient.h"

int main()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, "CoInitialize Error", "Error", MB_OK);
        exit(0);
    }

    IX *pIX = NULL;
    IY* pIY = NULL;

    hr = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IX, (void**)&pIX);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "CoCreateInstance Error", "Error", MB_OK);
        exit(0);
    }

    pIX->Fx();

    hr = pIX->QueryInterface(IID_IY, (void**)&pIY);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "pIX->QueryInterface(IID_IY, (void**)&pIY)", "Error", MB_OK);
        exit(0);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
