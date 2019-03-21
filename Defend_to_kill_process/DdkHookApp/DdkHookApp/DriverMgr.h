#undef UNICODE
#undef _UNICODE

#include <windows.h>
#include <stdio.h>

BOOL DriverInstall(LPCTSTR DriverName,LPCTSTR DriverExe);
BOOL DriverRemove(LPCTSTR DriverName,LPCTSTR DriverExe);
BOOL DriverStart(LPCTSTR DriverName);
BOOL DriverStop(LPCTSTR DriverName);