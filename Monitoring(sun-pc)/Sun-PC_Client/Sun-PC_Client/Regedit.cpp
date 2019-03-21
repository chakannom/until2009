#include "Regedit.h"

//레지스트리에 문자열 추가
BOOL RegAddString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData) {
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS) {
		return FALSE;
	}
	if(RegSetValueEx(key, lpValue, 0, REG_SZ, (LPBYTE)lpData, lstrlen(lpData)+1) != ERROR_SUCCESS) {
		return FALSE;
	}
	RegCloseKey(key);
	return TRUE;
}
//레지스트리에 문자열 삭제
BOOL RegDeleteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue) {
	HKEY key;
	DWORD dwDisp;
	if(RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE,NULL, &key, &dwDisp) != ERROR_SUCCESS) {
		return FALSE;
	}
	if(RegDeleteValue(key, lpValue) != ERROR_SUCCESS) {
		return FALSE;
	}
	RegCloseKey(key);
	return TRUE;
}
//레지스트리에 문자열 읽기
BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize) {
	HKEY key;
	DWORD dwDisp;
	DWORD Size = nSize;
	if(RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp) != ERROR_SUCCESS) {
		return FALSE;
	}
	if(RegQueryValueEx(key, lpValue, 0, NULL, (LPBYTE)lpRet, &Size) != ERROR_SUCCESS) {
		lstrcpy(lpRet,"Read fault");
		return FALSE;
	}
	RegCloseKey(key);
	return TRUE;
}