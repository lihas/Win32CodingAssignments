Create file AutomationServerTypeLib.idl, and then run the following command in Visual Studio developer command prompt

midl /env x64 /h AutomationProxyStubHeader.h /iid AutomationProxyStubGuids.c /dlldata AutomationProxyStubDlldata.c /proxy AutomationProxyStub.c AutomationServerTypeLib.idl

get from win7, and run the following command to register type library
regtlibv12.exe AutomationServerTypeLib.tlb
regsvr32 AutomationProxyStub.dll (admin command prompt)