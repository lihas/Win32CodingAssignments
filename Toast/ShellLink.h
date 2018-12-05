#pragma once
/*
Copied from - https://gist.github.com/mstroeck/e72e4e4dd6c326c11282
*/
#include <Windows.h>
#include<Psapi.h>
#include <wrl\module.h>
#include<wrl\client.h>
#include <ShlObj_core.h>
#include <iostream>
#include <propvarutil.h>
#include <propkey.h>
#include "Common.h"

using namespace Microsoft::WRL;

HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath);

HRESULT TryCreateShortcut()
{
    wchar_t shortcutPath[MAX_PATH];
    DWORD charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, MAX_PATH);
    HRESULT hr = charWritten > 0 ? S_OK : E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\\sahils.lnk");
        hr = concatError == 0 ? S_OK : E_INVALIDARG;
        if (SUCCEEDED(hr))
        {
            DWORD attributes = GetFileAttributes(shortcutPath);
            bool fileExists = attributes < 0xFFFFFFF;

            if (!fileExists)
            {
                hr = InstallShortcut(shortcutPath);
            }
            else
            {
                hr = S_FALSE;
            }
        }
    }
    return hr;
}

HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath)
{
    wchar_t exePath[MAX_PATH];

    DWORD charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));

    HRESULT hr = charWritten > 0 ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        ComPtr<IShellLink> shellLink;
        hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));

        if (SUCCEEDED(hr))
        {
            hr = shellLink->SetPath(exePath);
            if (SUCCEEDED(hr))
            {
                hr = shellLink->SetArguments(L"");
                if (SUCCEEDED(hr))
                {
                    ComPtr<IPropertyStore> propertyStore;

                    hr = shellLink.As(&propertyStore);
                    if (SUCCEEDED(hr))
                    {
                        PROPVARIANT appIdPropVar;
                        hr = InitPropVariantFromString(AppUserModelId, &appIdPropVar);
                        if (SUCCEEDED(hr))
                        {
                            hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
                            if (SUCCEEDED(hr))
                            {
                                /* hr = propertyStore->Commit();*/
                                std::wstring test = ToastActivatorCLSID;
                                UUID idTest;

                                UuidFromString((RPC_WSTR)test.c_str(), &idTest);
                                PropVariantClear(&appIdPropVar);
                                appIdPropVar.vt = VT_CLSID;
                                //appIdPropVar.puuid = const_cast<CLSID*>(&__uuidof(CToastActivator));
                                appIdPropVar.puuid = const_cast<CLSID*>(&idTest);
                                hr = propertyStore->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, appIdPropVar);
                                if (SUCCEEDED(hr))
                                {
                                    propertyStore->Commit();
                                    ComPtr<IPersistFile> persistFile;
                                    hr = shellLink.As(&persistFile);
                                    if (SUCCEEDED(hr))
                                    {
                                        hr = persistFile->Save(shortcutPath, TRUE);
                                    }
                                }
                            }
                            //PropVariantClear(&appIdPropVar);
                        }
                    }
                }
            }
        }
    }
    return hr;
}
