cd "%~dp0"
regtlibv12.exe AutomationServerTypeLib.tlb
regsvr32 /s AutomationProxyStub.dll
RegAsm64.exe CalcArea.dll /tlb:CalcArea.tlb
EXIT 0