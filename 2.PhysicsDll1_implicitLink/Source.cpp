#include <Windows.h>
#include "..\3.PhysicsDll1_loadLibrary\physics_inner.h"

typedef float (*PtoV_inner_t)(float P, unsigned int moles, unsigned int temp_kelvin);
typedef float (*VtoP_inner_t)(float V, unsigned int moles, unsigned int temp_kelvin);

HMODULE hPhysicsDll2_inner = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        hPhysicsDll2_inner = LoadLibrary("PhysicsDll2.dll");
        if (hPhysicsDll2_inner == NULL)
        {
            MessageBoxA(NULL, R"P(LoadLibrary("PhysicsDll2.dll"))P", "Error", MB_OK);
            return FALSE;
        }
    }
    break;
    case DLL_PROCESS_DETACH:
    {
        FreeLibrary(hPhysicsDll2_inner);
        hPhysicsDll2_inner = NULL;
    }
    break;
    case DLL_THREAD_ATTACH:
    {

    }
    break;
    case DLL_THREAD_DETACH:
    {

    }
    break;
    default:
        break;
    }

    return TRUE;
}

float PtoV(float P, unsigned int moles, unsigned int temp_kelvin)
{
    auto f = (PtoV_inner_t)GetProcAddress(hPhysicsDll2_inner,"PtoV_inner");
    return f(P, moles, temp_kelvin);
}

float VtoP(float V, unsigned int moles, unsigned int temp_kelvin)
{
    auto f = (VtoP_inner_t)GetProcAddress(hPhysicsDll2_inner, "VtoP_inner");
    return f(V, moles, temp_kelvin);
}
