#undef UNICODE
#undef _UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <winioctl.h>
#include <gdiplus.h>
#include "tlhelp32.h"
#include <map>
#include <string>

using namespace std;
using namespace Gdiplus;

#define WM_MYSOCKET_NOTIFY (WM_APP+0)  //WSAAsyncSelect에 쓰일 네트워크 이벤트 발생시 Post될 메시지
#define WM_MYTRAYICON_NOTIFY (WM_USER+1)  //TrayIcon에 쓰일 이벤트 발생시 Post될 메시지
#define WM_MYCLOSE (WM_USER+2)

#define IOCTL_PCSSMON_SHARDEVENT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PCSSMON_PROCINFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_PCSSLOCK_PCSSINFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0800, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define ProgName "Sun-PC_Client"
#define AutoRunReg "Software\\Microsoft\\Windows\\CurrentVersion\\Run"  //자동실행 등록하기 위한 레지스트리 위치
#define DeviceName_Monitor "pcssmonitor.sys"
#define DeviceName_Lock "pcsslock.sys"
#define MAXLINE 1024

//////////////////////////////////////////////////////
//Send 데이터 구조체
typedef struct tarIOData {
	char* pBuffer;
	int BufLen;
}IO_DATA;
//자신의 Path 정보를 저장하는 구조체
typedef struct _FilePath {
	char filepath[MAX_PATH];  //자신의  Full Path를 저장하는 변수(c:\windows\system\SUN-PC_CLIENT.exe)
	char drv[MAX_PATH];  //드라이버 저장 변수(c:)
	char path[MAX_PATH];  //Path 저장 변수(\windows\system\)
	char drv_path[MAX_PATH];  //드라이와 Path를 묶어서 저장하는 변수(c:\windows\system\)
	char fname[MAX_PATH];  //filename 저장 변수(SUN-PC_CLIENT)
	char ext[MAX_PATH];  //확장명 저장 변수(.exe)
} SFilePath;
//생성되고 소멸되는 프로세서 정보를 저장하는 구조체
typedef struct _PcssMonCallbackInfo {
	HANDLE hParentID;
	HANDLE hProcessID;
	BOOLEAN bCreate;
}PCSSMON_CALLBACK_INFO, *PPCSSMON_CALLBACK_INFO;

//////////////////////////////////////////////////////
BOOL CALLBACK ClientInitViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //클라이언트 INITVIEW 프로시져
BOOL ClientInitViewDlgInit(HWND hDlg);
BOOL ClientInitViewDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL ClientInitViewDlgPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);  //INITVIEW 배경화면 설정 함수

BOOL CALLBACK ClientDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //클라이언트 다이얼로그 프로시져
BOOL ClientDlgInit(HWND hDlg);
BOOL ClientDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ConnectDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //연결 다이얼로그 프로시져
BOOL ConnectDlgInit(HWND hDlg);
BOOL ConnectDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL ClientDlgSysCommand(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK AdminLoginDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //관리자로그인 다이얼로그 프로시져
BOOL AdminLoginDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ProcessMonitor(LPVOID arg);  //Process 감시 함수
DWORD WINAPI OnConnect(LPVOID arg);  //연결 함수
BOOL OnMySocketNotify(WPARAM wParam, LPARAM lParam);  //SEND,WRITE,CLOSE 발생시 수행하는 함수
BOOL OnMyTrayIconNotify(HWND hDlg, WPARAM wParam, LPARAM lParam);  //TRAYICON에서 메시지 발생시 수행하는 함수
void SetWindowState(HWND hChildDlg, HWND hParentDlg, BOOL Insert, int Width, int Height);
void DisplayText(char *fmt, ...);  //화면에 출력 함수
void Disconnect();  //연결 해제 함수
char *FindProcessName(HANDLE ProcessID, HANDLE ParentID);  //Process 번호로 Process 이름을 찾아주는 함수
void PCShutdown();


//////////////////////////////////////////////////////
HINSTANCE hInst;  //프로그램 인스턴스 핸들
HWND hClientInitViewDlg, hClientDlg;  //HWND 모음  
HWND hEditContent, hBTAddrSet, hBTAdminLogin, hBTAdminLogout, hBTProgExit, hBTAutoRunOn, hBTAutoRunOff, hBTLogout;  // ClientDlgProc의 HWND 모음
int SCR_Width = ::GetSystemMetrics(SM_CXSCREEN);  //스크린 X축 길이
int SCR_Height = ::GetSystemMetrics(SM_CYSCREEN);  //스크린 Y축 길이
Point ClientDlgSize((int)(SCR_Width*0.28), (int)(SCR_Height*0.5));
char PassWD[14], tPassWD[14];  //관리자로그인 패스워드
SOCKET ClientSocket;  //클라이언트소켓
char IPAddr[20];  //서버연결주소
int PortNum = 5056;  //서버연결포트
SFilePath FPath;  //자신의 Path 정보를 저장할 구조체 선언
IO_DATA IO_Data;
HWND hBTSendMsg, hEditSendMsg; // 프로토콜 테스트용으로 만듬
CRITICAL_SECTION cs;  //임계영역변수
map<int , char*> mapProcess;  //로그인 후 실행된 Process ID와 이름을 저장하는 Map
BOOL bRetInstall[2] = {FALSE};
char DeviceExe[2][_MAX_PATH];
static BOOL bLogin = FALSE, bViewDlg = TRUE;
static HANDLE hDriverMonFile;
static HANDLE hProcessEvent;