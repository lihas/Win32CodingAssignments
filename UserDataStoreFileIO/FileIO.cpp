#include <iostream>
#include <string>
#include <algorithm>
#include<fstream>
#include<filesystem>
#include "FileIO.h"

using std::string;
using std::to_string;
using std::count;
using std::transform;
using std::back_inserter;
using std::ofstream;

string SaveDataToFile(DlgData dlgData, int userCount)
{
    string fileName = to_string(userCount);
    fileName += "_";
    string userName = dlgData.name;
    transform(userName.begin(), userName.end(), back_inserter(fileName), [](char c) -> char {if (c == ' ')  return '_';  else  return c; });
    fileName += ".txt";
    ofstream of(fileName, std::ios::trunc);
    of << "Name: " <<dlgData.name << std::endl;
    of << "Age: " <<dlgData.age << std::endl;
    of << "Salary: " <<dlgData.salary << std::endl;
    
    switch (dlgData.gender)
    {
    case Gender::MALE:
    {
        of << "Gender: male";
    }
    break;
    case Gender::FEMALE:
    {
        of << "Gender: female";
    }
    break;
    case Gender::OTHER:
    {
        of << "Gender: other";
    }
    break;
    }
    of << std::endl;
    of << "Marital status: " << (dlgData.isMarried ? "Married" : "Unmarried") << std::endl;

    return std::experimental::filesystem::current_path().string() + "\\" + fileName;
    
}