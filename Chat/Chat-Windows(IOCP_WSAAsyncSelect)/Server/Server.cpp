#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>
#include <list>
#include "resource.h"

using namespace std;

#define WM_MYLISTEN_NOTIFY (WM_APP+0)  //WSAAsyncSelect에 쓰일 네트워크 이벤트 발생시 Post될 메시지
#define IO_TYPE_SEND 0
#define IO_TYPE_RECV 1
#define PORTNUM 5056
#define MAXLINE 1024

//IOCP 소켓 데이터 구조체
typedef struct _tarPerSocketData {
	SOCKET ClientSocket;
}PER_SOCKET_DATA;
//IOCP I/O 데이터 구조체
typedef struct tarParIOData {
	WSAOVERLAPPED Overlapped;
	BOOL Type;
	char Buffer[MAXLINE];
	WSABUF WsaBuf;
}PER_IO_DATA;

BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //서버 다이얼로그 프로시져
void OnServerStart();  //서버 시작 함수
void OnMyListenNotify(WPARAM wParam,LPARAM lParam);  //Accept 발생시 수행하는 함수
DWORD WINAPI WorkerThread(LPVOID arg);  //WorkerThread 함수
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData);  //Recv 함수
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,char* pBuffer);  //Send 함수
void DisplayText(char *fmt,...);  //화면에 출력 함수

