#pragma once
#include<Windows.h>

//Structures
enum class Gender
{
    MALE,
    FEMALE,
    OTHER
};

struct DlgData {
    TCHAR name[MAX_PATH];
    UINT age;
    UINT salary;
    Gender gender;
    bool isMarried;
};

