#include <Windows.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include "DesktopNotificationManagerCompat.h"
#include "NotificationActivator.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

wchar_t AppUserModelId[] = L"lihas.in.toast1";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    auto hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library cannot be initialized.\n Program will now exit"), TEXT("Program Error"), MB_OK);
        exit(0);
    }

    //Set App user model ID - AUMID
    if (SetCurrentProcessExplicitAppUserModelID(AppUserModelId) != S_OK)
    {
        MessageBoxA(NULL, "SetCurrentProcessExplicitAppUserModelID() Failed", "Error", MB_OK);
        return -1;
    }

    //SHGetPropertyStoreForWindow();

    hr = DesktopNotificationManagerCompat::RegisterAumidAndComServer(AppUserModelId, __uuidof(NotificationActivator));

    if (hr != S_OK)
    {
        MessageBoxA(NULL, "RegisterAumidAndComServer() Failed", "Error", MB_OK);
        return -1;
    }

    hr = DesktopNotificationManagerCompat::RegisterActivator();

    if (hr != S_OK)
    {
        MessageBoxA(NULL, "RegisterActivator() Failed", "Error", MB_OK);
        return -1;
    }

    ComPtr<IXmlDocument> doc;
    hr = DesktopNotificationManagerCompat::CreateXmlDocumentFromString(L"<toast><visual><binding template='ToastGeneric'><text>Hello World</text></binding></visual></toast>", &doc);

    if (SUCCEEDED(hr))
    {
        ComPtr<IToastNotifier> notifier;
        hr = DesktopNotificationManagerCompat::CreateToastNotifier(&notifier);

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
        
    }
    else
    {
        MessageBoxA(NULL, "CreateXmlDocumentFromString() Failed", "Error", MB_OK);
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
        return -1;
    }


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
