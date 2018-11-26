#include "ComponentY.h"

class CComponentY : public IY
{
private:
    long m_cRef;
public:
    CComponentY();
    ~CComponentY();

    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    HRESULT __stdcall Fy();
};

long g_cComponents = 0;
long g_cServerLocks = 0;

CComponentY::CComponentY()
{
    m_cRef = 1;
    InterlockedIncrement(&g_cComponents);
}

CComponentY::~CComponentY()
{
    InterlockedDecrement(&g_cComponents);
}

ULONG CComponentY::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CComponentY::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }

    return m_cRef;
}

HRESULT  CComponentY::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IY*>(this);
    }
    else if (riid == IID_IY)
    {
        *ppv = static_cast<IY*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

HRESULT CComponentY::Fy()
{
    MessageBoxA(NULL, "CComponentY::Fy()", "CComponentY::Fy()", MB_OK);
    return S_OK;
}

class CComponentYClassFactory : public IClassFactory
{
private:
    long m_cRef;
public:

    CComponentYClassFactory();
    ~CComponentYClassFactory();
    // Inherited via IClassFactory
    HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject);
    ULONG __stdcall AddRef(void);
    ULONG __stdcall Release(void);
    HRESULT __stdcall CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppvObject);
    HRESULT __stdcall LockServer(BOOL fLock);

};

CComponentYClassFactory::CComponentYClassFactory()
{
    m_cRef = 1;
}

CComponentYClassFactory::~CComponentYClassFactory()
{
}

HRESULT __stdcall CComponentYClassFactory::QueryInterface(REFIID riid, void ** ppv)
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

ULONG __stdcall CComponentYClassFactory::AddRef(void)
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG __stdcall CComponentYClassFactory::Release(void)
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }

    return m_cRef;
}

HRESULT __stdcall CComponentYClassFactory::CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppv)
{
    if (pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    CComponentY *pCComponentY = new CComponentY;
    if (pCComponentY == NULL)
        return E_OUTOFMEMORY;

    HRESULT hr = pCComponentY->QueryInterface(riid, ppv);
    pCComponentY->Release();
    return hr;
}

HRESULT __stdcall CComponentYClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&g_cServerLocks);
    }
    else
    {
        InterlockedDecrement(&g_cServerLocks);
    }

    return S_OK;
    
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    CComponentYClassFactory *pCComponentYClassFactory = NULL;
    HRESULT hr;

    if (rclsid != CLSID_CComponentY)
        return CLASS_E_CLASSNOTAVAILABLE;

    pCComponentYClassFactory = new CComponentYClassFactory;

    hr = pCComponentYClassFactory->QueryInterface(riid, ppv);
    pCComponentYClassFactory->Release();

    return hr;
}

HRESULT __stdcall DllCanUnloadNow()
{
    if ((g_cComponents == 0) && (g_cServerLocks == 0))
        return S_OK;
    else
        S_FALSE;
}