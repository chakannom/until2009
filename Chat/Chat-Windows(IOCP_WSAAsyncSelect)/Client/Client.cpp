#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "resource.h"

using namespace std;

#define WM_MYSOCKET_NOTIFY (WM_APP+0)  //WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻��� Post�� �޽���
#define MAXLINE 1024

BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //Ŭ���̾�Ʈ ���̾�α� ���ν���
BOOL CALLBACK ConnectDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);  //���� ���̾�α� ���ν���
void OnConnect();  //���� �Լ�
void OnMySocketNotify(WPARAM wParam,LPARAM lParam);  //SEND,WRITE,CLOSE �߻��� �����ϴ� �Լ�
void Disconnect();  //���� ���� �Լ�
void DisplayText(char *fmt,...);  //ȭ�鿡 ��� �Լ�

HINSTANCE hInst;
HWND hClientDlg,hEditContent,hEditSendMsg,hBTSend,hBTConnect,hBTDisconnect;
char IPAddr[20]="127.0.0.1";
int PortNum=5056;
SOCKET ClientSocket;
char Buffer[MAXLINE];  //�޽��� ���� ����

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,int nCmdShow) {
	hInst=hInstance;
	//���� �ʱ�ȭ
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"���� �ʱ�ȭ ����!","ERROR",MB_OK);
		return 0;
	}
	//���̾�α� ���� �� ����
	DialogBox(hInst,MAKEINTRESOURCE(IDD_CLIENT_DLG),NULL,ClientDlgProc);
	//���� ����
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}
//Ŭ���̾�Ʈ ���̾�α� ���ν���
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
//���� ���̾�α� ���ν���
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
					//������ ����
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
//���� �Լ�
void OnConnect() {
	//Socket ����
	ClientSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ClientSocket==INVALID_SOCKET) {
		DisplayText("[ERROR] Socket ���� ����!\n");
		return;
	}else DisplayText("[SUCCESS] Socket ���� ����!\n");
	//�ּ� ���� ����
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=inet_addr(IPAddr);
	ServerAddr.sin_port=htons(PortNum);
	DisplayText("������ ������.....\n");
	//Connect
	if(connect(ClientSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText("[ERROR] Connect ����!\n");
		closesocket(ClientSocket);
		return;
	}else DisplayText("[SUCCESS] Connect ����!\n");
	//WSAAsyncSelect ����
	if(WSAAsyncSelect(ClientSocket,hClientDlg,WM_MYSOCKET_NOTIFY,FD_READ|FD_WRITE|FD_CLOSE)==SOCKET_ERROR) {
		DisplayText("[ERROR] WSAAsyncSelect ����!\n");
		closesocket(ClientSocket);
		return;
	}else DisplayText("[SUCCESS] WSAAsyncSelect ����!\n");
	//��ư ���� ����
	EnableWindow(hEditSendMsg,TRUE);
	EnableWindow(hBTSend,TRUE);
	EnableWindow(hBTDisconnect,TRUE);
	EnableWindow(hBTConnect,FALSE);

	DisplayText("Connect!!!\n");
}
//SEND,WRITE,CLOSE �߻��� �����ϴ� �Լ�
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
//���� ���� �Լ�
void Disconnect() {
	//���� �ݱ�
	closesocket(ClientSocket);
	//��ư ���� ����
	EnableWindow(hEditSendMsg,FALSE);
	EnableWindow(hBTSend,FALSE);
	EnableWindow(hBTDisconnect,FALSE);
	EnableWindow(hBTConnect,TRUE);
	DisplayText("������ ������ ������.\n");
}
//ȭ�鿡 ��� �Լ�
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