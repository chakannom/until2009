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
	//프로그램 중복 실행 방지(뮤택스 사용)
	HANDLE hMutex=CreateMutex(NULL, FALSE, "SUN-PC_CLIENT-58BF3D18-A4CC-49a4-9C90-81F86420A546");
	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex);
		MessageBox(NULL, "SUN-PC_CLIENT가 이미 실행중입니다.", "ERROR", MB_OK);
		hMutex = NULL;
		return 0;
	}
	hInst = hInstance;
	//현재 파일위치를 FilePath변수에 저장함
	GetModuleFileName(NULL, FPath.filepath, MAX_PATH);
	_splitpath_s(FPath.filepath, FPath.drv, FPath.path, FPath.fname, FPath.ext);
	sprintf_s(FPath.drv_path, "%s%s", FPath.drv, FPath.path);
	SetCurrentDirectory(FPath.drv_path);  //프로그램의 현재 디렉토리를 세팅함
	//윈속 초기화
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) {
		MessageBox(NULL, "윈속 초기화 실패!", "ERROR", MB_OK);
		return 0;
	}
	//Process Lock Device 인스통 & 시작
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
			DisplayText("PcssLock 드라이버 연결 실패\n");
			return 1;
		}
		
		bRet = DeviceIoControl(hDriverLockFile, IOCTL_PCSSLOCK_PCSSINFO, &hPcssID, sizeof(HANDLE), NULL, 0, &dwBytesReturned, NULL);
		if(bRet == FALSE) {
			DisplayText("PcssLock 드라이버로 프로세스ID 보내기 실패\n");
			CloseHandle(hDriverLockFile);
			return 1;
		}
		CloseHandle(hDriverLockFile);
	} else MessageBox(NULL, "프로세스 락 디바이스 설치 실패!", "ERROR", MB_OK);

	//Process Monitor Device 인스통 & 시작
	GetCurrentDirectory(_MAX_PATH, DeviceExe[1]);
	strcat_s(DeviceExe[1], "\\i386\\");
	strcat_s(DeviceExe[1], DeviceName_Monitor);
	if(DriverInstall(DeviceName_Monitor, DeviceExe[1])) {
		DriverStart(DeviceName_Monitor);
		bRetInstall[1] = TRUE;
	} else MessageBox(NULL, "프로세스 모니터 디바이스 설치 실패!", "ERROR", MB_OK);

	//임계영역 초기화
	InitializeCriticalSection(&cs);
	//GDI+ 라이브러리를 초기화
	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if(GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) {
		MessageBox(NULL, "GDI+ 라이브러리를 초기화할 수 없습니다.", "알림", MB_OK);
		return 0;
	}
	//HOOK 설정 부분(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC 방지 설정)
	TaskSwitching_Control(TRUE);
	//다이얼로그 생성 및 실행
	DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVIEW_DLG), NULL, ClientInitViewDlgProc);
	//HOOK 해제 부분(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC 방지 해제)
	TaskSwitching_Control(FALSE);
	//GDI+ 라이브러리 종료
	GdiplusShutdown(gpToken);
	//윈속 종료
	closesocket(ClientSocket);
	WSACleanup();
	//임계영역 정리
	DeleteCriticalSection(&cs);
	//Process Monitor Device 정지 & 제거
	if(bRetInstall[1]) {
		CloseHandle(hProcessEvent);
		CloseHandle(hDriverMonFile);
		DriverStop(DeviceName_Monitor);
		DriverRemove(DeviceName_Monitor, DeviceExe[1]);
		bRetInstall[1] = FALSE;
	}
	//Process Lock Device 정지 & 제거
	if(bRetInstall[0]) {
		DriverStop(DeviceName_Lock);
		DriverRemove(DeviceName_Lock, DeviceExe[0]);
		bRetInstall[0] = FALSE;
	}
	return 0;
}
//클라이언트 INITVIEW 프로시져
BOOL CALLBACK ClientInitViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:  //다이얼로그 생성시
		{
			return ClientInitViewDlgInit(hDlg);
		}
		case WM_COMMAND:  //명령이 있을시
		{
			return ClientInitViewDlgCommand(hDlg, wParam, lParam);
		}
		case WM_PAINT:  //이미지 출력시
		{
			//INITVIEW 배경화면 설정
			return ClientInitViewDlgPaint(hDlg, wParam, lParam);
		}
		case WM_MYCLOSE:  //다이얼로그 닫을시
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
	//해상도에 맞춰서 전체화면 윈도우를 만듬
	MoveWindow(hDlg, 0, 0, SCR_Width, SCR_Height, TRUE);

	//ID와 PW 컨트롤러 설정
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
	//서버IP주소를 파일로부터 읽어들임
	FileRead("serveraddress.txt", IPAddr, 20);
	//프로그램 진행 윈도우를 생성함
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
//INITVIEW 배경화면 설정
BOOL ClientInitViewDlgPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Image image(L"SUNPCBGC.GIF");
	// 에러 처리
	if(image.GetLastStatus() != Ok) {
		MessageBox(hClientInitViewDlg, "이미지 파일을 읽을 수 없습니다.", "에러", MB_OK);
		return FALSE;
	}
	graphics.DrawImage(&image, 0, 0, SCR_Width, SCR_Height);
	EndPaint(hWnd, &ps);
	return TRUE;
}
//클라이언트 다이얼로그 프로시져
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
			//TrayIcon 파괴 부분
			NOTIFYICONDATA nid = {0};
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hDlg;
			nid.uID = 1;
			Shell_NotifyIcon(NIM_DELETE, &nid);
			//프로그램 종료
			EndDialog(hClientInitViewDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL ClientDlgInit(HWND hDlg) {
	Point ButtonSize((int)((ClientDlgSize.X-20)/3), (int)(ClientDlgSize.Y*0.063));

	hClientDlg = hDlg;
	//클라이언트 다이얼로그 적절한 크기로 만듬
	SetWindowState(hDlg, hClientInitViewDlg, FALSE, ClientDlgSize.X, ClientDlgSize.Y);
	//System Menu 중 닫기메뉴 이름 수정
	ModifyMenu(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_BYCOMMAND, SC_CLOSE, "트레이 아이콘");
	//아이콘 수정
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
	//AutoRunOn&Off Button 중 어떤 것을 나타낼지를 선택함(레지스트에 값이 있는 경우 On을, 없는 경우 Off를)
	if(RegReadString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName, NULL, 0)) {
		ShowWindow(hBTAutoRunOn, SW_HIDE);
	} else {
		ShowWindow(hBTAutoRunOff, SW_HIDE);
	}
	//TrayIcon 생성 부분
	NOTIFYICONDATA nid = {0};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CLIENT));  //<-원하는 아이콘 넣기ㅎㅎ
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
	//ProcessMonitor 쓰레드 생성
	hThread = CreateThread(NULL, 0, ProcessMonitor, NULL, 0, &ThreadId);
	if(hThread == NULL) return FALSE;
	CloseHandle(hThread);

	//서버와 연결을 시도하는 쓰레드 생성
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
			//서버IP주소를 설정하는 윈도우 열기
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONNECT_DLG), hDlg, ConnectDlgProc);
			break;
		}
		case IDC_BT_ADMINLOGIN:
		{
			//관리자로그인 윈도우 열기
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ADMINLOGIN_DLG), hDlg, AdminLoginDlgProc);
			break;
		}
		case IDC_BT_ADMINLOGOUT:
		{
			//버튼 비활성화
			EnableWindow(hEditSendMsg, FALSE);
			EnableWindow(hBTSendMsg, FALSE);
			EnableWindow(hBTAddrSet, FALSE);
			EnableWindow(hBTAutoRunOn, FALSE);
			EnableWindow(hBTAutoRunOff, FALSE);
			EnableWindow(hBTProgExit, FALSE);
			ShowWindow(hBTAdminLogout, SW_HIDE);
			ShowWindow(hBTAdminLogin, SW_SHOW);
			//hClientDlg가 hClientInitViewDlg의 Child Window로 돌아감
			SetWindowState(hDlg, hClientInitViewDlg, FALSE,ClientDlgSize.X, ClientDlgSize.Y);

			//hClientInitViewDlg 활성화
			ShowWindow(hClientInitViewDlg, SW_SHOW);
			
			break;
		}
		case IDC_BT_PROGEXIT:
		{
			//프로그램 종료 메시지를 보냄
			SendMessage(hClientInitViewDlg, WM_MYCLOSE, 0, 0);
			break;
		}
		case IDC_BT_AUTORUNON:
		{
			//자동실행 ON을 위해 레지스트리에 등록함
			if(RegAddString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName, FPath.filepath)) {
				ShowWindow(hBTAutoRunOn, SW_HIDE);
				ShowWindow(hBTAutoRunOff, SW_SHOW);
			} else DisplayText("[ERROR] 자동실행 ON 실패!\n");
			break;
		}
		case IDC_BT_AUTORUNOFF:
		{
			//자동실행 OFF를 위해 레지스트리에서 삭제함
			if(RegDeleteString(HKEY_LOCAL_MACHINE, AutoRunReg, ProgName)) {
				ShowWindow(hBTAutoRunOff, SW_HIDE);
				ShowWindow(hBTAutoRunOn, SW_SHOW);
			} else DisplayText("[ERROR] 자동실행 OFF 실패!\n");
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

//연결 다이얼로그 프로시져
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
//관리자로그인 다이얼로그 프로시져
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
			//관리자비밀번호를 파일에서 읽어들임
			FileRead("admin.txt", tPassWD, 14);
			//입력한 값과 읽어들인 값이 같은지 비교
			if(strcmp(PassWD, tPassWD)) {
				MessageBox(hDlg, "비밀번호가 틀립니다.", "로그인 실패", MB_OK);
				SetDlgItemText(hDlg, IDC_EDIT_ADMINLOGIN, "");
				break;
			}
			//버튼 활성화
			EnableWindow(hEditSendMsg, TRUE);
			EnableWindow(hBTSendMsg, TRUE);
			EnableWindow(hBTAddrSet, TRUE);
			EnableWindow(hBTAutoRunOn, TRUE);
			EnableWindow(hBTAutoRunOff, TRUE);
			EnableWindow(hBTProgExit, TRUE);
			ShowWindow(hBTAdminLogout, SW_SHOW);
			ShowWindow(hBTAdminLogin, SW_HIDE);
			//hClientDlg가 독립된 Parent Window가 됨
			SetWindowState(hClientDlg, 0, TRUE, ClientDlgSize.X+5, ClientDlgSize.Y+25);
			//hClientInitViewDlg 비활성화
			ShowWindow(hClientInitViewDlg, SW_HIDE);
			//관리자로그인 다이얼로그 종료 메시지를 보냄
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
		case IDC_BT_ADMINCANCEL:
		{
			//관리자로그인 다이얼로그 종료 메시지를 보냄
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		}
	}
	return TRUE;
}
//Process 감시 함수
DWORD WINAPI ProcessMonitor(LPVOID arg) {

	DWORD dwBytesReturned;
	PCSSMON_CALLBACK_INFO PcssMonCallbackInfo;
	char *tmpName;
	map<int , char*>::iterator it;
	BOOL  bRet;
	
	hDriverMonFile = CreateFile("\\\\.\\dosPcssMon", GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hDriverMonFile == INVALID_HANDLE_VALUE) {
		DisplayText("PcssMon 드라이버 연결 실패\n");
		return 1;
	}

	hProcessEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hProcessEvent == NULL) {
		DisplayText("PcssMon 드라이버를 위한 이벤트 생성 실패\n");
		CloseHandle(hDriverMonFile);
		return 1;
	}

	bRet = DeviceIoControl(hDriverMonFile, IOCTL_PCSSMON_SHARDEVENT, &hProcessEvent, sizeof(HANDLE), NULL, 0, &dwBytesReturned, NULL);
	if(bRet == FALSE) {
		DisplayText("PcssMon 드라이버로 이벤트 보내기 실패\n");
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
//연결 함수
DWORD WINAPI OnConnect(LPVOID arg) {
	//Socket 생성
	ClientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(ClientSocket == INVALID_SOCKET) {
		DisplayText("[ERROR] Socket 생성 실패!\n");
		return 1;
	}else DisplayText("[SUCCESS] Socket 생성 성공!\n");
	//주소 정보 설정
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(IPAddr);
	ServerAddr.sin_port = htons(PortNum);
	DisplayText("서버에 접속중.....\n");

	while(1) {
		//Connect
		if(connect(ClientSocket, (PSOCKADDR)&ServerAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
			DisplayText("[ERROR] Connect 실패!\n");
			ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_addr.s_addr = inet_addr(IPAddr);
			ServerAddr.sin_port = htons(PortNum);
			Sleep(1000);
			continue;
		}else {
			DisplayText("[SUCCESS] Connect 성공!\n");
			break;
		}
	}
	//WSAAsyncSelect 실행
	if(WSAAsyncSelect(ClientSocket, hClientDlg, WM_MYSOCKET_NOTIFY, FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR) {
		DisplayText("[ERROR] WSAAsyncSelect 실패!\n");
		closesocket(ClientSocket);
		return 1;
	}else DisplayText("[SUCCESS] WSAAsyncSelect 성공!\n");

	DisplayText("Connect!!!\n");

	return 0;
}
//SEND,WRITE,CLOSE 발생시 수행하는 함수
BOOL OnMySocketNotify(WPARAM wParam, LPARAM lParam) {
	//임계 영역에 들어감
	EnterCriticalSection(&cs);

	int nEvent = WSAGETSELECTEVENT(lParam);
	if(nEvent == FD_READ) {
		char Buffer[MAXLINE];

		recv(ClientSocket, Buffer, MAXLINE, 0);

		CPacket* pPacket = (CPacket*)GetPacketObject();
		if(pPacket->Unpack(Buffer, MAXLINE)) {
			MessageBox(hClientDlg, "pPacket->Unpack 실패", "ERROR", MB_OK);
			//임계 영역을 빠져나감
			LeaveCriticalSection(&cs);
			return FALSE;
		}
//		DisplayText("pPacket->pHead->key : %s\n", pPacket->pHead->key);
//		DisplayText("pPacket->pHead->code : %d\n", pPacket->pHead->code);
//		DisplayText("pPacket->pHead->cb : %d\n", pPacket->pHead->cb);

		if(pPacket->pHead->cb >= 0) {
			switch(pPacket->GetCode()) {
				case pPacket->login_ack:  //로그인 인증 받음
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
					MessageBox(hClientDlg, "아이디 또는 비밀번호를 확인해보시오", "ERROR", MB_OK);
					break;
				}
				case pPacket->logout_ack:  //로그아웃 확인 받음
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
					//자동실행 OFF를 위해 레지스트리에서 삭제함
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
					DisplayText("연결된 프로토콜이 없음\n"); 
					break;
				}
			}
		} else DisplayText("데이터 오류 발생\n");

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
			//임계 영역을 빠져나감
			LeaveCriticalSection(&cs);
			return FALSE;
		}
		CloseHandle(hThread);
	}
	//임계 영역을 빠져나감
	LeaveCriticalSection(&cs);
	return TRUE;
}
//TRAYIOCN에서 메시지 발생시 수행하는 함수
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
	//HOOK 설정 부분(WIN key,ALT+F4,ALT+TAB,ALT+ESC,CTRL+ESC,CTRL+SHIFT+ESC 방지 설정)
	TaskSwitching_Control(!Insert);
}
//화면에 출력 함수
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
//연결 해제 함수
void Disconnect() {
	//소켓 닫기
	closesocket(ClientSocket);
	DisplayText("서버와 연결이 끊어짐.%d\n",bLogin);

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
//Process 번호로 Process 이름을 찾아주는 함수
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