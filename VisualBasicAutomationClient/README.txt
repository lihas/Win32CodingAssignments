1. convert AutomationServerTypeLib.tlb type library into a form hich .Net understands.
tlbimp ..\AutomationProxyStub\AutomationServerTypeLib.tlb /out:.\AutomationServerTypeLibForDotNet.dll
2. Compile
vbc /t:winexe /r:Microsoft.VisualBasic.dll /r:AutomationServerTypeLibForDotNet.dll VBAutomationClient.vb
3. Run
.\VBAutomationClient.exe