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
//Server���̾�α� ���ν��� �κ�
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	DWORD ThreadId;
	switch(iMsg)	{
		case WM_INITDIALOG:
			hServerDlg=hDlg;
			hEdit=GetDlgItem(hDlg,IDC_EDITINFO);
			DisplayText(hEdit,"Server ������...\r\n");
			CreateThread(NULL,0,ServerMain,NULL,0,&ThreadId); //ServerMain ������ ����
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
// TCP ���� ���� �κ�
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
	//���� dll �ʱ�ȭ
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)	{
		MessageBox(hServerDlg,"���� �ʱ�ȭ ����","WSAStartup",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	// TCP ���ϻ���
	sServerSock=socket(AF_INET,SOCK_STREAM,0);
	if(sServerSock==INVALID_SOCKET)	{
		MessageBox(hServerDlg,"TCP ���ϻ��� ����","socket",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//�ּ������Է�
	ZeroMemory(&addr_in,sizeof(addr_in));
	addr_in.sin_family=AF_INET;
	addr_in.sin_port=htons(PORTNUM);
	addr_in.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	//bind
	ret=bind(sServerSock,(SOCKADDR*)&addr_in,sizeof(addr_in));
	if(ret==SOCKET_ERROR)	{
		MessageBox(hServerDlg,"bind ����","bind",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//listen
	ret=listen(sServerSock,5);
	if(ret==SOCKET_ERROR)	{
		MessageBox(hServerDlg,"listen ����","listen",MB_OK);
		SendMessage(hServerDlg,WM_CLOSE,0,0);
	}
	//�ڽ��� �������ּ� ã��
	if(gethostname(hostname,sizeof(hostname))==0)	{
		if((hostinfo=gethostbyname(hostname))!=NULL)	{
			sprintf(title,"YS Server(������ IP�ּ� : %s / Port��ȣ : %d)",inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list),PORTNUM);
			SetWindowText(hServerDlg,title);
		}
	}
	DisplayText(hEdit,"�������� ���� !! Ŭ���̾�Ʈ�� ������ ��ٸ��ϴ�..\r\n");

	while(1){
		// accept()
		addrlen=sizeof(clientaddr);
		client_sock=accept(sServerSock,(SOCKADDR*)&clientaddr,&addrlen);
		if(client_sock==INVALID_SOCKET)	{
			DisplayText(hEdit,"Accept() ����\r\n");
			continue;
		}
		client_socks[clientcnt++]=client_sock;
		DisplayText(hEdit,"[TCP����]Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		// ������ ����
		hThread=CreateThread(NULL,0,ProcessClient,(LPVOID)client_sock,0,&ThreadId);
		if(hThread==NULL) DisplayText(hEdit,"[����] ������ ���� ����!\r\n");
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

	// Ŭ���̾�Ʈ ���� ���
	addrlen=sizeof(clientaddr);
	getpeername(client_sock,(SOCKADDR *)&clientaddr,&addrlen);

	while(1)	{
		// ������ �ޱ�
		ret=recv(client_sock,buf,BUFSIZE,0);
		if(ret==SOCKET_ERROR||ret==0) break;
		// ���� ������ ���
		buf[ret]='\0';
		DisplayText(hEdit,"[TCP/%s:%d] %s\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port),buf);
		// ������ ������
		for(int i=0;i<clientcnt;i++)	{
			if(client_socks[i]!=client_sock)	{
				ret=send(client_socks[i],buf,ret,0);
				if(ret==SOCKET_ERROR)	{
					getpeername(client_socks[i],(SOCKADDR *)&clientaddr,&addrlen);
					DisplayText(hEdit,"Send()����(%s:%d)\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
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
	DisplayText(hEdit,"[TCP ����]Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

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
	// Ŀ���� �� ������ �ű�
	SendMessage(Edit,EM_SETSEL,0,-1);
	SendMessage(Edit,EM_SETSEL,-1,-1);
	// ���ڿ��� �߰���
	vsprintf(chTmp,Msg,arg);
	SendMessage(Edit,EM_REPLACESEL,0,(LPARAM)chTmp);

	va_end(arg);
}