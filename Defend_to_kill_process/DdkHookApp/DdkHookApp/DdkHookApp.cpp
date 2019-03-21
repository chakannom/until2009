#undef UNICODE
#undef _UNICODE

#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "tlhelp32.h"
#include "DriverMgr.h"

BOOL CALLBACK DdkHookAppDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
void PSList();

HINSTANCE hInst;
HWND hDdkHookAppDlg,hListPS,hEditPS,hEditState;
HANDLE handle;
char FullDeviceName[100]={"\\\\.\\ddkhook\\PSNUM"};
char DriverName[]={"ddkhook.sys"};
char DriverExe[_MAX_PATH];
BOOL bInstall=FALSE,bStart=FALSE;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	hInst=hInstance;
	GetCurrentDirectory(_MAX_PATH,DriverExe);
	strcat_s(DriverExe,"\\ddkhook.sys");
	DialogBox(hInst,MAKEINTRESOURCE(IDD_DDKHOOKAPP_DLG),NULL,DdkHookAppDlgProc);
	return 0;
}

BOOL CALLBACK DdkHookAppDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	int n=0;
	LVCOLUMN COL;
	BOOL RetBOOL;
	switch(uMsg) {
		case WM_INITDIALOG:
			hDdkHookAppDlg=hDlg;
			hListPS=GetDlgItem(hDlg,IDC_LIST_PS);
			hEditPS=GetDlgItem(hDlg,IDC_EDIT_PS);
			hEditState=GetDlgItem(hDlg,IDC_EDIT_STATE);
			COL.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
			COL.fmt=LVCFMT_LEFT; //왼쪽 정렬
			COL.cx=150;
			COL.pszText="ProcName"; //항목의 이름
			COL.iSubItem=0; //항목의 번호
			SendMessage(hListPS, LVM_INSERTCOLUMN,COL.iSubItem,(LPARAM)&COL);
			COL.cx=50;
			COL.pszText="ProcID"; //항목의 이름
			COL.iSubItem=1; //항목의 번호
			SendMessage(hListPS, LVM_INSERTCOLUMN,COL.iSubItem,(LPARAM)&COL);
			PSList();
			return TRUE;
		case WM_NOTIFY:
			switch(LOWORD(wParam)) {
				case IDC_LIST_PS:
					if(((LPNMHDR)lParam)->code==NM_DBLCLK) {
						char Text1[32]={0},Text2[32]={0},Temp[128]={0};
						ULONG ret;
						int iSlected=ListView_GetNextItem(hListPS,-1,LVNI_FOCUSED);
						if(iSlected==-1) {
							MessageBox(hDlg,"No Items in ListView","Error",MB_OK|MB_ICONINFORMATION);
							break;
						}
						ListView_GetItemText(hListPS,iSlected,0,Text1,32);
						strcpy_s(Temp,Text1);
						ListView_GetItemText(hListPS,iSlected,1,Text1,32);
						sprintf_s(Text2,"(%s): %s",Text1,"프로세스강제종료막음");
						strcat_s(Temp,Text2);
						SetWindowText(hEditPS,Temp);
						//-----------------------------
						handle=CreateFile(FullDeviceName,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
						if(handle==(HANDLE)-1) {
							SetWindowText(hEditPS,"");
							MessageBox(hDlg,"CreateFile Error","Error",MB_OK);
							break;
						}
						WriteFile(handle,Text1,(DWORD)strlen(Text1),&ret,NULL);
						CloseHandle(handle);
					}
					break;
			}
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BT_REVIEW:
					ListView_DeleteAllItems(hListPS);
					PSList();
					break;
				case IDC_BT_DRINST:
					RetBOOL=DriverInstall(DriverName,DriverExe);
					if(!RetBOOL) SetWindowText(hEditState,"Cannot install");
					else {
						bInstall=TRUE;
						RetBOOL=DriverStart(DriverName);
						if(!RetBOOL) SetWindowText(hEditState,"Cannot start");
						else {
							bStart=TRUE;
							SetWindowText(hEditState,"Started");
						}
					}
					break;
				case IDC_BT_DRREM:
					if(bInstall) {
						RetBOOL=DriverStop(DriverName);
						if(!RetBOOL) SetWindowText(hEditState,"Cannot stop");
						else {
							bStart=FALSE;
							RetBOOL=DriverRemove(DriverName,DriverExe);
							if(!RetBOOL) SetWindowText(hEditState,"Cannot remove");
							else {
								bInstall=FALSE;
								SetWindowText(hEditState,"Removed");
							}
						}
					}
					else SetWindowText(hEditState,"Cannot remove");
					break;
			}
			return TRUE;
		case WM_CLOSE:
			if(bStart) DriverStop(DriverName);
			if(bInstall) DriverRemove(DriverName,DriverExe);
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}

void PSList() {
	HANDLE hSnapshot;
	PROCESSENTRY32 pEntry;
	char flag;
	char procID[10];
	LVITEM LI;
	int ListCnt=0;

	hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);
	pEntry.dwSize=sizeof(pEntry);
	flag=Process32First(hSnapshot,&pEntry);

	while(flag) {
		ListCnt=ListView_GetItemCount(hListPS);
		LI.mask=LVIF_TEXT|LVIF_PARAM;
		LI.iItem=ListCnt;
		LI.iSubItem=0;
		LI.pszText=pEntry.szExeFile;
		ListView_InsertItem(hListPS,&LI);
		sprintf_s(procID,"%d",pEntry.th32ProcessID);
		ListView_SetItemText(hListPS,ListCnt,1,procID);
	
		flag=Process32Next(hSnapshot,&pEntry);
	}
	
	CloseHandle(hSnapshot);
	ListView_SetExtendedListViewStyle(hListPS,LVS_EX_FULLROWSELECT);
}