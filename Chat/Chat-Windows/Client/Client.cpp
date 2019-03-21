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
//Client ���̾�α� ���ν��� �κ�
BOOL CALLBACK ClientDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	int ret;
	char TmpStr[512];
	switch(iMsg)	{
		case WM_INITDIALOG:
			//���̾�α� ���� ��������
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
					//������ ���ڿ� �б�
					GetDlgItemText(hDlg,IDC_EDIT_SEND,TmpStr,sizeof(TmpStr));
					//������ ���ڿ� �ڽ��� CHAT�� ���(NickName�� TmpStr�� ��ħ)
					strcpy(ChatStr,NickName);
					strcat(ChatStr," : ");
					strcat(ChatStr,TmpStr);
					strcpy(TmpStr,"");
					DisplayText(hEditChat,"%s\r\n",ChatStr);
					// ä�� ���ڿ� �ڽ� ����
					SetDlgItemText(hDlg,IDC_EDIT_SEND,"");
					SetFocus(hEditSend);
					//Disconnect �����̸� Send�� ���� �ʱ� ���� break��
					if(sClientSock==0) break;
					//Send()������ ������
					ret=send(sClientSock,ChatStr,sizeof(ChatStr),0);
					if(ret==SOCKET_ERROR)	{
						DisplayText(hEditChat,"send() ����\r\n");
						break;
					}
					strcpy(ChatStr,"");
					break;
				case IDC_BNCONNECT:	//connect ���̾�α� ����
					DialogBox(hInst,MAKEINTRESOURCE(IDD_CONNECTDLG),hDlg,ConnectDlgProc);
					break;
				case IDC_BNDISCONNECT:	//���� ����
					DestSocket();
					bClose=TRUE;
					break;
				case IDC_BNID:	//��ȭ�� ���̾�α� ����
					DialogBox(hInst,MAKEINTRESOURCE(IDD_IDDLG),hDlg,IDDlgProc);
					break;
				case IDC_BNCLOSE:	//Ŭ���̾�Ʈ ���̾�α� �ޱ�(���α׷� ����)
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
//Connect ���̾�α� ���ν��� �κ�
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
					//Connect���̾�α׿��� ������ ����
					GetDlgItemText(hDlg,IDC_IPADDRESS,IPAddr,sizeof(IPAddr));
					PortNum=GetDlgItemInt(hDlg,IDC_PORT,NULL,FALSE);
					// Ŭ���̾�Ʈ ������ ����
					hClientThread=CreateThread(NULL,0,ClientMain,NULL,0,&ThreadId);
					if(hClientThread==NULL)	{
						MessageBox(hClientDlg,"�����带 ������ �� �����ϴ�.\r\n������ ������ �� �����ϴ�.\r\n���α׷��� �����մϴ�","������ ����",MB_OK);
						SendMessage(hClientDlg,WM_CLOSE,0,0);
					}
					else {
						DisplayText(hEditChat,"������ ������...\r\n");
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
			EndDialog(hDlg,0);	//connenct ���̾�α� �ݱ�
			return TRUE;
	}
	return FALSE;
}
//��ȭ�� ���̾�α� ���ν��� �κ�
BOOL CALLBACK IDDlgProc(HWND hDlg,UINT iMsg,WPARAM wPm,LPARAM lPm)
{
	switch(iMsg)	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_ID,NickName);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wPm))	{
				case IDOK:
					//��ȭ�� ���̾�α׿��� ������ ����
					GetDlgItemText(hDlg,IDC_ID,NickName,sizeof(NickName));
					SendMessage(hDlg,WM_CLOSE,0,0);
					break;
			}
			return TRUE;
		case WM_CLOSE:
			//��ȭ�� ���̾�α� �ݱ�
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}
//TCP ���� ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{
	int ret;
	SOCKADDR_IN serveraddr;
	HANDLE hRecvThread;
	DWORD ThreadId;
	// ���� �ʱ�ȭ
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)	{
		MessageBox(hClientDlg,"���� �ʱ�ȭ ����","WSAStartup",MB_OK);
		DestSocket();
		return -1;
	}
	// TCP ���ϻ���
	sClientSock=socket(AF_INET,SOCK_STREAM,0);
	if(sClientSock==INVALID_SOCKET)	{
		MessageBox(hClientDlg,"TCP ���ϻ��� ����","socket",MB_OK);
		DestSocket();
		return -1;
	}
	//�ּ������Է�
	ZeroMemory(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port = htons(PortNum);
	serveraddr.sin_addr.S_un.S_addr=inet_addr(IPAddr);
	//connect()
	ret=connect(sClientSock,(SOCKADDR*)&serveraddr,sizeof(serveraddr));
	if(ret==SOCKET_ERROR)	{
		MessageBox(hClientDlg,"connect() ����","connect",MB_OK);
		DestSocket();
		return -1;
	}
	//��ư ���� ����
	EnableWindow(hBNConnect,FALSE);
	EnableWindow(hBNDisconnect,TRUE);

	DisplayText(hEditChat,"������ �����Ͽ����ϴ�.\r\n");
	//�ޱ� ������ ����
	hRecvThread=CreateThread(NULL,0,RecvThread,(LPVOID)sClientSock,0,&ThreadId);
	if(hRecvThread==NULL)	{
		MessageBox(hClientDlg,"�����带 ������ �� �����ϴ�.\r\n���α׷��� �����մϴ�.","������ ����",MB_OK);
		SendMessage(hClientDlg,WM_CLOSE,0,0);
	}
	//������ ���� ���
	ret=WaitForSingleObject(hRecvThread,INFINITE);
	//������ ����
	TerminateThread(hRecvThread,-1);
	CloseHandle(hRecvThread);

	DisplayText(hEditChat,"������ ������ ���������ϴ�.\r\n");
	////��ư ���� ����
	EnableWindow(hBNDisconnect,FALSE);
	EnableWindow(hBNConnect,TRUE);

	DestSocket();	//���� ����

	return 0;
}
// ������ �ޱ�
DWORD WINAPI RecvThread(LPVOID arg)
{
	int ret;
	SOCKET client_sock=(SOCKET)arg;
	while(1)	{
		//Recv()������ �ޱ⸦ ��ٸ� 
		ret=recv(client_sock,ChatStr,sizeof(ChatStr),0);
		if(ret==SOCKET_ERROR||ret==0) break;
		else if(ret==SOCKET_ERROR&&bClose==TRUE)	{
			DisplayText(hEditChat,"recv() ����\r\n");
			break;
		}
		//������ ���� �ι��� �Է�
		ChatStr[ret]='\0';
		//���� ������ ���
		DisplayText(hEditChat,"%s\r\n",ChatStr);
	}
	//client_sock �ݱ�
	closesocket(client_sock);
	return 0;
}
//���� �ݱ�
void DestSocket()
{
	if(sClientSock!=0) closesocket(sClientSock);
	sClientSock=0;
	WSACleanup();
}
//��ºκ�
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