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



#pragma warning (disable: 4311) //C4311 Warning 메시지를 안 나타나게 함.

#define WM_MYLISTEN_NOTIFY (WM_APP+0)  //WSAAsyncSelect에 쓰일 네트워크 이벤트 발생시 Post될 메시지
#define IO_TYPE_SEND 0  //보내는 메시지
#define IO_TYPE_RECV 1  //받는 메시지
#define PORTNUM 5056  //서버포트번호
#define MAXLINE 1024  //최대 문자열 수

typedef struct _tarIOData {
	char* pBuffer;
	int BufLen;
}IO_DATA;
//IOCP 소켓 데이터 구조체
typedef struct _tarPerSocketData {
	SOCKET ClientSocket;  //접속되있는 클라이언트 소켓정보
	SOCKADDR_IN ClientAddr;
	char UserID[20];
	CXmlLogControl XmlLogCtrl;
	HWND hChatDlg,hChatContent;

}PER_SOCKET_DATA;
//IOCP I/O 데이터 구조체
typedef struct _tarParIOData {
	WSAOVERLAPPED Overlapped;
	BOOL Type;  //보내는 메시지인지 받는 메시지인지 구별
	char Buffer[MAXLINE];  //메시지 내용
	WSABUF WsaBuf;
}PER_IO_DATA;
//자신의 Path 정보를 저장하는 구조체
typedef struct _FilePath {
	char filepath[MAX_PATH];  //자신의  Full Path를 저장하는 변수(c:\windows\system\SUN-PC_CLIENT.exe)
	char drv[MAX_PATH];  //드라이버 저장 변수(c:)
	char path[MAX_PATH];  //Path 저장 변수(\windows\system\)
	char drv_path[MAX_PATH];  //드라이와 Path를 묶어서 저장하는 변수(c:\windows\system\)
	char fname[MAX_PATH];  //filename 저장 변수(SUN-PC_CLIENT)
	char ext[MAX_PATH];  //확장명 저장 변수(.exe)
} SFilePath;

///////////////////////////////
HINSTANCE hInst;  //프로그램 인스턴스 핸들
HWND hServerDlg,hBTServerOn,hBTServerOff,hStcPcCnt,hStcUserCnt,hListLoginUser,hEditContent;;  //ServerDlg의 HWND 모음
HWND hUserMngDlg, hListUserMng;  //UserMngDlg의 HWND 모음
HWND hAdminPWChangeDlg;  //AdminPWChangeDlg의 HWND 모음
SOCKET ListenSocket;  //서버소켓
HANDLE hComPort;  //IOCP 생성 핸들
CRITICAL_SECTION cs;  //임계영역변수
list<PER_SOCKET_DATA*> listPerSocket;  //접속자들을 저장하는 리스트
DWORD dwTransfer;  //데이터 길이
SFilePath FPath;  //자신의 Path 정보를 저장할 구조체 선언
int CPU_Cnt;
HANDLE* hWorkerThread;
int User_Cnt;


////////////////////////////
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //서버 다이얼로그 프로시져
BOOL ServerDlgInit(HWND hDlg);  //ServerDlg 초기화
BOOL ServerDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);  //ServerDlg Notify
BOOL ServerDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);  //ServerDlg 커멘드
BOOL ServerDlgPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);  //ServerDlg 배경화면 설정
BOOL ServerDlgCtlcolorstatic(WPARAM wParam,LPARAM lParam);  //ServerDlg Static 컨트롤러 배경생 지정

BOOL CALLBACK UserMngDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //사용자관리 다이얼로그 프로시져
BOOL UserMngDlgInit(HWND hDlg);  //UserMngDlg 초기화
BOOL UserMngDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL UserMngDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);

BOOL CALLBACK AdminPWChangeDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //관리자 패스워드 변경 다이얼로그 프로시져

BOOL CALLBACK ChatDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //ChatDlg 프로시져

void OnServerStart();  //서버 시작 함수
BOOL OnMyListenNotify(WPARAM wParam,LPARAM lParam);  //Accept 발생시 수행하는 함수
DWORD WINAPI WorkerThread(LPVOID arg);  //WorkerThread 함수
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData);  //Recv 함수
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,IO_DATA* IO_Data);  //Send 함수
void DisplayText(HWND hContent,char *fmt,...);  //화면에 출력 함수
DWORD WINAPI ServerUsageThread(LPVOID arg);  //Server 사용률 체크 함수
void LoginUserListReview(HWND hList);
void CreateDir(char* Path);