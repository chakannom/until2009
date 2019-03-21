#include "resource.h"
#include "Sun-PC_Client.h"
#include "FileCtrl.h"
#include "Regedit.h"
#include "DriverMgr.h"
#include "../tasksw/tasksw.h"
#include "../packet/packet.cpp"
extern "C" void* GetMyPacketObject();

/////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	//���α׷� �ߺ� ���� ����(���ý� ���)
	HANDLE hMutex=CreateMutex(NULL, FALSE, "SUN-PC_CLIENT-58BF3D18-A4CC-49a4-9C90-81F86420A546");
	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex);
		MessageBox(NULL, "SUN-PC_CLIENT�� �̹� �������Դϴ�.", "ERROR", MB_OK);
		hMutex = NULL;
		return 0;
	}
	hInst = hInstance;
	//���� ������ġ�� FilePath������ ������
	GetModuleFileName(NULL, FPath.filepath, MAX_PATH);
	_splitpath_s(FPath.filepath, FPath.drv, FPath.path, FPath.fname, FPath.ext);
	sprintf_s(FPath.drv_path, "%s%s", FPath.drv, FPath.path);
	SetCurrentDirectory(FPath.drv_path);  //���α׷��� ���� ���丮�� ������
	//���� �ʱ�ȭ
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) {
		MessageBox(NULL, "���� �ʱ�ȭ ����!", "ERROR", MB_OK);
		return 0;
	}
	//Process Lock Device �ν��� & ����
	GetCurrentDirectory(_MAX_PATH, DeviceExe[0]);
	strcat_s(DeviceExe[0], "\\i386\\");
	strcat_s(DeviceExe[0], DeviceName_Lock);
	if(DriverInstall(DeviceName_Lock, DeviceExe[0])) {
		HANDLE hPcssID = (HANDLE)GetCurrentProcessId();
		HANDLE hDriverLockFile;
		DWORD dwBytesReturned;
		BOOL bRet;

		DriverStart(DeviceName_Lock);
		bRetInstall[0] = TRUE;

		hDriverLockFile = CreateFile("\\\\.\\dosPcssLock", GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(hDriverLockFile == INVALID_HANDLE_VALUE) {
			DisplayText("PcssLock ����̹� ���� ����\n");
			return 1;
		}
		
		bRet = DeviceIoControl(hDriverLockFile, IOCTL_PCSSLOCK_PCSSINFO, &hPcssID, sizeof(HANDLE), NULL, 0, &dwBytesReturned, NULL);
		if(bRet == FALSE) {
			DisplayText("PcssLock ����̹��� ���μ���ID ������ ����\n");
			CloseHandle(hDriverLockFile);
			return 1;
		}
		CloseHandle(hDriverLockFile);
	} else MessageBox(NULL, "���μ��� �� ����̽� ��ġ ����!", "ERROR", MB_OK);

	//Process Monitor Device �ν��� & ����
	GetCurrentDirectory(_MAX_PATH, DeviceExe[1]);
	strcat_s(DeviceExe[1], "\\i386\\");
	strcat_s(DeviceExe[1], DeviceName_Monitor);
	if(DriverInstall(DeviceName_Monitor, DeviceExe[1])) {
		DriverStart(DeviceName_Monitor);
		bRetInstall[1] = TRUE;
	} else MessageBox(NULL, "���μ��� ����� ����̽� ��ġ ����!", "ERROR", MB_OK);

	//�Ӱ迵�� �ʱ�ȭ
	InitializeCriticalSection(&cs);
	//GDI+ ���̺귯���� �ʱ�ȭ
	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if(GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
		MessageBox(NULL, "GDI+ ���̺귯���� �ʱ�ȭ�� �� �����ϴ�.", "�˸�", MB_OK);
		return 0;
	}
	//HOOK ���� �κ�(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC ���� ����)
	TaskSwitching_Control(TRUE);
	//���̾�α� ���� �� ����
	DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVIEW_DLG), NULL, ClientInitViewDlgProc);
	//HOOK ���� �κ�(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC ���� ����)
	TaskSwitching_Control(FALSE);
	//GDI+ ���̺귯�� ����
	GdiplusShutdown(gpToken);
	//���� ����
	closesocket(ClientSocket);
	WSACleanup();
	//�Ӱ迵�� ����
	DeleteCriticalSection(&cs);
	//Process Monitor Device ���� & ����
	if(bRetInstall[1]) {
		CloseHandle(hProcessEvent);
		CloseHandle(hDriverMonFile);
		DriverStop(DeviceName_Monitor);
		DriverRemove(DeviceName_Monitor, DeviceExe[1]);
		bRetInstall[1] = FALSE;
	}
	//Process Lock Device ���� & ����
	if(bRetInstall[0]) {
		DriverStop(DeviceName_Lock);
		DriverRemove(DeviceName_Lock, DeviceExe[0]);
		bRetInstall[0] = FALSE;
	}
	return 0;
}
//Ŭ���̾�Ʈ INITVIEW ���ν���
BOOL CALLBACK ClientInitViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:  //���̾�α� ������
		{
			return ClientInitViewDlgInit(hDlg);
		}
		case WM_COMMAND:  //����� ������
		{
			return ClientInitViewDlgCommand(hDlg, wParam, lParam);
		}
		case WM_PAINT:  //�̹��� ��½�
		{
			//INITVIEW ���ȭ�� ����
			return ClientInitViewDlgPaint(hDlg, wParam, lParam);
		}
		case WM_MYCLOSE:  //���̾�α� ������
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL ClientInitViewDlgInit(HWND hDlg) {
	HWND hEditLoID,hEditLoPW, hBTLogin;
	
	hClientInitViewDlg = hDlg;
	//�ػ󵵿� ���缭 ��üȭ�� �����츦 ����
	MoveWindow(hDlg, 0, 0, SCR_Width, SCR_Height, TRUE);

	//ID�� PW ��Ʈ�ѷ� ����
	hEditLoID = GetDlgItem(hDlg, IDC_EDIT_LOGINID);
	hEditLoPW = GetDlgItem(hDlg, IDC_EDIT_LOGINPW);
	hBTLogin = GetDlgItem(hDlg, IDC_BT_LOGIN);
	MoveWindow(hEditLoID, (int)(SCR_Width*0.75), (int)(SCR_Height*0.70), (int)(SCR_Width*0.11), (int)(SCR_Height*0.04), TRUE);
	MoveWindow(hEditLoPW, (int)(SCR_Width*0.75), (int)(SCR_Height*0.75), (int)(SCR_Width*0.11), (int)(SCR_Height*0.04), TRUE);
	MoveWindow(hBTLogin, (int)(SCR_Width*0.87), (int)(SCR_Height*0.70), (int)(SCR_Width*0.06), (int)(SCR_Height*0.04), TRUE);
	HFONT hFont = CreateFont((int)(SCR_Height*0.025), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH|FF_ROMAN, "Arial");
	SendMessage(hEditLoID, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	SendMessage(hEditLoPW, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	SendMessage(hBTLogin, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	//����IP�ּҸ� ���Ϸκ��� �о����
	FileRead("serveraddress.txt", IPAddr, 20);
	//���α׷� ���� �����츦 ������
	hClientDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIENT_DLG), hDlg, ClientDlgProc);
	ShowWindow(hClientDlg, SW_SHOW);

	return TRUE;
}

BOOL ClientInitViewDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDC_BT_LOGIN:
		{
			LOGIN login;
			CPacket* pPacket = (CPacket*)GetPacketObject();
			GetDlgItemText(hDlg, IDC_EDIT_LOGINID, login.userid, 20);
			GetDlgItemText(hDlg, IDC_EDIT_LOGINPW, login.userpw, 20);
			pPacket->Pack(pPacket->login_req, (char*)&login, (int)sizeof(LOGIN));
			IO_Data.pBuffer = pPacket->GetPacket();
			IO_Data.BufLen = pPacket->GetLength();
			SendMessage(hClientDlg, WM_MYSOCKET_NOTIFY, ClientSocket, WSAMAKESELECTREPLY(FD_WRITE, 0));
			SetDlgItemText(hDlg, IDC_EDIT_LOGINID, "");
			SetDlgItemText(hDlg, IDC_EDIT_LOGINPW, "");
			delete pPacket;
			break;
		}
	}

	return TRUE;
}
//INITVIEW ���ȭ�� ����
BOOL ClientInitViewDlgPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Image image(L"SUNPCBGC.GIF");
	// ���� ó��
	if(image.GetLastStatus() != Ok) {
		MessageBox(hClientInitViewDlg, "�̹��� ������ ���� �� �����ϴ�.", "����", MB_OK);
		return FALSE;
	}
	graphics.DrawImage(&image, 0, 0, SCR_Width, SCR_Height);
	EndPaint(hWnd, &ps);
	return TRUE;
}
//Ŭ���̾�Ʈ ���̾�α� ���ν���
BOOL CALLBACK ClientDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			return ClientDlgInit(hDlg);
		}
		case WM_COMMAND:
		{
			return ClientDlgCommand(hDlg, wParam, lParam);
		}
		case WM_SYSCOMMAND:
		{
			return ClientDlgSysCommand(hDlg, uMsg, wParam, lParam);
		}
		case WM_MYTRAYICON_NOTIFY:
		{
			return OnMyTrayIconNotify(hDlg, wParam, lParam);
		}
		case WM_MYSOCKET_NOTIFY:
		{
			return OnMySocketNotify(wParam, lParam);
		}
		case WM_MYCLOSE:
		{
			//TrayIcon �ı� �κ�
			NOTIFYICONDATA nid = {0};
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hDlg;
			nid.uID = 1;
			Shell_NotifyIcon(NIM_DELETE, &nid);
			//���α׷� ����
			EndDialog(hClientInitViewDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL ClientDlgInit(HWND hDlg) {
	Point ButtonSize((int)((ClientDlgSize.X-20)/3), (int)(ClientDlgSize.Y*0.063));

	hClientDlg = hDlg;
	//Ŭ���̾�Ʈ ���̾�α� ������ ũ��� ����
	SetWindowState(hDlg, hClientInitViewDlg, FALSE, ClientDlgSize.X, ClientDlgSize.Y);
	//System Menu �� �ݱ�޴� �̸� ����
	ModifyMenu(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_BYCOMMAND, SC_CLOSE, "Ʈ���� ������");
	//������ ����
	SetClassLong(hDlg, GCL_HICON, (LONG)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CLIENT)));

	hEditContent = GetDlgItem(hDlg, IDC_EDIT_CONTENT);
	MoveWindow(hEditContent, 5, 5, (int)(ClientDlgSize.X-10), (int)(ClientDlgSize.Y*0.6), TRUE);
	hBTAddrSet = GetDlgItem(hDlg, IDC_BT_ADDRSET);
	MoveWindow(hBTAddrSet, 5, 10+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTAdminLogin = GetDlgItem(hDlg, IDC_BT_ADMINLOGIN);
	MoveWindow(hBTAdminLogin, 10+ButtonSize.X, 10+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTAdminLogout = GetDlgItem(hDlg, IDC_BT_ADMINLOGOUT);
	MoveWindow(hBTAdminLogout, 10+ButtonSize.X, 10+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTProgExit = GetDlgItem(hDlg, IDC_BT_PROGEXIT);
	MoveWindow(hBTProgExit, 15+2*ButtonSize.X, 10+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTAutoRunOn = GetDlgItem(hDlg, IDC_BT_AUTORUNON);
	MoveWindow(hBTAutoRunOn, 5, 40+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTAutoRunOff = GetDlgItem(hDlg, IDC_BT_AUTORUNOFF);
	MoveWindow(hBTAutoRunOff, 5, 40+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	hBTLogout = GetDlgItem(hDlg, IDC_BT_LOGOUT);
	MoveWindow(hBTLogout, 15+2*ButtonSize.X, 40+(int)(ClientDlgSize.Y*0.6), ButtonSize.X, ButtonSize.Y, TRUE);
	//AutoRunOn&Off Button �� � ���� ��Ÿ������ ������(������Ʈ�� ���� �ִ� ��� On��, ���� ��� Off��)
	if(RegReadString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName, NULL, 0)) {
		ShowWindow(hBTAutoRunOn, SW_HIDE);
	} else {
		ShowWindow(hBTAutoRunOff, SW_HIDE);
	}
	//TrayIcon ���� �κ�
	NOTIFYICONDATA nid = {0};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CLIENT));  //<-���ϴ� ������ �ֱ⤾��
	nid.hWnd = hDlg;
	strcpy_s(nid.szTip, ProgName);
	nid.uCallbackMessage = WM_MYTRAYICON_NOTIFY;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uID = 1;
	Shell_NotifyIcon(NIM_ADD, &nid);

	hBTSendMsg = GetDlgItem(hDlg, IDC_BT_SENDMSG);
	MoveWindow(hBTSendMsg, ClientDlgSize.X-90, ClientDlgSize.Y-31, 85, 25, TRUE);
	hEditSendMsg = GetDlgItem(hDlg, IDC_EDIT_SENDMSG);
	MoveWindow(hEditSendMsg, 5, ClientDlgSize.Y-30, ClientDlgSize.X-100, 25, TRUE);

	DWORD ThreadId;
	HANDLE hThread;
	//ProcessMonitor ������ ����
	hThread = CreateThread(NULL, 0, ProcessMonitor, NULL, 0, &ThreadId);
	if(hThread == NULL) return FALSE;
	CloseHandle(hThread);

	//������ ������ �õ��ϴ� ������ ����
	hThread = CreateThread(NULL, 0, OnConnect, NULL, 0, &ThreadId);
	if(hThread == NULL) return FALSE;
	CloseHandle(hThread);

	return TRUE;
}
BOOL ClientDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDC_BT_SENDMSG:
		{
			CHATMSG chatmsg;
			CPacket* pPacket = (CPacket*)GetPacketObject();
			GetDlgItemText(hDlg, IDC_EDIT_SENDMSG, chatmsg.msg, 1004);
			DisplayText("[Send msg] %s\n", chatmsg.msg);
			pPacket->Pack(pPacket->chat_req, (char*)&chatmsg, (int)sizeof(CHATMSG));
			IO_Data.pBuffer = pPacket->GetPacket();
			IO_Data.BufLen = pPacket->GetLength();
			SendMessage(hDlg, WM_MYSOCKET_NOTIFY, ClientSocket, WSAMAKESELECTREPLY(FD_WRITE, 0));
			SetDlgItemText(hDlg, IDC_EDIT_SENDMSG, "");
			delete pPacket;
			break;
		}
		case IDC_BT_ADDRSET:
		{
			//����IP�ּҸ� �����ϴ� ������ ����
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONNECT_DLG), hDlg, ConnectDlgProc);
			break;
		}
		case IDC_BT_ADMINLOGIN:
		{
			//�����ڷα��� ������ ����
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ADMINLOGIN_DLG), hDlg, AdminLoginDlgProc);
			break;
		}
		case IDC_BT_ADMINLOGOUT:
		{
			//��ư ��Ȱ��ȭ
			EnableWindow(hEditSendMsg, FALSE);
			EnableWindow(hBTSendMsg, FALSE);
			EnableWindow(hBTAddrSet, FALSE);
			EnableWindow(hBTAutoRunOn, FALSE);
			EnableWindow(hBTAutoRunOff, FALSE);
			EnableWindow(hBTProgExit, FALSE);
			ShowWindow(hBTAdminLogout, SW_HIDE);
			ShowWindow(hBTAdminLogin, SW_SHOW);
			//hClientDlg�� hClientInitViewDlg�� Child Window�� ���ư�
			SetWindowState(hDlg, hClientInitViewDlg, FALSE,ClientDlgSize.X, ClientDlgSize.Y);

			//hClientInitViewDlg Ȱ��ȭ
			ShowWindow(hClientInitViewDlg, SW_SHOW);
			
			break;
		}
		case IDC_BT_PROGEXIT:
		{
			//���α׷� ���� �޽����� ����
			SendMessage(hClientInitViewDlg, WM_MYCLOSE, 0, 0);
			break;
		}
		case IDC_BT_AUTORUNON:
		{
			//�ڵ����� ON�� ���� ������Ʈ���� �����
			if(RegAddString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName, FPath.filepath)) {
				ShowWindow(hBTAutoRunOn, SW_HIDE);
				ShowWindow(hBTAutoRunOff, SW_SHOW);
			} else DisplayText("[ERROR] �ڵ����� ON ����!\n");
			break;
		}
		case IDC_BT_AUTORUNOFF:
		{
			//�ڵ����� OFF�� ���� ������Ʈ������ ������
			if(RegDeleteString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName)) {
				ShowWindow(hBTAutoRunOff, SW_HIDE);
				ShowWindow(hBTAutoRunOn, SW_SHOW);
			} else DisplayText("[ERROR] �ڵ����� OFF ����!\n");
			break;
		}
		case IDC_BT_LOGOUT:
		{
			CPacket* pPacket = (CPacket*)GetPacketObject();
			pPacket->Pack(pPacket->logout_req);
			IO_Data.pBuffer = pPacket->GetPacket();
			IO_Data.BufLen = pPacket->GetLength();
			SendMessage(hClientDlg, WM_MYSOCKET_NOTIFY, ClientSocket, WSAMAKESELECTREPLY(FD_WRITE, 0));
			delete pPacket;
			break;
		}
	}
	return TRUE;
}
BOOL ClientDlgSysCommand(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case SC_CLOSE:
		{
			bViewDlg = FALSE;
			ShowWindow(hDlg, SW_HIDE);
			break;
		}
		default:
		{
			DefWindowProc(hDlg, uMsg, wParam, lParam);
			break;
		}
	}
	return TRUE;
}

