#pragma once
#include "Windows.h"
#include "DesktopNotificationManagerCompat.h"
#include <windows.ui.notifications.h>
#include <NotificationActivationCallback.h>

using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;

class DECLSPEC_UUID("{AB72300E-401A-4F44-B337-16CECAC4D406}") NotificationActivator WrlSealed WrlFinal : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback>
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(_In_ LPCWSTR appUserModelId, _In_ LPCWSTR invokedArgs, _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data, ULONG dataCount) override;
};

CoCreatableClass(NotificationActivator);

