#include "resource.h"
#include <winsock2.h>
#include <stdio.h>

BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm);
BOOL CALLBACK ConnectDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm);
BOOL CALLBACK IDDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm);
DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI RecvThread(LPVOID arg);
void DestSocket();
void DisplayText(HWND Edit,char* Msg,...);

HINSTANCE hInst;
HWND hClientDlg,hEditChat,hEditSend,hBNSend,hBNConnect,hBNDisconnect;
SOCKET sClientSock;
char ChatStr[512];
char IPAddr[20]="127.0.0.1";
int PortNum=9292;
char NickName[50]="Client";
BOOL bClose=FALSE;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrvInst,LPSTR lpCmdLine,int nCmdShow)
{
	hInst=hInstance;
	DialogBox(hInst,MAKEINTRESOURCE(IDD_CLIENT_DLG),NULL,ClientDlgProc);
	return 0;
}
//Client 다이얼로그 프로시저 부분
BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	int ret;
	char TmpStr[512];
	switch(iMsg)	{
		case WM_INITDIALOG:
			//다이얼로그 정보 가져오기
			hClientDlg=hDlg;
			hEditChat=GetDlgItem(hDlg,IDC_EDIT_CHAT);
			hEditSend=GetDlgItem(hDlg,IDC_EDIT_SEND);
			hBNConnect=GetDlgItem(hDlg,IDC_BNCONNECT);
			hBNDisconnect=GetDlgItem(hDlg,IDC_BNDISCONNECT);
			EnableWindow(hBNDisconnect,FALSE);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wPm))	{
				case IDC_BNSEND:
					//전송할 문자열 읽기
					GetDlgItemText(hDlg,IDC_EDIT_SEND,TmpStr,sizeof(TmpStr));
					//전송할 문자열 자신의 CHAT에 출력(NickName과 TmpStr을 합침)
					strcpy(ChatStr,NickName);
					strcat(ChatStr," : ");
					strcat(ChatStr,TmpStr);
					strcpy(TmpStr,"");
					DisplayText(hEditChat,"%s\r\n",ChatStr);
					// 채팅 문자열 박스 비우기
					SetDlgItemText(hDlg,IDC_EDIT_SEND,"");
					SetFocus(hEditSend);
					//Disconnect 상태이면 Send를 하지 않기 위해 break함
					if(sClientSock==0) break;
					//Send()데이터 보내기
					ret=send(sClientSock,ChatStr,sizeof(ChatStr),0);
					if(ret==SOCKET_ERROR)	{
						DisplayText(hEditChat,"send() 실패\r\n");
						break;
					}
					strcpy(ChatStr,"");
					break;
				case IDC_BNCONNECT:	//connect 다이얼로그 열기
					DialogBox(hInst,MAKEINTRESOURCE(IDD_CONNECTDLG),hDlg,ConnectDlgProc);
					break;
				case IDC_BNDISCONNECT:	//연결 끊기
					DestSocket();
					bClose=TRUE;
					break;
				case IDC_BNID:	//대화명 다이얼로그 열기
					DialogBox(hInst,MAKEINTRESOURCE(IDD_IDDLG),hDlg,IDDlgProc);
					break;
				case IDC_BNCLOSE:	//클라이언트 다이얼로그 받기(프로그램 종료)
					SendMessage(hDlg,WM_CLOSE,0,0);
					break;
			}
			return TRUE;
		case WM_CLOSE:
			DestSocket();
			bClose=TRUE;
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//Connect 다이얼로그 프로시저 부분
BOOL CALLBACK ConnectDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	HANDLE hClientThread;
	DWORD ThreadId;
	switch(iMsg)	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_IPADDRESS,IPAddr);
			SetDlgItemInt(hDlg,IDC_PORT,PortNum,FALSE);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wPm))	{
				case IDOK:
					//Connect다이얼로그에서 데이터 저장
					GetDlgItemText(hDlg,IDC_IPADDRESS,IPAddr,sizeof(IPAddr));
					PortNum=GetDlgItemInt(hDlg,IDC_PORT,NULL,FALSE);
					// 클라이언트 스레드 시작
					hClientThread=CreateThread(NULL,0,ClientMain,NULL,0,&ThreadId);
					if(hClientThread==NULL)	{
						MessageBox(hClientDlg,"스레드를 시작할 수 없습니다.\r\n서버와 연결할 수 없습니다.\r\n프로그램을 종료합니다","스레드 실패",MB_OK);
						SendMessage(hClientDlg,WM_CLOSE,0,0);
					}
					else {
						DisplayText(hEditChat,"서버와 연결중...\r\n");
					}
					SendMessage(hDlg,WM_CLOSE,0,0);
					SetFocus(hEditSend);
					bClose=FALSE;
					break;
				case IDCANCEL:
					SendMessage(hDlg,WM_CLOSE,0,0);
					break;
			}
			return TRUE;
		case WM_CLOSE:
			EndDialog(hDlg,0);	//connenct 다이얼로그 닫기
			return TRUE;
	}
	return FALSE;
}
//대화명 다이얼로그 프로시저 부분
BOOL CALLBACK IDDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	switch(iMsg)	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_ID,NickName);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wPm))	{
				case IDOK:
					//대화명 다이얼로그에서 데이터 저장
					GetDlgItemText(hDlg,IDC_ID,NickName,sizeof(NickName));
					SendMessage(hDlg,WM_CLOSE,0,0);
					break;
			}
			return TRUE;
		case WM_CLOSE:
			//대화명 다이얼로그 닫기
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//TCP 연결 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
	int ret;
	SOCKADDR_IN serveraddr;
	HANDLE hRecvThread;
	DWORD ThreadId;
	// 윈속 초기화
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)	{
		MessageBox(hClientDlg,"윈속 초기화 실패","WSAStartup",MB_OK);
		DestSocket();
		return -1;
	}
	// TCP 소켓생성
	sClientSock=socket(AF_INET,SOCK_STREAM,0);
	if(sClientSock==INVALID_SOCKET)	{
		MessageBox(hClientDlg,"TCP 소켓생성 실패","socket",MB_OK);
		DestSocket();
		return -1;
	}
	//주소정보입력
	ZeroMemory(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port = htons(PortNum);
	serveraddr.sin_addr.S_un.S_addr=inet_addr(IPAddr);
	//connect()
	ret=connect(sClientSock,(SOCKADDR*)&serveraddr,sizeof(serveraddr));
	if(ret==SOCKET_ERROR)	{
		MessageBox(hClientDlg,"connect() 실패","connect",MB_OK);
		DestSocket();
		return -1;
	}
	//버튼 상태 변경
	EnableWindow(hBNConnect,FALSE);
	EnableWindow(hBNDisconnect,TRUE);

	DisplayText(hEditChat,"서버에 접속하였습니다.\r\n");
	//받기 스레드 생성
	hRecvThread=CreateThread(NULL,0,RecvThread,(LPVOID)sClientSock,0,&ThreadId);
	if(hRecvThread==NULL)	{
		MessageBox(hClientDlg,"스레드를 시작할 수 없습니다.\r\n프로그램을 종료합니다.","스레드 실패",MB_OK);
		SendMessage(hClientDlg,WM_CLOSE,0,0);
	}
	//스레드 종료 대기
	ret=WaitForSingleObject(hRecvThread,INFINITE);
	//스레드 종료
	TerminateThread(hRecvThread,-1);
	CloseHandle(hRecvThread);

	DisplayText(hEditChat,"서버와 연결이 끊어졌습니다.\r\n");
	////버튼 상태 변경
	EnableWindow(hBNDisconnect,FALSE);
	EnableWindow(hBNConnect,TRUE);

	DestSocket();	//소켓 닫음

	return 0;
}
// 데이터 받기
DWORD WINAPI RecvThread(LPVOID arg)
{
	int ret;
	SOCKET client_sock=(SOCKET)arg;
	while(1)	{
		//Recv()데이터 받기를 기다림 
		ret=recv(client_sock,ChatStr,sizeof(ChatStr),0);
		if(ret==SOCKET_ERROR||ret==0) break;
		else if(ret==SOCKET_ERROR&&bClose==TRUE)	{
			DisplayText(hEditChat,"recv() 실패\r\n");
			break;
		}
		//데이터 끝에 널문자 입력
		ChatStr[ret]='\0';
		//받은 데이터 출력
		DisplayText(hEditChat,"%s\r\n",ChatStr);
	}
	//client_sock 닫기
	closesocket(client_sock);
	return 0;
}
//소켓 닫기
void DestSocket()
{
	if(sClientSock!=0) closesocket(sClientSock);
	sClientSock=0;
	WSACleanup();
}
//출력부분
void DisplayText(HWND Edit,char* Msg,...)
{
	va_list arg;
	va_start(arg,Msg);

	char chTmp[128];
	// 커서를 맨 끝으로 옮김
	SendMessage(Edit,EM_SETSEL,0,-1);
	SendMessage(Edit,EM_SETSEL,-1,-1);
	// 문자열을 추가함
	vsprintf(chTmp,Msg,arg);
	SendMessage(Edit,EM_REPLACESEL,0,(LPARAM)chTmp);

	va_end(arg);
}