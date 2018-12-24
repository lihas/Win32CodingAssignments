cl.exe /c /EHsc /ZW MyApp.cpp
cl.exe /MD MyApp.obj /link vccorlib.lib runtimeobject.lib /SUBSYSTEM:windows /APPCONTAINER
cd MyCertificates
makecert.exe -n "CN=astromedicomp.org" -r -a sha1 -sv astromedicomp.org.pvk astromedicomp.org.cert -ss root REM value of CN should match publisher attribute in identity tag of manifest
makecert.exe -n "CN=astromedicomp.org" -a sha1 -sk astromedicomp.org -iv astromedicomp.org.pvk -ic astromedicomp.org.cert -sr currentuser -ss MyApp
dir cert:\currentuser\MyApp (powershell)
pvk2pfx.exe -pvk astromedicomp.org.pvk -spc astromedicomp.org.cert -pfx astromedicomp.org.pfx
cd ..
copy Assets folder, and Package.appxmanifest file from an existing UWP project
Open manifest, and change - lang to en-us <Resource Language="en-us"/>
mkdir MyPackage
copy MyApp.exe Package.appxmanifest Assets to MyPackage folder
Rename Package.appxmanifest to AppxManifest.xml
makeappx.exe pack /d .\MyPackage /P MyApp.appx REM this will throw file missing errors, for time being make copies of existing files, and rename them to match these files
signtool.exe sign /fd sha256 /f MyCertificates\astromedicomp.org.pfx MyApp.appx