HINSTANCE hInst;
HWND hServerDlg,hEditContent;
SOCKET ListenSocket;
HANDLE hComPort;
CRITICAL_SECTION cs;
list<PER_SOCKET_DATA*> listPerSocket;  //접속자들을 저장하는 리스트
DWORD dwTransfer;  //데이터 길이

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	hInst=hInstance;
	//임계영역 초기화
	InitializeCriticalSection(&cs);
	//윈속 초기화
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"윈속 초기화 실패!","ERROR",MB_OK);
		return 0;
	}
	//다이얼로그 생성 및 실행
	DialogBox(hInst,MAKEINTRESOURCE(IDD_SERVER_DLG),NULL,ServerDlgProc);
	//윈속 종료
	closesocket(ListenSocket);
	WSACleanup();
	//임계영역 정리
	DeleteCriticalSection(&cs);
	return 0;
}
//서버 다이얼로그 프로시져
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
			hServerDlg=hDlg;
			hEditContent=GetDlgItem(hDlg,IDC_EDIT_CONTENT);
			SetDlgItemText(hDlg,IDC_EDIT_SENDMSG,"이 부분은 우선 보류~~ START버튼을 누르면 서버가 시작됩니다.");
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BT_START:
					OnServerStart();
					break;
				case IDC_BT_SEND:
					MessageBox(hDlg,"이 부분은 우선 보류~~","공사중",MB_OK);
					break;
			}
			return TRUE;
		case WM_MYLISTEN_NOTIFY:
			OnMyListenNotify(wParam,lParam);
			return TRUE;
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//서버 시작 함수
void OnServerStart() {
	//Socket 생성
	ListenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ListenSocket==INVALID_SOCKET) {
		DisplayText("[ERROR] Socket 생성 실패!\n");
		return;
	}else DisplayText("[SUCCESS] Socket 생성 성공!\n");
	//주소 정보 설정
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(PORTNUM);
	//Bind
	if(bind(ListenSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText("[ERROR] Bind 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] Bind 성공!\n");
	//Listen
	if(listen(ListenSocket,5)==SOCKET_ERROR) {
		DisplayText("[ERROR] Listen 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] Listen 성공!\n");
	//메시지 방식의 non-blocking 모드의 소켓으로 변경
	WSAAsyncSelect(ListenSocket,hServerDlg,WM_MYLISTEN_NOTIFY,FD_ACCEPT);
	//IOCP 생성
	hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(hComPort==NULL) {
		DisplayText("[ERROR] IOCP 생성 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] IOCP 생성 성공!\n");
	//시스템정보획득(CPU개수를 위한 부분)
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	//WorkerThread 생성(CPU개수*2만큼)
	HANDLE hThread;
	DWORD ThreadId;
	for(int i=0;i<(int)SystemInfo.dwNumberOfProcessors*2;i++) {
		hThread=CreateThread(NULL,0,WorkerThread,hComPort,0,&ThreadId);
		if(hThread==NULL) {
			DisplayText("[ERROR] %d번째 WorkerThread 생성 실패!\n",i+1);
			return;
		}else DisplayText("[SUCCESS] %d번째 WorkerThread 생성 성공!\n",i+1);
		CloseHandle(hThread);
	}
}
//Accept 발생시 수행하는 함수
void OnMyListenNotify(WPARAM wParam,LPARAM lParam) {
	int nEvent=WSAGETSELECTEVENT(lParam);
	int nError=WSAGETSELECTERROR(lParam);
	if(nEvent==FD_ACCEPT&&nError==0) {
		SOCKET ClientSocket;
		SOCKADDR_IN ClientAddr;
		int AddrLen=sizeof(ClientAddr);
		//새로운 클라이언트를 받음
		ClientSocket=accept(ListenSocket,(PSOCKADDR)&ClientAddr,&AddrLen);
		//IOCP 소켓 데이터 구조체 메모리 할당
		PER_SOCKET_DATA* PerSocketData=new PER_SOCKET_DATA;
		PerSocketData->ClientSocket=ClientSocket;
		//임계 영역에 들어감
		EnterCriticalSection(&cs);
		//접속한 클라이언트를 listPerIO에 저장함
		listPerSocket.push_back(PerSocketData);
		//임계 영역을 빠져나감
		LeaveCriticalSection(&cs);
		//억셉트된 소켓을 IOCP에 참여시킴
		CreateIoCompletionPort((HANDLE)ClientSocket,hComPort,(DWORD)PerSocketData,0);
		// 최초로 WSARecv() 나 WSASend() 를 등록하게된다.
		// 자신이 정한 프로토콜 방식대로 하면된다. 
		IOCP_Recv(PerSocketData);
		//접속자IP와 총 사용자수를 화면에 출력
		DisplayText("접속자IP:(%s), 사용자수:%d\n",inet_ntoa(ClientAddr.sin_addr),listPerSocket.size());
	}
}
//WorkerThread 함수
DWORD WINAPI WorkerThread(LPVOID arg) {
	PER_SOCKET_DATA* PerSocketData=NULL;
	PER_IO_DATA* PerIOData=NULL;
	BOOL bRet;

	while(1) {
		bRet=GetQueuedCompletionStatus(hComPort,&dwTransfer,(LPDWORD)&PerSocketData,(LPOVERLAPPED*)&PerIOData,INFINITE);
		//소켓 연결이 끊김
		if(dwTransfer==0) {
			if(PerIOData) {
				//임계 영역에 들어감
				EnterCriticalSection(&cs);
				//클라이언트를 listPerIO에서 제거
				listPerSocket.remove(PerSocketData);
				//임계 영역을 빠져나감
				LeaveCriticalSection(&cs);
				//소켓 닫기
				closesocket(PerSocketData->ClientSocket);
				//객체 파괴
				delete PerSocketData;
				delete PerIOData;
				//사용자수 화면에 출력
				DisplayText("사용자수:%d\n",listPerSocket.size());
			}
			continue;
		}
		// io가 NULL이면 I/O 완료 패킷이 아니라 사용자 메시지이다.
		if(PerIOData) {
			// 받은 데이터가 있다.
			if(PerIOData->Type==IO_TYPE_RECV) {
				DisplayText("[Message] %s\n",PerIOData->Buffer);
				// 다시 받을 수 있게 I/O 요청을 한다.
				IOCP_Recv(PerSocketData);
				//임계 영역에 들어감
				EnterCriticalSection(&cs);
				// 받았으므로 받은 데이터를 처리한 후 알맞는 결과를 보내준다.
				list<PER_SOCKET_DATA*>::iterator it;
				for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
					if(*it!=PerSocketData) IOCP_Send(*it,PerIOData->Buffer);
				}
				//임계 영역을 빠져나감
				LeaveCriticalSection(&cs);
			}
			else
			// 보낸 데이터가 있다.
			if(PerIOData->Type==IO_TYPE_SEND) {
				// 처리할 일이 있으면 처리한다.
				DisplayText("처리할 일이 있으면 처리한다.\n");
			}
			// IO를 삭제한다.
			delete PerIOData;
		}
	}
	return 0;
}
//Recv 함수
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData) {
	int nError;
	DWORD dwRecv,dwFlags=0;

	PER_IO_DATA* PerIOData=new PER_IO_DATA;
	ZeroMemory(PerIOData,sizeof(PER_IO_DATA));
	PerIOData->WsaBuf.buf=PerIOData->Buffer;
	PerIOData->WsaBuf.len=MAXLINE;
	PerIOData->Type=IO_TYPE_RECV;

	nError=WSARecv(PerSocketData->ClientSocket,&PerIOData->WsaBuf,1,&dwRecv,&dwFlags,&PerIOData->Overlapped,NULL);
	if(nError==0) {
		// 한번에 받기 성공
		DisplayText("한번에 받기 성공\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O 받기 요청 성공
		DisplayText("IOCP I/O 받기 요청 성공\n");
	}
	else {
		DisplayText("받기 실패\n");
		//실패시 소켓 닫기
		closesocket(PerSocketData->ClientSocket);
		//객체 파괴
		delete PerSocketData;
		delete PerIOData;
	}
}
//Send 함수
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,char* pBuffer) {
	int nError;
	DWORD dwSend;

	PER_IO_DATA* PerIOData=new PER_IO_DATA;
	ZeroMemory(PerIOData,sizeof(PER_IO_DATA));
	PerIOData->WsaBuf.buf=PerIOData->Buffer;
	int len = _msize(PerIOData->WsaBuf.buf) / sizeof(char);
	strcpy_s(PerIOData->WsaBuf.buf, len + 1,pBuffer);
	PerIOData->WsaBuf.len=dwTransfer;
	PerIOData->Type=IO_TYPE_SEND;

	nError=WSASend(PerSocketData->ClientSocket,&PerIOData->WsaBuf,1,&dwSend,0,&PerIOData->Overlapped,NULL);
	if(nError==0) {
		// 한번에 보내기 성공
		DisplayText("한번에 보내기 성공\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O 보내기 요청 성공
		DisplayText("IOCP I/O 보내기 요청 성공\n");
	}
	else {
		DisplayText("보내기 실패\n");
		//실패시 소켓 닫기
		closesocket(PerSocketData->ClientSocket);
		//객체 파괴
		delete PerSocketData;
		delete PerIOData;
	}
}
//화면에 출력 함수
void DisplayText(char *fmt,...) {
	va_list arg;
	va_start(arg,fmt);
	
	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf,fmt,arg);
	
	EnterCriticalSection(&cs);  //임계 영역에 들어감
	int nLength=GetWindowTextLength(hEditContent);
	SendMessage(hEditContent,EM_SETSEL,nLength,nLength);
	SendMessage(hEditContent,EM_REPLACESEL,FALSE,(LPARAM)cbuf);
	LeaveCriticalSection(&cs);  //임계 영역에 빠져나감
	
	va_end(arg);
}