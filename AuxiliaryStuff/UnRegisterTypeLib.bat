cd "%~dp0"
regtlibv12.exe -u AutomationServerTypeLib.tlb
regsvr32 /s /u AutomationProxyStub.dll
RegAsm64.exe /unregister CalcArea.dll /tlb:CalcArea.tlb
EXIT 0