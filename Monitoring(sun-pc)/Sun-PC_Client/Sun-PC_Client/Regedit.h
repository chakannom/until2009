#undef UNICODE
#undef _UNICODE

#include <windows.h>

BOOL RegAddString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);  //레지스트리에 문자열 추가
BOOL RegDeleteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue);  //레지스트리에 문자열 삭제
BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize);  //레지스트리에 문자열 읽기