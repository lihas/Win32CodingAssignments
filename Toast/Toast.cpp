#include <Windows.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include "DesktopNotificationManagerCompat.h"
#include "NotificationActivator.h"
#include <propvarutil.h>
#include <propkey.h>
#include"Common.h"
#include "ShellLink.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global
bool raiseToast = false;
ComPtr<IXmlDocument> doc;

//prototype
int raise(ComPtr<IXmlDocument>& doc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    auto hr = CoInitialize(NULL);
    
    /*{
        TryCreateShortcut();
        CoUninitialize();
        return 0;
    }*/
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library cannot be initialized.\n Program will now exit"), TEXT("Program Error"), MB_OK);
        CoUninitialize();
        exit(0);
    }

    ////Set App user model ID - AUMID
    //if (SetCurrentProcessExplicitAppUserModelID(AppUserModelId) != S_OK)
    //{
    //    MessageBoxA(NULL, "SetCurrentProcessExplicitAppUserModelID() Failed", "Error", MB_OK);
    //    return -1;
    //}

    //SHGetPropertyStoreForWindow();

    hr = DesktopNotificationManagerCompat::RegisterAumidAndComServer(AppUserModelId, __uuidof(NotificationActivator));

    if (hr != S_OK)
    {
        MessageBoxA(NULL, "RegisterAumidAndComServer() Failed", "Error", MB_OK);
        CoUninitialize();
        return -1;
    }

    hr = DesktopNotificationManagerCompat::RegisterActivator();

    if (hr != S_OK)
    {
        MessageBoxA(NULL, "RegisterActivator() Failed", "Error", MB_OK);
        CoUninitialize();
        return -1;
    }

    hr = DesktopNotificationManagerCompat::CreateXmlDocumentFromString(
        LR"(<?xml version="1.0"?><toast><visual><binding template='ToastGeneric'><text>Hello World</text></binding></visual><actions><input id = "tbReply" type="text" placeHolderContent="Type a response" /><action content = "Reply" arguments="action=reply&amp;conversationId=384928" activationType="background" hint-inputId="tbReply" /></actions></toast>)"
        , &doc);

    if (SUCCEEDED(hr))
    {
        raiseToast = true;
    }
    else
    {
        MessageBoxA(NULL, "CreateXmlDocumentFromString() Failed", "Error", MB_OK);
        CoUninitialize();
        return -1;
    }

    TCHAR appClassName[] = TEXT("lihasToast");
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = appClassName;
    wc.lpszClassName = appClassName;

    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "RegisterClassEx() Failed", "Error", MB_OK);
        CoUninitialize();
        return -1;
    }

    HWND hwnd = CreateWindow(appClassName, appClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return int(msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //Set App user model ID - AUMID
        if (SetCurrentProcessExplicitAppUserModelID(AppUserModelId) != S_OK)
        {
            MessageBoxA(NULL, "SetCurrentProcessExplicitAppUserModelID() Failed", "Error", MB_OK);
            return -1;
        }

        IPropertyStore* pps;
        HRESULT hr = SHGetPropertyStoreForWindow(hwnd, IID_IPropertyStore, (void**)&pps);
        if (FAILED(hr))
        {
            MessageBoxA(NULL, "WM_CREATE - SHGetPropertyStoreForWindow() failed", "Error", MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }
        else
        {
            PROPVARIANT pv;
            hr = InitPropVariantFromString(ToastActivatorCLSID, &pv);
            if (SUCCEEDED(hr))
            {
                pps->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, pv);
                raise(doc);
                PropVariantClear(&pv);
            }
            pps->Release();
        }
    }
    break;
    case WM_DESTROY:
    {
        IPropertyStore* pps;
        HRESULT hr = SHGetPropertyStoreForWindow(hwnd, IID_IPropertyStore, (void**)&pps);
        if (FAILED(hr))
        {
            MessageBoxA(NULL, "WM_DESTROY - SHGetPropertyStoreForWindow() failed", "Error", MB_OK | MB_ICONERROR);
            DestroyWindow(hwnd);
        }
        else
        {
            PROPVARIANT pv;
            PropVariantInit(&pv);
            pps->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, pv);
            PropVariantClear(&pv);
            pps->Release();
        }
        PostQuitMessage(0);
    }
    break;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int raise(ComPtr<IXmlDocument>& doc)
{
    if (!raiseToast)
    {
        MessageBoxA(NULL, "raiseToast is false", "Error", MB_OK);
        return -1;
    }

    ComPtr<IToastNotifier> notifier;
    auto hr = DesktopNotificationManagerCompat::CreateToastNotifier(&notifier);

    if (SUCCEEDED(hr))
    {
        ComPtr<IToastNotification> toast;

        hr = DesktopNotificationManagerCompat::CreateToastNotification(doc.Get(), &toast);
        if (SUCCEEDED(hr))
        {
            hr = notifier->Show(toast.Get());
            if (SUCCEEDED(hr))
            {
                MessageBoxA(NULL, "notifier->Show()", "Success", MB_OK);
            }
            else
            {
                MessageBoxA(NULL, "notifier->Show() Failed", "Error", MB_OK);
                return -1;
            }
        }
        else
        {
            MessageBoxA(NULL, "CreateToastNotification() Failed", "Error", MB_OK);
            return -1;
        }
    }
    else
    {
        MessageBoxA(NULL, "CreateToastNotifier() Failed", "Error", MB_OK);
        return -1;
    }

    return 0;
}

/*
XML from C# example
<?xml version=\"1.0\"?>
<toast launch=\"action=viewConversation&amp;conversationId=384928\">
<visual>
<binding template=\"ToastGeneric\">
<text>Andrew sent you a pciture</text>
<text>Check this out</text>
<image src=\"https://storage.googleapis.com/graphicriver-149805.appspot.com/sreda/AnimatedFire/Test-8.gif\"/>
<image src=\"ms-appdata:///local/StoreLogo.png\" placement=\"appLogoOverride\" hint-crop=\"circle\"/>
</binding>
</visual>
<actions>
<input id=\"tbReply\" type=\"text\" placeHolderContent=\"Type a response\"/>
<action content=\"Reply\" arguments=\"action=reply&amp;conversationId=384928\" activationType=\"background\" imageUri=\"Assets/StoreLogo.png\" hint-inputId=\"tbReply\"/>
<action content=\"Like\" arguments=\"action=like&amp;conversationId=384928\" activationType=\"background\"/>
<action content=\"View\" arguments=\"action=ViewImage&amp;imageUrl=https%3A%2F%2Fstorage.googleapis.com%2Fgraphicriver-149805.appspot.com%2Fsreda%2FAnimatedFire%2FTest-8.gif\"/>
</actions>
</toast>"

*/