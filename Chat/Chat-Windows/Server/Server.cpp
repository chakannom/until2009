#include "resource.h"
#include <winsock2.h>
#include <stdio.h>

#define PORTNUM 9292
#define BUFSIZE 512
#define MAXCLIENT 10

BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm);
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);
SOCKET InitSocket(HWND hWnd);
void DestSocket();
void DisplayText(HWND Edit,char* Msg,...);

HINSTANCE hInst;
HWND hServerDlg,hEdit;
SOCKET sServerSock;
SOCKET client_socks[MAXCLIENT];
int clientcnt=0;
char title[128];

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrvInst,LPSTR lpCmdLine,int nCmdShow)
{

	hInst=hInstance;
	DialogBox(hInst,MAKEINTRESOURCE(IDD_SERVER_DLG),NULL,ServerDlgProc);
	return 0;
}
//Server다이얼로그 프로시저 부분
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	DWORD ThreadId;
	switch(iMsg)	{
		case WM_INITDIALOG:
			hServerDlg=hDlg;
			hEdit=GetDlgItem(hDlg,IDC_EDITINFO);
			DisplayText(hEdit,"Server 실행중...\r\n");
			CreateThread(NULL,0,ServerMain,NULL,0,&ThreadId); //ServerMain 스레드 생성
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wPm))	{
				case IDC_BTNCLOSE:
					SendMessage(hDlg,WM_CLOSE,0,0);
					break;
			}
			return TRUE;
		case WM_CLOSE:
			DestSocket();
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
// TCP 서버 시작 부분
DWORD WINAPI ServerMain(LPVOID arg)
{
	WSADATA wsaData;
	SOCKADDR_IN addr_in;
	
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	DWORD ThreadId;

	PHOSTENT hostinfo;
	char hostname[255];

	int ret;
	//윈속 dll 초기화
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)	{
		MessageBox(hServerDlg,"윈속 초기화 실패","WSAStartup",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	// TCP 소켓생성
	sServerSock=socket(AF_INET,SOCK_STREAM,0);
	if(sServerSock==INVALID_SOCKET)	{
		MessageBox(hServerDlg,"TCP 소켓생성 실패","socket",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//주소정보입력
	ZeroMemory(&addr_in,sizeof(addr_in));
	addr_in.sin_family=AF_INET;
	addr_in.sin_port=htons(PORTNUM);
	addr_in.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	//bind
	ret=bind(sServerSock,(SOCKADDR*)&addr_in,sizeof(addr_in));
	if(ret==SOCKET_ERROR)	{
		MessageBox(hServerDlg,"bind 실패","bind",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//listen
	ret=listen(sServerSock,5);
	if(ret==SOCKET_ERROR)	{
		MessageBox(hServerDlg,"listen 실패","listen",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//자신의 아이피주소 찾기
	if(gethostname(hostname,sizeof(hostname))==0)	{
		if((hostinfo=gethostbyname(hostname))!=NULL)	{
			sprintf(title,"YS Server(서버의 IP주소 : %s / Port번호 : %d)",inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list),PORTNUM);
			SetWindowText(hServerDlg,title);
		}
	}
	DisplayText(hEdit,"서버소켓 생성 !! 클라이언트의 접속을 기다립니다..\r\n");

	while(1){
		// accept()
		addrlen=sizeof(clientaddr);
		client_sock=accept(sServerSock,(SOCKADDR*)&clientaddr,&addrlen);
		if(client_sock==INVALID_SOCKET)	{
			DisplayText(hEdit,"Accept() 실패\r\n");
			continue;
		}
		client_socks[clientcnt++]=client_sock;
		DisplayText(hEdit,"[TCP서버]클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		// 스레드 생성
		hThread=CreateThread(NULL,0,ProcessClient,(LPVOID)client_sock,0,&ThreadId);
		if(hThread==NULL) DisplayText(hEdit,"[오류] 스레드 생성 실패!\r\n");
		else CloseHandle(hThread);
	}
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock=(SOCKET)arg;
	char buf[BUFSIZE+1];
	SOCKADDR_IN clientaddr;
	int addrlen;
	int ret;

	// 클라이언트 정보 얻기
	addrlen=sizeof(clientaddr);
	getpeername(client_sock,(SOCKADDR *)&clientaddr,&addrlen);

	while(1)	{
		// 데이터 받기
		ret=recv(client_sock,buf,BUFSIZE,0);
		if(ret==SOCKET_ERROR||ret==0) break;
		// 받은 데이터 출력
		buf[ret]='\0';
		DisplayText(hEdit,"[TCP/%s:%d] %s\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port),buf);
		// 데이터 보내기
		for(int i=0;i<clientcnt;i++)	{
			if(client_socks[i]!=client_sock)	{
				ret=send(client_socks[i],buf,ret,0);
				if(ret==SOCKET_ERROR)	{
					getpeername(client_socks[i],(SOCKADDR *)&clientaddr,&addrlen);
					DisplayText(hEdit,"Send()실패(%s:%d)\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
				}
			}
		}
	}
	// closesocket()
	for(int i=0;i<clientcnt-1;i++)	{
		if(client_socks[i]==client_sock)	{
			for(;i<clientcnt-1;i++) client_socks[i]=client_socks[i+1];
			break;
		}
	}
	clientcnt--;
	closesocket(client_sock);
	DisplayText(hEdit,"[TCP 서버]클라이언트 종료: IP 주소=%s, 포트 번호=%d\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

	return 0;
}

void DestSocket()
{
	for(int i=0;i<clientcnt;i++)	{
		closesocket(client_socks[i]);
	}
	if(sServerSock!=0) closesocket(sServerSock);
	WSACleanup();
}

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