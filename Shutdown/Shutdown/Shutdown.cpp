#include <windows.h>
#include "resource.h"

HINSTANCE hInst;

BOOL CALLBACK ShutdownDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ReBoot();
void PowerOff();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	hInst = hInstance;
	DialogBoxW(hInstance, MAKEINTRESOURCE(IDD_SHUTDOWN_DLG), NULL, ShutdownDlgProc);
	return 0;
}

BOOL CALLBACK ShutdownDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			HICON  hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SHUTDOWN_ICON));
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam)) {
				case IDC_BT_REBOOT:
				{
					ReBoot();
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					break;
				}
				case IDC_BT_POWEROFF:
				{
					PowerOff();
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					break;
				}
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}

void ReBoot() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, L"SeShutdownPrivilege", &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

	ExitWindowsEx(EWX_REBOOT, 0);
}

void PowerOff() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, L"SeShutdownPrivilege", &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

	ExitWindowsEx(EWX_POWEROFF, 0);
}