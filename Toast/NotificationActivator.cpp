#include "NotificationActivator.h"

HRESULT NotificationActivator::Activate(LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA * data, ULONG dataCount)
{
    MessageBoxA(NULL, "NotificationActivator::Activate()", "Info", MB_OK | MB_ICONINFORMATION);
    return S_OK;
}
