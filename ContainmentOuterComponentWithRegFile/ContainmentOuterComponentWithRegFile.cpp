#define UNICODE
#include<Windows.h>
#include"..\ContainmentInnerComponentWithRegFile\ContainmentInnerComponentWithRegFile.h"
#include "ContainmentOuterComponentWithRegFile.h"

//class declarations
class CSumSubtract : public ISum, ISubtract, IMultiplication, IDivision
{
private:
    long m_cRef;
    IMultiplication *m_pIMultiplication;
    IDivision *m_pIDivision;
public:
    //constructor
    CSumSubtract();
    ~CSumSubtract();

    //IUnknown method declarations
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //ISum method declarations
    HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
    //ISubtract method declarations
    HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
    //IMultiplication method declarations
    HRESULT  __stdcall MultiplicationOfTwoIntegers(int, int, int*);
    //IDivision method declarations
    HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);

    //method for inner component creation
    HRESULT __stdcall InitializeInnerComponent();
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
    long m_cRef;
public:
    //constructor
    CSumSubtractClassFactory();
    //destructor
    ~CSumSubtractClassFactory();
    //IUnknown method declaration
    HRESULT __stdcall QueryInterface(REFIID, void**);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();

    //IClassFactory method declarations
    HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
    HRESULT __stdcall LockServer(BOOL);
};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}

CSumSubtract::CSumSubtract()
{
    m_pIMultiplication = NULL;
    m_pIDivision = NULL;
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract()
{
    InterlockedDecrement(&glNumberOfActiveComponents);
    if (m_pIMultiplication)
    {
        m_pIMultiplication->Release();
        m_pIMultiplication = NULL;
    }
    if (m_pIDivision)
    {
        m_pIDivision->Release();
        m_pIDivision = NULL;
    }
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IUnknown)
    {
        *ppv = static_cast<ISum*>(this);
    }
    else if (riid == IID_ISum)
    {
        *ppv = static_cast<ISum*>(this);
    }
    else if (riid == IID_ISubtract)
    {
        *ppv = static_cast<ISubtract*>(this);
    }
    else if (riid == IID_IMultiplication)
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
    return(S_OK);
}

ULONG CSumSubtract::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CSumSubtract::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
    *pSum = num1 + num2;
    return S_OK;
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
    *pSubtract = num1 - num2;
    return S_OK;
}

HRESULT CSumSubtract::MultiplicationOfTwoIntegers(int num1, int num2, int* pMultiplication)
{
    m_pIMultiplication->MultiplicationOfTwoIntegers(num1, num2, pMultiplication);
    return S_OK;
}

HRESULT CSumSubtract::DivisionOfTwoIntegers(int num1, int num2, int* pDivision)
{
    m_pIDivision->DivisionOfTwoIntegers(num1, num2, pDivision);
    return S_OK;
}

HRESULT CSumSubtract::InitializeInnerComponent()
{
    HRESULT hr;
    hr = CoCreateInstance(CLSID_MultiplicationDivision, NULL, CLSCTX_INPROC_SERVER, IID_IMultiplication, (void**)&m_pIMultiplication);

    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("IMultiplication interface cannot be obtained from inner component"), TEXT("Error"), MB_OK);
        return E_FAIL;
    }

    hr = m_pIMultiplication->QueryInterface(IID_IDivision, (void**)&m_pIDivision);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("IDivision interface cannot be obtained from inner component"), TEXT("Error"), MB_OK);
        return E_FAIL;
    }
    return S_OK;
}

CSumSubtractClassFactory::CSumSubtractClassFactory()
{
    m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory()
{
}

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CSumSubtractClassFactory::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CSumSubtractClassFactory::Release()
{
    InterlockedDecrement(&m_cRef);
    if (m_cRef == 0)
    {
        delete this;
        return 0;
    }
    return m_cRef;
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    //variable declarations
    CSumSubtract *pCSumSubtract = NULL;
    HRESULT hr;

    if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;

    pCSumSubtract = new CSumSubtract;
    if (pCSumSubtract == NULL)
    {
        return E_OUTOFMEMORY;
    }

    hr = pCSumSubtract->InitializeInnerComponent();
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("Failed to initialize inner component"), TEXT("Error"), MB_OK);
        pCSumSubtract->Release();
        return hr;
    }

    hr = pCSumSubtract->QueryInterface(riid, ppv);
    pCSumSubtract->Release();
    return hr;
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else
        InterlockedDecrement(&glNumberOfServerLocks);
    return S_OK;
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
    HRESULT hr;

    if (rclsid != CLSID_SumSubtract)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    pCSumSubtractClassFactory = new CSumSubtractClassFactory;
    if (pCSumSubtractClassFactory == NULL)
    {
        return E_OUTOFMEMORY;
    }
    hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
    pCSumSubtractClassFactory->Release();
    return hr;
}

HRESULT __stdcall DllCanUnloadNow()
{
    if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
        return S_OK;
    else
        return S_FALSE;
}