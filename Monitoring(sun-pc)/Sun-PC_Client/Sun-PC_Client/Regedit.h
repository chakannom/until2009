#undef UNICODE
#undef _UNICODE

#include <windows.h>

BOOL RegAddString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);  //������Ʈ���� ���ڿ� �߰�
BOOL RegDeleteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue);  //������Ʈ���� ���ڿ� ����
BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize);  //������Ʈ���� ���ڿ� �б