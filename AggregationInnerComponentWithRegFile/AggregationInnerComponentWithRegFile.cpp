#include "AggregationInnerComponentWithRegFile.h"

interface INoAggregationIUnknown
{
    virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) = 0;
    virtual ULONG __stdcall AddRef_NoAggregation() = 0;
    virtual ULONG __stdcall Release_NoAggregation() = 0;
};

class CMultiplicationDivision : public INoAggregationIUnknown, IMultiplication, IDivision
{
private:
    long m_cRef;
    IUnknown *m_pIunknownOuter;
public:
    CMultiplicationDivision(IUnknown*);
    ~CMultiplicationDivision();

    //Aggregation IUnknown
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //No Aggregation IUnknown
    HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**);
    ULONG __stdcall AddRef_NoAggregation();
    ULONG __stdcall Release_NoAggregation();

    //IMultiplication
    HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);
    //IDivision
    HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);
};

class CMultiplicationDivisionClassFactory: public IClassFactory
{
private:
    long m_cRef;
public:
    CMultiplicationDivisionClassFactory();
    ~CMultiplicationDivisionClassFactory();

    //IUnknown
    HRESULT __stdcall QueryInterface(REFIID, void** ppv);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IClassFactory
    HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void **);
    HRESULT __stdcall LockServer(BOOL);
};

//global variables
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    return TRUE;
}

CMultiplicationDivision::CMultiplicationDivision(IUnknown *pIUnknownOuter)
{
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
    if (pIUnknownOuter != NULL)
    {
        m_pIunknownOuter = pIUnknownOuter;
    }
    else
    {
        m_pIunknownOuter = reinterpret_cast<IUnknown*>(static_cast<INoAggregationIUnknown*>(this));
    }
}

CMultiplicationDivision::~CMultiplicationDivision()
{
    InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void** ppv)
{
    return m_pIunknownOuter->QueryInterface(riid, ppv);
}

ULONG CMultiplicationDivision::AddRef()
{
    return m_pIunknownOuter->AddRef();
}

ULONG CMultiplicationDivision::Release()
{
    return m_pIunknownOuter->Release();
}

HRESULT CMultiplicationDivision::QueryInterface_NoAggregation(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<INoAggregationIUnknown*>(this);
    }
    else if(riid == IID_IMultiplication)
    {
        *ppv = static_cast<IMultiplication*>(this);
    }
    else if (riid == IID_IDivision)
    {
        *ppv = static_cast<IDivision*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return S_OK;
}

ULONG CMultiplicationDivision::AddRef_NoAggregation()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMultiplicationDivision::Release_NoAggregation()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
    *pMultiplication = num1 * num2;
    return S_OK;
}

HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
    *pDivision = num1 / num2;
    return S_OK;
}

CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory()
{
    m_cRef = 1;
}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory()
{

}

HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void** ppv)
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

    reinterpret_cast<IUnknown *>(*ppv)->AddRef();
    return S_OK;
}

ULONG CMultiplicationDivisionClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CMultiplicationDivisionClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }

    return m_cRef;
}

HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    CMultiplicationDivision *pCMultiplicationDivision = NULL;
    HRESULT hr;

    if ((pUnkOuter != NULL) && (riid != IID_IUnknown))
    {
        return CLASS_E_NOAGGREGATION;
    }

    pCMultiplicationDivision = new CMultiplicationDivision(pUnkOuter);
    if (pCMultiplicationDivision == NULL)
    {
        return E_OUTOFMEMORY;
    }

    hr = pCMultiplicationDivision->QueryInterface_NoAggregation(riid, ppv);
    pCMultiplicationDivision->Release_NoAggregation();
    return hr;
}

HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&glNumberOfServerLocks);
    }
    else
    {
        InterlockedDecrement(&glNumberOfServerLocks);
    }

    return S_OK;
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    CMultiplicationDivisionClassFactory* pCMultiplicationDivisionClassFactory = NULL;

    HRESULT hr;

    if (rclsid != CLSID_MultiplicationDivision)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory();
    if (pCMultiplicationDivisionClassFactory == nullptr)
    {
        E_OUTOFMEMORY;
    }

    hr = pCMultiplicationDivisionClassFactory->QueryInterface(riid, ppv);
    pCMultiplicationDivisionClassFactory->Release();

    return hr;
}

HRESULT __stdcall DllCanUnloadNow(void)
{
    if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}
