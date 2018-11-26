#include "CMPNT1.h"
#include "..\CONTAIN_COMPONENT_Y\ComponentY.h"

class CA : public IX, public IY
{
private:
    long m_cRef;
    IY* m_pIY;
public:
    CA();
    ~CA();
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    HRESULT __stdcall Fx();
    HRESULT __stdcall Fy();

    HRESULT Init();
};

LONG g_cComponents = 0;
LONG g_cServerLocks = 0;

CA::CA()
{
    m_cRef = 1;
    m_pIY = NULL;
    InterlockedIncrement(&g_cComponents);
}

CA::~CA()
{
    InterlockedDecrement(&g_cComponents);

    if (m_pIY)
    {
        m_pIY->Release();
    }

}

HRESULT CA::Init()
{
    HRESULT hr = ::CoCreateInstance(CLSID_CComponentY, NULL, CLSCTX_INPROC_SERVER, IID_IY, (void**)&m_pIY);

    if (FAILED(hr))
    {
        return E_FAIL;
    }
    else
    {
        return S_OK;
    }
}

HRESULT CA::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<IX*>(this);
    }
    else if (riid == IID_IX)
    {
        *ppv = static_cast<IX*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG CA::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CA::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CA::Fx()
{
    MessageBoxA(NULL, "CA::Fx()", "CA::Fx()", MB_OK);
    return S_OK;
}

HRESULT CA::Fy()
{
    MessageBoxA(NULL, "calling m_pIY->Fy()", "CA::Fy()", MB_OK);
    m_pIY->Fy();
    return S_OK;
}

class CAClassFactory : public IClassFactory
{
private:
    long m_cRef;
public:
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    HRESULT __stdcall CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppvObject);
    HRESULT __stdcall LockServer(BOOL fLock);

};

ULONG CAClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CAClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
    }
    return m_cRef;
}

HRESULT CAClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    if (riid != IID_IUnknown)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else if (riid != IID_IClassFactory)
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

// Inherited via IClassFactory
HRESULT __stdcall CAClassFactory::CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppv)
{
    if (pUnkOuter != NULL)
    {
        return CLASS_E_NOAGGREGATION;
    }

    CA* pA = new CA;
    if (pA == NULL)
    {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pA->Init();
    if (FAILED(hr))
    {
        pA->Release();
        return hr;
    }

    hr = pA->QueryInterface(riid, ppv);
    pA->Release();

    return hr;
    
}

HRESULT __stdcall CAClassFactory::LockServer(BOOL fLock)
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
    if (rclsid != CLSID_CA)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CAClassFactory *pCAClassFactory = new CAClassFactory;
    HRESULT hr;
    hr = pCAClassFactory->QueryInterface(riid, ppv);
    pCAClassFactory->Release();
    return hr;
    
}

HRESULT __stdcall DllCanUnloadNow()
{
    if ((g_cComponents == 0) && (g_cServerLocks == 0))
        return S_OK;
    else
        S_FALSE;
}
