#undef UNICODE
#undef _UNICODE

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gdiplus.h>
#include <list>
#include <string>
#include "XmlLogControl.h"

using namespace std;
using namespace Gdiplus;



#pragma warning (disable: 4311) //C4311 Warning �޽����� �� ��Ÿ���� ��.

#define WM_MYLISTEN_NOTIFY (WM_APP+0)  //WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻��� Post�� �޽���
#define IO_TYPE_SEND 0  //������ �޽���
#define IO_TYPE_RECV 1  //�޴� �޽���
#define PORTNUM 5056  //������Ʈ��ȣ
#define MAXLINE 1024  //�ִ� ���ڿ� ��

typedef struct _tarIOData {
	char* pBuffer;
	int BufLen;
}IO_DATA;
//IOCP ���� ������ ����ü
typedef struct _tarPerSocketData {
	SOCKET ClientSocket;  //���ӵ��ִ� Ŭ���̾�Ʈ ��������
	SOCKADDR_IN ClientAddr;
	char UserID[20];
	CXmlLogControl XmlLogCtrl;
	HWND hChatDlg,hChatContent;

}PER_SOCKET_DATA;
//IOCP I/O ������ ����ü
typedef struct _tarParIOData {
	WSAOVERLAPPED Overlapped;
	BOOL Type;  //������ �޽������� �޴� �޽������� ����
	char Buffer[MAXLINE];  //�޽��� ����
	WSABUF WsaBuf;
}PER_IO_DATA;
//�ڽ��� Path ������ �����ϴ� ����ü
typedef struct _FilePath {
	char filepath[MAX_PATH];  //�ڽ���  Full Path�� �����ϴ� ����(c:\windows\system\SUN-PC_CLIENT.exe)
	char drv[MAX_PATH];  //����̹� ���� ����(c:)
	char path[MAX_PATH];  //Path ���� ����(\windows\system\)
	char drv_path[MAX_PATH];  //����̿� Path�� ��� �����ϴ� ����(c:\windows\system\)
	char fname[MAX_PATH];  //filename ���� ����(SUN-PC_CLIENT)
	char ext[MAX_PATH];  //Ȯ��� ���� ����(.exe)
} SFilePath;

///////////////////////////////
HINSTANCE hInst;  //���α׷� �ν��Ͻ� �ڵ�
HWND hServerDlg,hBTServerOn,hBTServerOff,hStcPcCnt,hStcUserCnt,hListLoginUser,hEditContent;;  //ServerDlg�� HWND ����
HWND hUserMngDlg, hListUserMng;  //UserMngDlg�� HWND ����
HWND hAdminPWChangeDlg;  //AdminPWChangeDlg�� HWND ����
SOCKET ListenSocket;  //��������
HANDLE hComPort;  //IOCP ���� �ڵ�
CRITICAL_SECTION cs;  //�Ӱ迵������
list<PER_SOCKET_DATA*> listPerSocket;  //�����ڵ��� �����ϴ� ����Ʈ
DWORD dwTransfer;  //������ ����
SFilePath FPath;  //�ڽ��� Path ������ ������ ����ü ����
int CPU_Cnt;
HANDLE* hWorkerThread;
int User_Cnt;


////////////////////////////
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //���� ���̾�α� ���ν���
BOOL ServerDlgInit(HWND hDlg);  //ServerDlg �ʱ�ȭ
BOOL ServerDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);  //ServerDlg Notify
BOOL ServerDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);  //ServerDlg Ŀ���
BOOL ServerDlgPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);  //ServerDlg ���ȭ�� ����
BOOL ServerDlgCtlcolorstatic(WPARAM wParam,LPARAM lParam);  //ServerDlg Static ��Ʈ�ѷ� ���� ����

BOOL CALLBACK UserMngDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //����ڰ��� ���̾�α� ���ν���
BOOL UserMngDlgInit(HWND hDlg);  //UserMngDlg �ʱ�ȭ
BOOL UserMngDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL UserMngDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);

BOOL CALLBACK AdminPWChangeDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //������ �н����� ���� ���̾�α� ���ν���

BOOL CALLBACK ChatDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //ChatDlg ���ν���

void OnServerStart();  //���� ���� �Լ�
BOOL OnMyListenNotify(WPARAM wParam,LPARAM lParam);  //Accept �߻��� �����ϴ� �Լ�
DWORD WINAPI WorkerThread(LPVOID arg);  //WorkerThread �Լ�
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData);  //Recv �Լ�
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,IO_DATA* IO_Data);  //Send �Լ�
void DisplayText(HWND hContent,char *fmt,...);  //ȭ�鿡 ��� �Լ�
DWORD WINAPI ServerUsageThread(LPVOID arg);  //Server ���� üũ �Լ�
void LoginUserListReview(HWND hList);
void CreateDir(char* Path);