//���� ���̾�α� ���ν���
BOOL CALLBACK ConnectDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			return ConnectDlgInit(hDlg);
		}
		case WM_COMMAND:
		{
			return ConnectDlgCommand(hDlg, wParam, lParam);
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL ConnectDlgInit(HWND hDlg) {
	SetDlgItemText(hDlg, IDC_IPADDRESS, IPAddr);
	SetDlgItemInt(hDlg, IDC_EDIT_PORTNUM, PortNum, FALSE);

	return TRUE;
}
BOOL ConnectDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDC_BT_CONNECTOK:
		{
			GetDlgItemText(hDlg, IDC_IPADDRESS, IPAddr, sizeof(IPAddr));
			PortNum = GetDlgItemInt(hDlg, IDC_EDIT_PORTNUM, NULL, FALSE);
			FileWrite("serveraddress.txt", IPAddr, (DWORD)strlen(IPAddr));
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		case IDC_BT_CONNECTCANCEL:
		{
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
	}
	return TRUE;
}
//�����ڷα��� ���̾�α� ���ν���
BOOL CALLBACK AdminLoginDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			return TRUE;
		}
		case WM_COMMAND:
		{
			return AdminLoginDlgCommand(hDlg, wParam, lParam);
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL AdminLoginDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDC_BT_ADMINOK:
		{
			GetDlgItemText(hDlg, IDC_EDIT_ADMINLOGIN, PassWD, sizeof(PassWD));
			//�����ں�й�ȣ�� ���Ͽ��� �о����
			FileRead("admin.txt", tPassWD, 14);
			//�Է��� ���� �о���� ���� ������ ��
			if(strcmp(PassWD, tPassWD)) {
				MessageBox(hDlg, "��й�ȣ�� Ʋ���ϴ�.", "�α��� ����", MB_OK);
				SetDlgItemText(hDlg, IDC_EDIT_ADMINLOGIN, "");
				break;
			}
			//��ư Ȱ��ȭ
			EnableWindow(hEditSendMsg, TRUE);
			EnableWindow(hBTSendMsg, TRUE);
			EnableWindow(hBTAddrSet, TRUE);
			EnableWindow(hBTAutoRunOn, TRUE);
			EnableWindow(hBTAutoRunOff, TRUE);
			EnableWindow(hBTProgExit, TRUE);
			ShowWindow(hBTAdminLogout, SW_SHOW);
			ShowWindow(hBTAdminLogin, SW_HIDE);
			//hClientDlg�� ������ Parent Window�� ��
			SetWindowState(hClientDlg, 0, TRUE, ClientDlgSize.X+5, ClientDlgSize.Y+25);
			//hClientInitViewDlg ��Ȱ��ȭ
			ShowWindow(hClientInitViewDlg, SW_HIDE);
			//�����ڷα��� ���̾�α� ���� �޽����� ����
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		case IDC_BT_ADMINCANCEL:
		{
			//�����ڷα��� ���̾�α� ���� �޽����� ����
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
	}
	return TRUE;
}
//Process ���� �Լ�
DWORD WINAPI ProcessMonitor(LPVOID arg) {

	DWORD dwBytesReturned;
	PCSSMON_CALLBACK_INFO PcssMonCallbackInfo;
	char *tmpName;
	map<int , char*>::iterator it;
	BOOL  bRet;
	
	hDriverMonFile = CreateFile("\\\\.\\dosPcssMon", GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hDriverMonFile == INVALID_HANDLE_VALUE) {
		DisplayText("PcssMon ����̹� ���� ����\n");
		return 1;
	}

	hProcessEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hProcessEvent == NULL) {
		DisplayText("PcssMon ����̹��� ���� �̺�Ʈ ���� ����\n");
		CloseHandle(hDriverMonFile);
		return 1;
	}

	bRet = DeviceIoControl(hDriverMonFile, IOCTL_PCSSMON_SHARDEVENT, &hProcessEvent, sizeof(HANDLE), NULL, 0, &dwBytesReturned, NULL);
	if(bRet == FALSE) {
		DisplayText("PcssMon ����̹��� �̺�Ʈ ������ ����\n");
		CloseHandle(hProcessEvent);
		CloseHandle(hDriverMonFile);
		return 1;
	}

	while(1) {
		tmpName = new char[40];
		WaitForSingleObject(hProcessEvent, INFINITE);
		bRet = DeviceIoControl(hDriverMonFile, IOCTL_PCSSMON_PROCINFO, 0, 0, &PcssMonCallbackInfo, sizeof(PCSSMON_CALLBACK_INFO), &dwBytesReturned, 0);
		if(PcssMonCallbackInfo.bCreate) {
			strcpy_s(tmpName, 40, FindProcessName(PcssMonCallbackInfo.hProcessID, PcssMonCallbackInfo.hParentID));
			mapProcess.insert(pair<int, char*>((int)PcssMonCallbackInfo.hProcessID, tmpName));
			DisplayText("Pcss Start (PcssID : %d, PcssName : %s)\n", PcssMonCallbackInfo.hProcessID, tmpName);
		} else {
			it = mapProcess.find((int)PcssMonCallbackInfo.hProcessID);
			if(it != mapProcess.end()) {
				strcpy_s(tmpName, 40, it->second);
				DisplayText("Pcss End (PcssID : %d, PcssName : %s)\n", PcssMonCallbackInfo.hProcessID, tmpName);
				delete it->second;
				mapProcess.erase(it);
			} else {

				strcpy_s(tmpName, 40, "Unknown");
				DisplayText("Pcss End (PcssID : %d, PcssName : %s)\n", PcssMonCallbackInfo.hProcessID, tmpName);

			}

		}

		if(bLogin) {
			PROCESS process;
			CPacket* pPacket = (CPacket*)GetPacketObject();
			process.create = PcssMonCallbackInfo.bCreate;
			process.processid = (int)PcssMonCallbackInfo.hProcessID;
			strcpy_s(process.processName, tmpName);
			pPacket->Pack(pPacket->process_req, (char*)&process, (int)sizeof(PROCESS));
			IO_Data.pBuffer = pPacket->GetPacket();
			IO_Data.BufLen = pPacket->GetLength();
			SendMessage(hClientDlg, WM_MYSOCKET_NOTIFY, ClientSocket, WSAMAKESELECTREPLY(FD_WRITE, 0));
			delete pPacket;
		}
		if(!PcssMonCallbackInfo.bCreate) {
			delete tmpName;
		}
	}

	return 0;
}
//���� �Լ�
DWORD WINAPI OnConnect(LPVOID arg) {
	//Socket ����
	ClientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(ClientSocket == INVALID_SOCKET) {
		DisplayText("[ERROR] Socket ���� ����!\n");
		return 1;
	}else DisplayText("[SUCCESS] Socket ���� ����!\n");
	//�ּ� ���� ����
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(IPAddr);
	ServerAddr.sin_port = htons(PortNum);
	DisplayText("������ ������.....\n");

	while(1) {
		//Connect
		if(connect(ClientSocket, (PSOCKADDR)&ServerAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
			DisplayText("[ERROR] Connect ����!\n");
			ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_addr.s_addr = inet_addr(IPAddr);
			ServerAddr.sin_port = htons(PortNum);
			Sleep(1000);
			continue;
		}else {
			DisplayText("[SUCCESS] Connect ����!\n");
			break;
		}
	}
	//WSAAsyncSelect ����
	if(WSAAsyncSelect(ClientSocket, hClientDlg, WM_MYSOCKET_NOTIFY, FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR) {
		DisplayText("[ERROR] WSAAsyncSelect ����!\n");
		closesocket(ClientSocket);
		return 1;
	}else DisplayText("[SUCCESS] WSAAsyncSelect ����!\n");

	DisplayText("Connect!!!\n");

	return 0;
}
//SEND,WRITE,CLOSE �߻��� �����ϴ� �Լ�
BOOL OnMySocketNotify(WPARAM wParam, LPARAM lParam) {
	//�Ӱ� ������ ��
	EnterCriticalSection(&cs);

	int nEvent = WSAGETSELECTEVENT(lParam);
	if(nEvent == FD_READ) {
		char Buffer[MAXLINE];

		recv(ClientSocket, Buffer, MAXLINE, 0);

		CPacket* pPacket = (CPacket*)GetPacketObject();
		if(pPacket->Unpack(Buffer, MAXLINE)) {
			MessageBox(hClientDlg, "pPacket->Unpack ����", "ERROR", MB_OK);
			//�Ӱ� ������ ��������
			LeaveCriticalSection(&cs);
			return FALSE;
		}
//		DisplayText("pPacket->pHead->key : %s\n", pPacket->pHead->key);
//		DisplayText("pPacket->pHead->code : %d\n", pPacket->pHead->code);
//		DisplayText("pPacket->pHead->cb : %d\n", pPacket->pHead->cb);

		if(pPacket->pHead->cb >= 0) {
			switch(pPacket->GetCode()) {
				case pPacket->login_ack:  //�α��� ���� ����
				{
					EnableWindow(hEditSendMsg, TRUE);
					EnableWindow(hBTSendMsg, TRUE);
					EnableWindow(hBTLogout, TRUE);
					EnableWindow(hBTAdminLogin, FALSE);
					SetWindowState(hClientDlg, 0, TRUE, ClientDlgSize.X+5, ClientDlgSize.Y+25);
					ShowWindow(hClientInitViewDlg, SW_HIDE);
					bLogin = TRUE;
					break;
				}
				case pPacket->login_fail:
				{
					MessageBox(hClientDlg, "���̵� �Ǵ� ��й�ȣ�� Ȯ���غ��ÿ�", "ERROR", MB_OK);
					break;
				}
				case pPacket->logout_ack:  //�α׾ƿ� Ȯ�� ����
				{
					EnableWindow(hEditSendMsg, FALSE);
					EnableWindow(hBTSendMsg, FALSE);
					EnableWindow(hBTLogout, FALSE);
					EnableWindow(hBTAdminLogin, TRUE);
					SetWindowState(hClientDlg, hClientInitViewDlg, FALSE, ClientDlgSize.X, ClientDlgSize.Y);
					ShowWindow(hClientInitViewDlg, SW_SHOW);
					bLogin = FALSE;
					break;
				}
				case pPacket->adminpw_req:
				{
					PADMINPW pAdminPW=(PADMINPW)pPacket->pData;
					FileWrite("admin.txt", pAdminPW->passwd, (DWORD)strlen(pAdminPW->passwd));
					break;
				}
				case pPacket->cpcautorunon_req:
				{
					if(RegAddString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName, FPath.filepath)) {
						ShowWindow(hBTAutoRunOn, SW_HIDE);
						ShowWindow(hBTAutoRunOff, SW_SHOW);
					}
					break;
				}
				case pPacket->cpcautorunoff_req:
				{
					//�ڵ����� OFF�� ���� ������Ʈ������ ������
					if(RegDeleteString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName)) {
						ShowWindow(hBTAutoRunOff, SW_HIDE);
						ShowWindow(hBTAutoRunOn, SW_SHOW);
					}
					break;
				}
				case pPacket->cpcshutdown_req:
				{
					PCShutdown();
					break;
				}
				case pPacket->chat_req:
				{
					PCHATMSG pChatMsg=(PCHATMSG)pPacket->pData;
					DisplayText("[From %s] : %s\n","Server",pChatMsg->msg);
					break;
				}
				default:
				{
					DisplayText("����� ���������� ����\n"); 
					break;
				}
			}
		} else DisplayText("������ ���� �߻�\n");

//		DisplayText("pPacket->pTail->key : %s\n", pPacket->pTail->key);
//		DisplayText("pPacket->pTail->cb : %d\n", pPacket->pTail->cb);
	
		delete pPacket;

	}
	else if(nEvent==FD_WRITE) {
		send(ClientSocket, IO_Data.pBuffer, IO_Data.BufLen, 0);
		IO_Data.pBuffer = NULL;
		IO_Data.BufLen = 0;
	}
	else if(nEvent == FD_CLOSE) {
		Disconnect();
		DWORD ThreadId;
		HANDLE hThread = CreateThread(NULL, 0, OnConnect, NULL, 0, &ThreadId);
		if(hThread == NULL) {
			//�Ӱ� ������ ��������
			LeaveCriticalSection(&cs);
			return FALSE;
		}
		CloseHandle(hThread);
	}
	//�Ӱ� ������ ��������
	LeaveCriticalSection(&cs);
	return TRUE;
}
//TRAYIOCN���� �޽��� �߻��� �����ϴ� �Լ�
BOOL OnMyTrayIconNotify(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	if(WSAGETSELECTEVENT(wParam) == 1) {
		switch(WSAGETSELECTEVENT(lParam)) {
			case WM_LBUTTONDBLCLK:
			{
				bViewDlg = TRUE;
				ShowWindow(hDlg, SW_SHOW);
				break;
			}
			case WM_RBUTTONUP:
			{
				MessageBox(hDlg, ProgName, ProgName, MB_OK);
				break;
			}
		}
	}
	return TRUE;
}
void SetWindowState(HWND hChildDlg, HWND hParentDlg, BOOL Insert, int Width, int Height) {
	SetWindowLong(hChildDlg, GWLP_HWNDPARENT, (LONG)hParentDlg);
	LONG style = GetWindowLong(hChildDlg, GWL_STYLE);
	if(Insert) {
		style |= WS_CAPTION;
		SetWindowPos(hChildDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	} else {
		style &= ~WS_CAPTION;
		SetWindowPos(hChildDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	SetWindowLong(hChildDlg, GWL_STYLE,style);
	MoveWindow(hClientDlg, (int)(SCR_Width*0.65), (int)(SCR_Height*0.05), Width, Height, TRUE);
	//HOOK ���� �κ�(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC ���� ����)
	TaskSwitching_Control(!Insert);
}
//ȭ�鿡 ��� �Լ�
static void DisplayText(char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);

	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf, fmt,arg);
	
	int nLength = GetWindowTextLength(hEditContent);
	SendMessage(hEditContent, EM_SETSEL, nLength, nLength);
	SendMessage(hEditContent, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

	va_end(arg);
}
//���� ���� �Լ�
void Disconnect() {
	//���� �ݱ�
	closesocket(ClientSocket);
	DisplayText("������ ������ ������.%d\n",bLogin);

	if(bLogin) {
		bLogin = FALSE;
		if(!bViewDlg) {
			bViewDlg = TRUE;
			ShowWindow(hClientDlg, SW_SHOW);
		}
		EnableWindow(hEditSendMsg, FALSE);
		EnableWindow(hBTSendMsg, FALSE);
		EnableWindow(hBTLogout, FALSE);
		EnableWindow(hBTAdminLogin, TRUE);
		SetWindowState(hClientDlg, hClientInitViewDlg, FALSE, ClientDlgSize.X, ClientDlgSize.Y);
		Sleep(1);
		ShowWindow(hClientInitViewDlg, SW_SHOW);
	}
}
//Process ��ȣ�� Process �̸��� ã���ִ� �Լ�
char *FindProcessName(HANDLE ProcessID, HANDLE ParentID) {
	HANDLE hSnapshot;
	PROCESSENTRY32 pEntry;
	char flag;

	hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	pEntry.dwSize=sizeof(pEntry);
	flag=Process32First(hSnapshot, &pEntry);

	while(flag) {
		if((pEntry.th32ProcessID == (DWORD)ProcessID) && (pEntry.th32ParentProcessID == (DWORD)ParentID)) return pEntry.szExeFile;
		flag=Process32Next(hSnapshot, &pEntry);
	}

	return "Unknown";
}
void PCShutdown() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
	LookupPrivilegeValue(NULL,"SeShutdownPrivilege",&luid);

	tp.PrivilegeCount=1;
	tp.Privileges[0].Luid=luid;
	tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken,FALSE,&tp,0,NULL,NULL);

	ExitWindowsEx(EWX_POWEROFF,0);
}