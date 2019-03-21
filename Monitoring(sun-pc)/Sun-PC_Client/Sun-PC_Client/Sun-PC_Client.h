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

#define WM_MYSOCKET_NOTIFY (WM_APP+0)  //WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻��� Post�� �޽���
#define WM_MYTRAYICON_NOTIFY (WM_USER+1)  //TrayIcon�� ���� �̺�Ʈ �߻��� Post�� �޽���
#define WM_MYCLOSE (WM_USER+2)

#define IOCTL_PCSSMON_SHARDEVENT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PCSSMON_PROCINFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_PCSSLOCK_PCSSINFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0800, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define ProgName "Sun-PC_Client"
#define AutoRunReg "Software\\Microsoft\\Windows\\CurrentVersion\\Run"  //�ڵ����� ����ϱ� ���� ������Ʈ�� ��ġ
#define DeviceName_Monitor "pcssmonitor.sys"
#define DeviceName_Lock "pcsslock.sys"
#define MAXLINE 1024

//////////////////////////////////////////////////////
//Send ������ ����ü
typedef struct tarIOData {
	char* pBuffer;
	int BufLen;
}IO_DATA;
//�ڽ��� Path ������ �����ϴ� ����ü
typedef struct _FilePath {
	char filepath[MAX_PATH];  //�ڽ���  Full Path�� �����ϴ� ����(c:\windows\system\SUN-PC_CLIENT.exe)
	char drv[MAX_PATH];  //����̹� ���� ����(c:)
	char path[MAX_PATH];  //Path ���� ����(\windows\system\)
	char drv_path[MAX_PATH];  //����̿� Path�� ��� �����ϴ� ����(c:\windows\system\)
	char fname[MAX_PATH];  //filename ���� ����(SUN-PC_CLIENT)
	char ext[MAX_PATH];  //Ȯ��� ���� ����(.exe)
} SFilePath;
//�����ǰ� �Ҹ�Ǵ� ���μ��� ������ �����ϴ� ����ü
typedef struct _PcssMonCallbackInfo {
	HANDLE hParentID;
	HANDLE hProcessID;
	BOOLEAN bCreate;
}PCSSMON_CALLBACK_INFO, *PPCSSMON_CALLBACK_INFO;

//////////////////////////////////////////////////////
BOOL CALLBACK ClientInitViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //Ŭ���̾�Ʈ INITVIEW ���ν���
BOOL ClientInitViewDlgInit(HWND hDlg);
BOOL ClientInitViewDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL ClientInitViewDlgPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);  //INITVIEW ���ȭ�� ���� �Լ�

BOOL CALLBACK ClientDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //Ŭ���̾�Ʈ ���̾�α� ���ν���
BOOL ClientDlgInit(HWND hDlg);
BOOL ClientDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ConnectDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //���� ���̾�α� ���ν���
BOOL ConnectDlgInit(HWND hDlg);
BOOL ConnectDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL ClientDlgSysCommand(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK AdminLoginDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  //�����ڷα��� ���̾�α� ���ν���
BOOL AdminLoginDlgCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ProcessMonitor(LPVOID arg);  //Process ���� �Լ�
DWORD WINAPI OnConnect(LPVOID arg);  //���� �Լ�
BOOL OnMySocketNotify(WPARAM wParam, LPARAM lParam);  //SEND,WRITE,CLOSE �߻��� �����ϴ� �Լ�
BOOL OnMyTrayIconNotify(HWND hDlg, WPARAM wParam, LPARAM lParam);  //TRAYICON���� �޽��� �߻��� �����ϴ� �Լ�
void SetWindowState(HWND hChildDlg, HWND hParentDlg, BOOL Insert, int Width, int Height);
void DisplayText(char *fmt, ...);  //ȭ�鿡 ��� �Լ�
void Disconnect();  //���� ���� �Լ�
char *FindProcessName(HANDLE ProcessID, HANDLE ParentID);  //Process ��ȣ�� Process �̸��� ã���ִ� �Լ�
void PCShutdown();


//////////////////////////////////////////////////////
HINSTANCE hInst;  //���α׷� �ν��Ͻ� �ڵ�
HWND hClientInitViewDlg, hClientDlg;  //HWND ����  
HWND hEditContent, hBTAddrSet, hBTAdminLogin, hBTAdminLogout, hBTProgExit, hBTAutoRunOn, hBTAutoRunOff, hBTLogout;  // ClientDlgProc�� HWND ����
int SCR_Width = ::GetSystemMetrics(SM_CXSCREEN);  //��ũ�� X�� ����
int SCR_Height = ::GetSystemMetrics(SM_CYSCREEN);  //��ũ�� Y�� ����
Point ClientDlgSize((int)(SCR_Width*0.28), (int)(SCR_Height*0.5));
char PassWD[14], tPassWD[14];  //�����ڷα��� �н�����
SOCKET ClientSocket;  //Ŭ���̾�Ʈ����
char IPAddr[20];  //���������ּ�
int PortNum = 5056;  //����������Ʈ
SFilePath FPath;  //�ڽ��� Path ������ ������ ����ü ����
IO_DATA IO_Data;
HWND hBTSendMsg, hEditSendMsg; // �������� �׽�Ʈ������ ����
CRITICAL_SECTION cs;  //�Ӱ迵������
map<int , char*> mapProcess;  //�α��� �� ����� Process ID�� �̸��� �����ϴ� Map
BOOL bRetInstall[2] = {FALSE};
char DeviceExe[2][_MAX_PATH];
static BOOL bLogin = FALSE, bViewDlg = TRUE;
static HANDLE hDriverMonFile;
static HANDLE hProcessEvent;