#include<windows.h>
#include<iostream>

using namespace std;

typedef int(*sumType)(int, int);

int main()
{
    auto hModule = LoadLibrary("DllPrivateExport");
    if (hModule == NULL)
    {
        cout << "load library error" << endl;
        return -1;
    }

    auto pSum = (sumType)GetProcAddress(hModule, "sum");
    if (pSum == NULL)
    {
        cout << "GetProcAddress error" << endl;
        return -2;
    }
    cout << "Sum of 2, and 3 is " << pSum(2, 3) << endl;
    return 0;
}