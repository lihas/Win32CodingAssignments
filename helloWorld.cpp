/*
Hello world WINRT
Compile - cl.exe /c /EHsc /ZW helloWorld.cpp
Link - link.exe helloWorld.obj vccorlib.lib runtimeobject.lib /SUBSYSTEM:CONSOLE

Compile and link at once - cl.exe /EHsc /ZW helloWorld.cpp /link vccorlib.lib runtimeobject.lib /SUBSYSTEM:CONSOLE
*/

#include <iostream>
using namespace std;
using namespace Platform;

int main(Array<String ^>^ args)
{
    String^ str = "Hello World!!!";
    wcout << str->Data() << endl;
    return 0;
}