1. ClientExe implicitly links to PhysicsDll1, which does a load library for PhysicsDll2.
2. CCW - Com callable wrapper - using .Net COM component from unmanaged (native) code.
    2.1 Compiling .Net COM component -
        csc /platform:x64 /target:library /r:System.Windows.Forms.dll /out:CalcArea.dll CalcArea.cs
    2.2 Generating type library (tlb) for .Net the COM component, and register it (can use tlbexp.exe for generating tlb)
        c:\Windows\Microsoft.NET\Framework64\v4.0.30319\RegAsm.exe CalcArea.dll /tlb:CalcArea.tlb //The one in Framework folder is x86 version of regasm, and it will not register x64 components
    2.3 Create native DLL which will consume the .Net COM component
        in a cpp file write - 
        #import "..\CCW_ComCallableWrapper_DotNetComponent\CalcArea.tlb" no_namespace, raw_interfaces_only
        Now compile the CPP file, a .tlh file will be generated, include it as well
        #include "x64\Debug\calcarea.tlh"
    2.4 Copy CalcArea.dll to executable's directory (not current working directory but to directory in which executable is)