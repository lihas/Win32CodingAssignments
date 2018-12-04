Toast not showing, maybe because PKEY_AppUserModel_ToastActivatorCLSID.
Try setting this with SHGetPropertyStoreForWindow() API.

https://gist.github.com/mstroeck/e72e4e4dd6c326c11282
https://github.com/Squirrel/Squirrel.Windows/blob/master/src/Squirrel/ShellFile.cs
https://docs.microsoft.com/en-us/windows/uwp/design/shell/tiles-and-notifications/send-local-toast-desktop-cpp-wrl
