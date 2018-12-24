#include <stdio.h>
#include "AutomationServer.h"

//coclass declaration
class CMyMath : public IMyMath
{
private:
    long m_cRef;
    ITypeInfo *m_pITypeInfo = NULL;
public:
    CMyMath();
    ~CMyMath();

    //IUnknown specific method declaratios (inherited)
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IDispathc specific method declarations (inherited)
    HRESULT __stdcall GetTypeInfoCount(UINT*);
    HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**);
    HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*);
    HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

    //ISum specific method declarations (inherited)
    HRESULT __stdcall SumOfTwoIntegers(int, int, int*);

    //ISubtract specific method declarations (inherited)
    HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

    //custom methods
    HRESULT InitInstance();
};

// class factory declaration
class CMyMathClassFactory : public IClassFactory
{
private:
    long m_cRef;
public:
    //contructor
    CMyMathClassFactory();
    ~CMyMathClassFactory();

    //IUnknown specific methods
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IClassFactory specific
    HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
    HRESULT __stdcall LockServer(BOOL);
};

//global DLL handle
HMODULE ghModule = NULL;

//global variables
long glNumberOfActiveComponents = 0; //no. of active components
long glNumberOfServerLocks = 0; //number of locks on this dll

// {D625B198-FBC0-4814-8109-62607ECCA9E1}
static const GUID LIBID_AutomationServer =
{ 0xd625b198, 0xfbc0, 0x4814, { 0x81, 0x9, 0x62, 0x60, 0x7e, 0xcc, 0xa9, 0xe1 } };

//DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        ghModule = hDll;
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}

//CMyMath contructor
CMyMath::CMyMath()
{
    m_cRef = 1; //hardcoded initialization to anticipate possible failure
    //of QueryInterface()
    InterlockedIncrement(&glNumberOfActiveComponents); //increment global counter
}

CMyMath::~CMyMath()
{
    InterlockedDecrement(&glNumberOfActiveComponents); //descrement global counter
}

//Implementation of CMyMath's IUnknown's methods
HRESULT CMyMath::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else if (riid == IID_IDispatch)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else if (riid == IID_IMyMath)
    {
        *ppv = static_cast<IMyMath*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG CMyMath::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMyMath::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        m_pITypeInfo->Release();
        m_pITypeInfo = NULL;
        delete this;
        return 0;
    }
    return m_cRef;
}

//Implementation of IMyMath's methods
HRESULT CMyMath::SumOfTwoIntegers(int num1, int num2, int* pSum)
{
    *pSum = num1 + num2;
    return S_OK;
}

HRESULT CMyMath::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
    *pSubtract = num1 - num2;
    return S_OK;
}

HRESULT CMyMath::InitInstance()
{
    //function decarations
    void ComErrorDescriptionString(HWND, HRESULT);

    //variable declarations
    HRESULT hr;
    ITypeLib* pITypeLib = NULL;

    //code
    if (m_pITypeInfo == NULL)
    {
        hr = LoadRegTypeLib(LIBID_AutomationServer, 1, 0, 0x00, &pITypeLib);
        if (FAILED(hr))
        {
            ComErrorDescriptionString(NULL, hr);
            return hr;
        }

        hr = pITypeLib->GetTypeInfoOfGuid(IID_IMyMath, &m_pITypeInfo);

        if (FAILED(hr))
        {
            ComErrorDescriptionString(NULL, hr);
            pITypeLib->Release();
            return hr;
        }

        pITypeLib->Release();
    }
    return S_OK;
}

//Implementation of CMyMathClassFactory's Contructor method
CMyMathClassFactory::CMyMathClassFactory()
{
    m_cRef = 1;
}

//Implementation of CMyMathClassFactory's destructor
CMyMathClassFactory::~CMyMathClassFactory()
{
}

//Implementation of CMyMathClassFactory's IUnknown methods
HRESULT CMyMathClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else if (riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG CMyMathClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMyMathClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

//Implementation of CMyMathClassFactory's IClassFactory's methods

HRESULT CMyMathClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    //variables
    CMyMath *pCMyMath = NULL;
    HRESULT hr;

    if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;

    //create instance of component i.e. of CMyMath class
    pCMyMath = new CMyMath;
    if (pCMyMath == NULL)
        return E_OUTOFMEMORY;

    //call automation related init method
    pCMyMath->InitInstance();

    //get requested interface
    hr = pCMyMath->QueryInterface(riid, ppv);

    pCMyMath->Release(); //anticipate possible failure of QueryInterface
    return hr;
}

HRESULT CMyMathClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return S_OK;
}

//Implementation of IDispatch's methods
HRESULT CMyMath::GetTypeInfoCount(UINT *pCountTypeInfo)
{
    //as we have type library, we will return 1, else we would have returned 0
    *pCountTypeInfo = 1;
    return S_OK;
}

HRESULT CMyMath::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)
{
    *ppITypeInfo = NULL;
    if (iTypeInfo != 0)
        return DISP_E_BADINDEX;
    m_pITypeInfo->AddRef();
    *ppITypeInfo = m_pITypeInfo;
    return S_OK;
}

HRESULT CMyMath::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
    return DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId);
}

HRESULT CMyMath::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
     HRESULT hr;
    hr = DispInvoke(this, m_pITypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
    return hr;
}

//Exporte funtions from this Dll
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    //variable declarations
    CMyMathClassFactory *pCMyMathClassFactory = NULL;
    HRESULT hr;
    if (rclsid != CLSID_MyMath)
        return CLASS_E_CLASSNOTAVAILABLE;
    pCMyMathClassFactory = new CMyMathClassFactory();
    if (pCMyMathClassFactory == NULL)
        return E_OUTOFMEMORY;
    hr = pCMyMathClassFactory->QueryInterface(riid, ppv);
    pCMyMathClassFactory->Release();//anticipate possible failure of QueryInterface()
    return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
    if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return S_OK;
    else
        return S_FALSE;
}

void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
    //variable declarations

    wchar_t* szErrorMessage = NULL;
    wchar_t str[255];
    if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
    {
        hr = HRESULT_CODE(hr);
    }
        if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&szErrorMessage, 0, NULL) != 0)
        {
            swprintf_s(str, L"%#x : %s", hr, szErrorMessage);
            LocalFree(szErrorMessage);
        }
        else
            swprintf_s(str, L"[Could not find a description for error # %#x.]\n", hr);
        MessageBoxW(hwnd, str, L"Com error", MB_OK);
}

//register COM Dll into system registry
STDAPI DllRegisterServer()
{
    //not implemented
    return E_FAIL;
}

STDAPI DllUnregisterServer()
{
    //not implemented
    return E_FAIL;
}