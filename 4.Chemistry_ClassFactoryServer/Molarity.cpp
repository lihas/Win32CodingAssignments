#include "Molarity.h"

class CMolarity : IMolarity
{
private:
    long m_cRef;
public:
    CMolarity();
    ~CMolarity();

    //IUnknown
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IMolarity
    HRESULT __stdcall  CalculateMolarity(unsigned int moles, unsigned int volumeLitre, float* molarity);
};

class CMolarityClassFactory : public IClassFactory
{
private:
    long m_cRef;
public:
    CMolarityClassFactory();
    ~CMolarityClassFactory();

    //IUnknown
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IClass factory
    HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
    HRESULT __stdcall LockServer(BOOL);
};

long glNumberOfAciveComponents = 0;
long glNumerOfServerLocks = 0;

//DLL main
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    switch (dwReason)
    {
    default:
        break;
    }

    return TRUE;
}

CMolarity::CMolarity()
{
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfAciveComponents);
}

CMolarity::~CMolarity()
{
    InterlockedDecrement(&glNumberOfAciveComponents);
}

HRESULT CMolarity::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IMolarity*>(this);
    }
    else if (riid == IID_IMolarity)
    {
        *ppv = static_cast<IMolarity*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG CMolarity::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMolarity::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CMolarity::CalculateMolarity(unsigned int moles, unsigned int volumeLitre, float* molarity)
{
    *molarity = float(moles) / volumeLitre;
    return S_OK;
}

CMolarityClassFactory::CMolarityClassFactory()
{
    m_cRef = 1;
}

CMolarityClassFactory::~CMolarityClassFactory()
{
    m_cRef = 1;
}

HRESULT CMolarityClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CMolarityClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMolarityClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CMolarityClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    CMolarity *pCMolarity = nullptr;
    HRESULT hr;

    if (pUnkOuter != NULL)
    {
        return CLASS_E_NOAGGREGATION;
    }

    pCMolarity = new CMolarity();

    if (pCMolarity == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    hr = pCMolarity->QueryInterface(riid, ppv);
    pCMolarity->Release();
    return hr;
}

HRESULT CMolarityClassFactory::LockServer(BOOL fLock)
{
    if(fLock)
    {
        InterlockedIncrement(&glNumerOfServerLocks);
    }
    else
    {
        InterlockedDecrement(&glNumerOfServerLocks);
    }
    return S_OK;
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    CMolarityClassFactory* pCMolarityClassFactory = nullptr;
    HRESULT hr;

    if (rclsid != CLSID_Molarity)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    pCMolarityClassFactory = new CMolarityClassFactory();
    if (pCMolarityClassFactory == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    hr = pCMolarityClassFactory->QueryInterface(riid, ppv);
    pCMolarityClassFactory->Release();
    return hr;
}

HRESULT __stdcall DllCanUnloadNow()
{
    if (glNumberOfAciveComponents == 0 & glNumerOfServerLocks == 0)
    {
        return S_OK;
    }
    else
    {
        S_FALSE;
    }
}