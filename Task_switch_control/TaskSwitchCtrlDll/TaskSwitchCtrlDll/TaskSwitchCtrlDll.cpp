#undef UNICODE
#undef _UNICODE

#define DLLTYPE
#define  WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0400 

#include <windows.h>
#include <stdlib.h>
#include "TaskSwitchCtrlDll.h"

HINSTANCE hInst;  //Instance Handle
HHOOK hKeyboardHook=NULL;  //Low Level Keyboard Hook
BOOL RegTaskMgr=FALSE;  //������Ʈ���� ����� �Ǿ� �ִ��� Ȯ��

LRESULT CALLBACK LowLevelKeyBoardProc(int nCode,WPARAM wParam,LPARAM lParam);  //Low Level Keyboard ���ν���

BOOL WINAPI DllMain(HINSTANCE hInstDll,DWORD fdwReadon,LPVOID lpReserved) {
	switch(fdwReadon) {
		case DLL_PROCESS_ATTACH:
			hInst=hInstDll;
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
//TaskSwitching_Control �Լ�
extern "C" DLL_EX_IM BOOL TaskSwitching_Control(BOOL Type) {
	if(Type) {
		//���� ������ ��ġ(WH_KEYBOARD_LL�� NT4.0 SP3 ���Ŀ��� ��밡��)
		if(hKeyboardHook==NULL) {
			hKeyboardHook=SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyBoardProc,hInst,0);
			if(hKeyboardHook==NULL) return FALSE;
		}
	}
	else {
		//�� ��ü
		UnhookWindowsHookEx(hKeyboardHook);
		hKeyboardHook=NULL;
	}
	return TRUE;
}
//TaskManager_Control �Լ�
extern "C" DLL_EX_IM BOOL TaskManager_Control(BOOL Type) {
	#define KEY_DISABLETASKMGR "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
	#define VAL_DISABLETASKMGR "DisableTaskMgr"
	HKEY hKey;
	DWORD val,dwDisp,size=sizeof(LONG);
	LONG err;

	RegCreateKeyEx(HKEY_CURRENT_USER,KEY_DISABLETASKMGR,0,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dwDisp);
	err=RegQueryValueEx(hKey,VAL_DISABLETASKMGR,0,NULL,(LPBYTE)&val,&size);

	if(Type) {
		val=1;
		err=RegSetValueEx(hKey,VAL_DISABLETASKMGR,0,REG_DWORD,(BYTE*)&val,sizeof(val));
		if(err!=ERROR_SUCCESS) return FALSE;
	}
	else {
		err=RegDeleteValue(hKey,VAL_DISABLETASKMGR);
		if(err!=ERROR_SUCCESS) return FALSE;
	}
	RegCloseKey(hKey);
	return TRUE;
}
//Low Level Keyboard ���ν���
LRESULT CALLBACK LowLevelKeyBoardProc(int nCode,WPARAM wParam,LPARAM lParam) {
	PKBDLLHOOKSTRUCT p;
	if(nCode==HC_ACTION) {
		p=(PKBDLLHOOKSTRUCT)lParam;
		if((p->vkCode==VK_LWIN)||(p->vkCode==VK_RWIN)) return 1;  //WIN key (for Start Menu)
		else if((p->vkCode==VK_TAB)&&(p->flags&LLKHF_ALTDOWN)) return 1;  //ALT+TAB
		else if((p->vkCode==VK_ESCAPE)&&(p->flags&LLKHF_ALTDOWN)) return 1;  //ALT+ESC
		else if((p->vkCode==VK_ESCAPE)&&((GetAsyncKeyState(VK_CONTROL)&0x8000)!=0)) return 1;  //CTRL+ESC
		else if((p->vkCode==VK_ESCAPE)&&((GetAsyncKeyState(VK_CONTROL)&0x8000)!=0)&&((GetAsyncKeyState(VK_SHIFT)&0x8000)!=0)) return 1;  //CTRL+SHIFT+ESC
	}
	return CallNextHookEx(hKeyboardHook,nCode,wParam,lParam);
}
