#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "resource.h"

using namespace std;

#define WM_MYSOCKET_NOTIFY (WM_APP+0)  //WSAAsyncSelect에 쓰일 네트워크 이벤트 발생시 Post될 메시지
#define MAXLINE 1024

BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //클라이언트 다이얼로그 프로시져
BOOL CALLBACK ConnectDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //연결 다이얼로그 프로시져
void OnConnect();  //연결 함수
void OnMySocketNotify(WPARAM wParam,LPARAM lParam);  //SEND,WRITE,CLOSE 발생시 수행하는 함수
void Disconnect();  //연결 해제 함수
void DisplayText(char *fmt,...);  //화면에 출력 함수

HINSTANCE hInst;
HWND hClientDlg,hEditContent,hEditSendMsg,hBTSend,hBTConnect,hBTDisconnect;
char IPAddr[20]="127.0.0.1";
int PortNum=5056;
SOCKET ClientSocket;
char Buffer[MAXLINE];  //메시지 저장 변수

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,int nCmdShow) {
	hInst=hInstance;
	//윈속 초기화
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"윈속 초기화 실패!","ERROR",MB_OK);
		return 0;
	}
	//다이얼로그 생성 및 실행
	DialogBox(hInst,MAKEINTRESOURCE(IDD_CLIENT_DLG),NULL,ClientDlgProc);
	//윈속 종료
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}
//클라이언트 다이얼로그 프로시져
BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
			hClientDlg=hDlg;
			hEditContent=GetDlgItem(hDlg,IDC_EDIT_CONTENT);
			hEditSendMsg=GetDlgItem(hDlg,IDC_EDIT_SENDMSG);
			hBTSend=GetDlgItem(hDlg,IDC_BT_SEND);
			hBTConnect=GetDlgItem(hDlg,IDC_BT_CONNECT);
			hBTDisconnect=GetDlgItem(hDlg,IDC_BT_DISCONNECT);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BT_CONNECT:
					DialogBox(hInst,MAKEINTRESOURCE(IDD_CONNECT_DLG),NULL,ConnectDlgProc);
					break;
				case IDC_BT_DISCONNECT:
					Disconnect();
					break;
				case IDC_BT_SEND:
					GetDlgItemText(hDlg,IDC_EDIT_SENDMSG,Buffer,MAXLINE-1);
					DisplayText("[SEND MSG] %s\n",Buffer);
					SendMessage(hDlg,WM_MYSOCKET_NOTIFY,ClientSocket,WSAMAKESELECTREPLY(FD_WRITE,0));
					SetDlgItemText(hDlg,IDC_EDIT_SENDMSG,"");
					break;
			}
			return TRUE;
		case WM_MYSOCKET_NOTIFY:
			OnMySocketNotify(wParam,lParam);
			return TRUE;
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//연결 다이얼로그 프로시져
BOOL CALLBACK ConnectDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_IPADDRESS,IPAddr);
			SetDlgItemInt(hDlg,IDC_EDIT_PORTNUM,PortNum,FALSE);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BT_CONNETOK:
					GetDlgItemText(hDlg,IDC_IPADDRESS,IPAddr,sizeof(IPAddr));
					PortNum=GetDlgItemInt(hDlg,IDC_EDIT_PORTNUM,NULL,FALSE);
					SendMessage(hDlg,WM_CLOSE,0,0);
					//서버에 접속
					OnConnect();
					break;
			}
			return TRUE;
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//연결 함수
void OnConnect() {
	//Socket 생성
	ClientSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ClientSocket==INVALID_SOCKET) {
		DisplayText("[ERROR] Socket 생성 실패!\n");
		return;
	}else DisplayText("[SUCCESS] Socket 생성 성공!\n");
	//주소 정보 설정
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=inet_addr(IPAddr);
	ServerAddr.sin_port=htons(PortNum);
	DisplayText("서버에 접속중.....\n");
	//Connect
	if(connect(ClientSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText("[ERROR] Connect 실패!\n");
		closesocket(ClientSocket);
		return;
	}else DisplayText("[SUCCESS] Connect 성공!\n");
	//WSAAsyncSelect 실행
	if(WSAAsyncSelect(ClientSocket,hClientDlg,WM_MYSOCKET_NOTIFY,FD_READ|FD_WRITE|FD_CLOSE)==SOCKET_ERROR) {
		DisplayText("[ERROR] WSAAsyncSelect 실패!\n");
		closesocket(ClientSocket);
		return;
	}else DisplayText("[SUCCESS] WSAAsyncSelect 성공!\n");
	//버튼 상태 변경
	EnableWindow(hEditSendMsg,TRUE);
	EnableWindow(hBTSend,TRUE);
	EnableWindow(hBTDisconnect,TRUE);
	EnableWindow(hBTConnect,FALSE);

	DisplayText("Connect!!!\n");
}
//SEND,WRITE,CLOSE 발생시 수행하는 함수
void OnMySocketNotify(WPARAM wParam,LPARAM lParam) {
	int nEvent=WSAGETSELECTEVENT(lParam);
	if(nEvent==FD_READ) {
		ZeroMemory(Buffer,sizeof(Buffer));
		recv(ClientSocket,Buffer,sizeof(Buffer),0);
		DisplayText("[RECV MSG] %s\n",Buffer);
	}
	else if(nEvent==FD_WRITE) {
		send(ClientSocket,Buffer,strlen(Buffer),0);
	}
	else if(nEvent==FD_CLOSE) {
		Disconnect();
	}
}
//연결 해제 함수
void Disconnect() {
	//소켓 닫기
	closesocket(ClientSocket);
	//버튼 상태 변경
	EnableWindow(hEditSendMsg,FALSE);
	EnableWindow(hBTSend,FALSE);
	EnableWindow(hBTDisconnect,FALSE);
	EnableWindow(hBTConnect,TRUE);
	DisplayText("서버와 연결이 끊어짐.\n");
}
//화면에 출력 함수
void DisplayText(char *fmt,...) {
	va_list arg;
	va_start(arg,fmt);
	
	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf,fmt,arg);
	
	int nLength=GetWindowTextLength(hEditContent);
	SendMessage(hEditContent,EM_SETSEL,nLength,nLength);
	SendMessage(hEditContent,EM_REPLACESEL,FALSE,(LPARAM)cbuf);
	
	va_end(arg);
}