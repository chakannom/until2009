#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>
#include <list>
#include "resource.h"

using namespace std;

#define WM_MYLISTEN_NOTIFY (WM_APP+0)  //WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻��� Post�� �޽���
#define IO_TYPE_SEND 0
#define IO_TYPE_RECV 1
#define PORTNUM 5056
#define MAXLINE 1024

//IOCP ���� ������ ����ü
typedef struct _tarPerSocketData {
	SOCKET ClientSocket;
}PER_SOCKET_DATA;
//IOCP I/O ������ ����ü
typedef struct tarParIOData {
	WSAOVERLAPPED Overlapped;
	BOOL Type;
	char Buffer[MAXLINE];
	WSABUF WsaBuf;
}PER_IO_DATA;

BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //���� ���̾�α� ���ν���
void OnServerStart();  //���� ���� �Լ�
void OnMyListenNotify(WPARAM wParam,LPARAM lParam);  //Accept �߻��� �����ϴ� �Լ�
DWORD WINAPI WorkerThread(LPVOID arg);  //WorkerThread �Լ�
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData);  //Recv �Լ�
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,char* pBuffer);  //Send �Լ�
void DisplayText(char *fmt,...);  //ȭ�鿡 ��� �Լ�

HINSTANCE hInst;
HWND hServerDlg,hEditContent;
SOCKET ListenSocket;
HANDLE hComPort;
CRITICAL_SECTION cs;
list<PER_SOCKET_DATA*> listPerSocket;  //�����ڵ��� �����ϴ� ����Ʈ
DWORD dwTransfer;  //������ ����

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	hInst=hInstance;
	//�Ӱ迵�� �ʱ�ȭ
	InitializeCriticalSection(&cs);
	//���� �ʱ�ȭ
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"���� �ʱ�ȭ ����!","ERROR",MB_OK);
		return 0;
	}
	//���̾�α� ���� �� ����
	DialogBox(hInst,MAKEINTRESOURCE(IDD_SERVER_DLG),NULL,ServerDlgProc);
	//���� ����
	closesocket(ListenSocket);
	WSACleanup();
	//�Ӱ迵�� ����
	DeleteCriticalSection(&cs);
	return 0;
}
//���� ���̾�α� ���ν���
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
			hServerDlg=hDlg;
			hEditContent=GetDlgItem(hDlg,IDC_EDIT_CONTENT);
			SetDlgItemText(hDlg,IDC_EDIT_SENDMSG,"�� �κ��� �켱 ����~~ START��ư�� ������ ������ ���۵˴ϴ�.");
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BT_START:
					OnServerStart();
					break;
				case IDC_BT_SEND:
					MessageBox(hDlg,"�� �κ��� �켱 ����~~","������",MB_OK);
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
//���� ���� �Լ�
void OnServerStart() {
	//Socket ����
	ListenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ListenSocket==INVALID_SOCKET) {
		DisplayText("[ERROR] Socket ���� ����!\n");
		return;
	}else DisplayText("[SUCCESS] Socket ���� ����!\n");
	//�ּ� ���� ����
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(PORTNUM);
	//Bind
	if(bind(ListenSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText("[ERROR] Bind ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] Bind ����!\n");
	//Listen
	if(listen(ListenSocket,5)==SOCKET_ERROR) {
		DisplayText("[ERROR] Listen ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] Listen ����!\n");
	//�޽��� ����� non-blocking ����� �������� ����
	WSAAsyncSelect(ListenSocket,hServerDlg,WM_MYLISTEN_NOTIFY,FD_ACCEPT);
	//IOCP ����
	hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(hComPort==NULL) {
		DisplayText("[ERROR] IOCP ���� ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText("[SUCCESS] IOCP ���� ����!\n");
	//�ý�������ȹ��(CPU������ ���� �κ�)
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	//WorkerThread ����(CPU����*2��ŭ)
	HANDLE hThread;
	DWORD ThreadId;
	for(int i=0;i<(int)SystemInfo.dwNumberOfProcessors*2;i++) {
		hThread=CreateThread(NULL,0,WorkerThread,hComPort,0,&ThreadId);
		if(hThread==NULL) {
			DisplayText("[ERROR] %d��° WorkerThread ���� ����!\n",i+1);
			return;
		}else DisplayText("[SUCCESS] %d��° WorkerThread ���� ����!\n",i+1);
		CloseHandle(hThread);
	}
}
//Accept �߻��� �����ϴ� �Լ�
void OnMyListenNotify(WPARAM wParam,LPARAM lParam) {
	int nEvent=WSAGETSELECTEVENT(lParam);
	int nError=WSAGETSELECTERROR(lParam);
	if(nEvent==FD_ACCEPT&&nError==0) {
		SOCKET ClientSocket;
		SOCKADDR_IN ClientAddr;
		int AddrLen=sizeof(ClientAddr);
		//���ο� Ŭ���̾�Ʈ�� ����
		ClientSocket=accept(ListenSocket,(PSOCKADDR)&ClientAddr,&AddrLen);
		//IOCP ���� ������ ����ü �޸� �Ҵ�
		PER_SOCKET_DATA* PerSocketData=new PER_SOCKET_DATA;
		PerSocketData->ClientSocket=ClientSocket;
		//�Ӱ� ������ ��
		EnterCriticalSection(&cs);
		//������ Ŭ���̾�Ʈ�� listPerIO�� ������
		listPerSocket.push_back(PerSocketData);
		//�Ӱ� ������ ��������
		LeaveCriticalSection(&cs);
		//���Ʈ�� ������ IOCP�� ������Ŵ
		CreateIoCompletionPort((HANDLE)ClientSocket,hComPort,(DWORD)PerSocketData,0);
		// ���ʷ� WSARecv() �� WSASend() �� ����ϰԵȴ�.
		// �ڽ��� ���� �������� ��Ĵ�� �ϸ�ȴ�. 
		IOCP_Recv(PerSocketData);
		//������IP�� �� ����ڼ��� ȭ�鿡 ���
		DisplayText("������IP:(%s), ����ڼ�:%d\n",inet_ntoa(ClientAddr.sin_addr),listPerSocket.size());
	}
}
//WorkerThread �Լ�
DWORD WINAPI WorkerThread(LPVOID arg) {
	PER_SOCKET_DATA* PerSocketData=NULL;
	PER_IO_DATA* PerIOData=NULL;
	BOOL bRet;

	while(1) {
		bRet=GetQueuedCompletionStatus(hComPort,&dwTransfer,(LPDWORD)&PerSocketData,(LPOVERLAPPED*)&PerIOData,INFINITE);
		//���� ������ ����
		if(dwTransfer==0) {
			if(PerIOData) {
				//�Ӱ� ������ ��
				EnterCriticalSection(&cs);
				//Ŭ���̾�Ʈ�� listPerIO���� ����
				listPerSocket.remove(PerSocketData);
				//�Ӱ� ������ ��������
				LeaveCriticalSection(&cs);
				//���� �ݱ�
				closesocket(PerSocketData->ClientSocket);
				//��ü �ı�
				delete PerSocketData;
				delete PerIOData;
				//����ڼ� ȭ�鿡 ���
				DisplayText("����ڼ�:%d\n",listPerSocket.size());
			}
			continue;
		}
		// io�� NULL�̸� I/O �Ϸ� ��Ŷ�� �ƴ϶� ����� �޽����̴�.
		if(PerIOData) {
			// ���� �����Ͱ� �ִ�.
			if(PerIOData->Type==IO_TYPE_RECV) {
				DisplayText("[Message] %s\n",PerIOData->Buffer);
				// �ٽ� ���� �� �ְ� I/O ��û�� �Ѵ�.
				IOCP_Recv(PerSocketData);
				//�Ӱ� ������ ��
				EnterCriticalSection(&cs);
				// �޾����Ƿ� ���� �����͸� ó���� �� �˸´� ����� �����ش�.
				list<PER_SOCKET_DATA*>::iterator it;
				for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
					if(*it!=PerSocketData) IOCP_Send(*it,PerIOData->Buffer);
				}
				//�Ӱ� ������ ��������
				LeaveCriticalSection(&cs);
			}
			else
			// ���� �����Ͱ� �ִ�.
			if(PerIOData->Type==IO_TYPE_SEND) {
				// ó���� ���� ������ ó���Ѵ�.
				DisplayText("ó���� ���� ������ ó���Ѵ�.\n");
			}
			// IO�� �����Ѵ�.
			delete PerIOData;
		}
	}
	return 0;
}
//Recv �Լ�
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
		// �ѹ��� �ޱ� ����
		DisplayText("�ѹ��� �ޱ� ����\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O �ޱ� ��û ����
		DisplayText("IOCP I/O �ޱ� ��û ����\n");
	}
	else {
		DisplayText("�ޱ� ����\n");
		//���н� ���� �ݱ�
		closesocket(PerSocketData->ClientSocket);
		//��ü �ı�
		delete PerSocketData;
		delete PerIOData;
	}
}
//Send �Լ�
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
		// �ѹ��� ������ ����
		DisplayText("�ѹ��� ������ ����\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O ������ ��û ����
		DisplayText("IOCP I/O ������ ��û ����\n");
	}
	else {
		DisplayText("������ ����\n");
		//���н� ���� �ݱ�
		closesocket(PerSocketData->ClientSocket);
		//��ü �ı�
		delete PerSocketData;
		delete PerIOData;
	}
}
//ȭ�鿡 ��� �Լ�
void DisplayText(char *fmt,...) {
	va_list arg;
	va_start(arg,fmt);
	
	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf,fmt,arg);
	
	EnterCriticalSection(&cs);  //�Ӱ� ������ ��
	int nLength=GetWindowTextLength(hEditContent);
	SendMessage(hEditContent,EM_SETSEL,nLength,nLength);
	SendMessage(hEditContent,EM_REPLACESEL,FALSE,(LPARAM)cbuf);
	LeaveCriticalSection(&cs);  //�Ӱ� ������ ��������
	
	va_end(arg);
}