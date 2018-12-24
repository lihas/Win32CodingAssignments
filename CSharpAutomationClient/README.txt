1. convert AutomationServerTypeLib.tlb type library into a form hich .Net understands.
tlbimp ..\AutomationProxyStub\AutomationServerTypeLib.tlb /out:.\AutomationServerTypeLibForDotNet.dll
2. Compile - 
csc.exe /r:AutomationServerTypeLibForDotNet.dll CSharpAutomationClient.cs
3. run
.\CSharpAutomationClient.exe