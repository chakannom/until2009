#include "Sun-PC_Server.h"
#include "resource.h"
#include "XmlUserControl.h"
#include "../packet/packet.cpp"
extern "C" void* GetMyPacketObject();


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	//���α׷� �ߺ� ���� ����(���ý� ���)
	HANDLE hMutex=CreateMutex(NULL,FALSE,"SUN-PC_SERVER-58BF3D18-A4CC-49a4-9C90-81F86420A546");
	if(GetLastError()==ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex);
		MessageBox(NULL,"SUN-PC_SERVER�� �̹� �������Դϴ�.","ERROR",MB_OK);
		hMutex=NULL;
		return 0;
	}
	hInst=hInstance;
	//���� ������ġ�� FilePath������ ������
	GetModuleFileName(NULL,FPath.filepath,MAX_PATH);
	_splitpath_s(FPath.filepath,FPath.drv,FPath.path,FPath.fname,FPath.ext);
	sprintf_s(FPath.drv_path,"%s%s",FPath.drv,FPath.path);
	SetCurrentDirectory(FPath.drv_path);  //���α׷��� ���� ���丮�� ������
	//�ý�������ȹ��(CPU������ ���� �κ�)
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	CPU_Cnt=SystemInfo.dwNumberOfProcessors;
	hWorkerThread=new HANDLE[CPU_Cnt*2];
	//�Ӱ迵�� �ʱ�ȭ
	InitializeCriticalSection(&cs);
	//GDI+ ���̺귯���� �ʱ�ȭ
	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if(GdiplusStartup(&gpToken,&gpsi,NULL)!=Ok) {
		MessageBox(NULL,"GDI+ ���̺귯���� �ʱ�ȭ�� �� �����ϴ�.","�˸�",MB_OK);
		return 0;
	}
	//���̾�α� ���� �� ����
	DialogBox(hInst,MAKEINTRESOURCE(IDD_SERVER_DLG),NULL,ServerDlgProc);
	//GDI+ ���̺귯�� ����
	GdiplusShutdown(gpToken);
	//���� ����
	closesocket(ListenSocket);
	WSACleanup();
	//�Ӱ迵�� ����
	DeleteCriticalSection(&cs);

	delete hWorkerThread;
	return 0;
}
//���� ���̾�α� ���ν���
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:  //���̾�α� ������
		{
			return ServerDlgInit(hDlg);
		}
		case WM_NOTIFY:
		{
			return ServerDlgNotify(hDlg,wParam,lParam);
		}
		case WM_COMMAND:  //����� ������
		{
			return ServerDlgCommand(hDlg,wParam,lParam);
		}
		case WM_MYLISTEN_NOTIFY:  //���ο� Ŭ���̾�Ʈ�� ���ӽ�
		{
			return OnMyListenNotify(wParam,lParam);
		}
		case WM_PAINT:  //�̹��� ��½�
		{
			return ServerDlgPaint(hDlg,wParam,lParam);  //���� ���ȭ�� ����
		}
		case WM_CTLCOLORSTATIC:
		{
			return ServerDlgCtlcolorstatic(wParam,lParam);
		}
		case WM_CLOSE:  //���̾�α� ������
		{
			EndDialog(hDlg,0);
			return TRUE;
		}
	}
	return FALSE;
}
//ServerDlg �ʱ�ȭ
BOOL ServerDlgInit(HWND hDlg) {

	hServerDlg=hDlg;

	//ServerDlg�� �ִ� ��Ʈ�ѷ� ������ ��ġ�� ��ġ
	HFONT hFont=CreateFont(18,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_ROMAN,"HYsanB");
	hStcPcCnt=GetDlgItem(hDlg,IDC_STC_PCCNT);
	MoveWindow(hStcPcCnt,195,83,50,20,TRUE);
	SendMessage(hStcPcCnt,WM_SETFONT,(WPARAM)hFont,(LPARAM)FALSE);
	hStcUserCnt=GetDlgItem(hDlg,IDC_STC_USERCNT);
	MoveWindow(hStcUserCnt,195,108,50,20,TRUE);
	SendMessage(hStcUserCnt,WM_SETFONT,(WPARAM)hFont,(LPARAM)FALSE);
	hListLoginUser=GetDlgItem(hDlg,IDC_LIST_LOGINUSER);
	MoveWindow(hListLoginUser,308,84,259,161,TRUE);
	hBTServerOn=GetDlgItem(hDlg,IDC_BT_SERVERON);
	MoveWindow(hBTServerOn,445,259,122,30,TRUE);
	hBTServerOff=GetDlgItem(hDlg,IDC_BT_SERVEROFF);
	MoveWindow(hBTServerOff,445,259,122,30,TRUE);

	LVCOLUMN lvCol;
	lvCol.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT; //���� ����
	lvCol.cx=120;
	lvCol.pszText="Clinet IP"; //�׸��� �̸�
	lvCol.iSubItem=0; //�׸��� ��ȣ
	SendMessage(hListLoginUser, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);
	lvCol.cx=120;
	lvCol.pszText="Login ID"; //�׸��� �̸�
	lvCol.iSubItem=1; //�׸��� ��ȣ
	SendMessage(hListLoginUser, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);

	/////////�ϼ��Ǹ� ������ �κ�//////
	hEditContent=GetDlgItem(hDlg,IDC_EDIT_CONTENT);
	/////////�ϼ��Ǹ� ������ �κ�//////

	//Xml �ʱ�ȭ
	XmlUserInit();

	return TRUE;
}
BOOL ServerDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam) {
	switch(LOWORD(wParam)) {
		case IDC_LIST_LOGINUSER:
		{
			if(((LPNMHDR)lParam)->code==NM_DBLCLK) {
				char tmpBuf[20];
				int iSlected=ListView_GetNextItem(hListLoginUser,-1,LVNI_FOCUSED);
				if(iSlected==-1) {
					break;
				}
				ListView_GetItemText(hListLoginUser,iSlected,0,tmpBuf,20);
				list<PER_SOCKET_DATA*>::iterator it;
				for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
					if(strcmp(tmpBuf,inet_ntoa((*it)->ClientAddr.sin_addr))==0) {
						ShowWindow((*it)->hChatDlg,SW_SHOW);
						break;
					}
				}
			}
			break;
		}
	}
	return TRUE;
}
//ServerDlg Ŀ���
BOOL ServerDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam) {
	IO_DATA IO_Data;
	switch(LOWORD(wParam)) {
		case IDC_BT_SERVERON:
		{
			OnServerStart();
			break;
		}
		case IDC_BT_SERVEROFF:
		{
			listPerSocket.clear();
			//���� ����
			closesocket(ListenSocket);
			WSACleanup();
			//Terminate WorkerThread
			for(int i=0;i<CPU_Cnt*2;i++) {
				DWORD dwExitCode;
				GetExitCodeThread(hWorkerThread[i],&dwExitCode);
				TerminateThread(hWorkerThread[i],dwExitCode);
				CloseHandle(hWorkerThread[i]);
			}
			LoginUserListReview(hListLoginUser);
			User_Cnt=0;
			SetDlgItemInt(hServerDlg,IDC_STC_PCCNT,0,FALSE);
			SetDlgItemInt(hServerDlg,IDC_STC_USERCNT,User_Cnt,FALSE);
			//��ư ���� ����
			ShowWindow(hBTServerOn,SW_SHOW);
			ShowWindow(hBTServerOff,SW_HIDE);
			break;
		}
		case ID_M_USERMNG:
		{
			DialogBox(hInst,MAKEINTRESOURCE(IDD_USERMNG_DLG),hDlg,UserMngDlgProc);
			break;
		}
		case ID_M_ADMINPWCHANGE:
		{
			DialogBox(hInst,MAKEINTRESOURCE(IDD_ADMINPW_DLG),hDlg,AdminPWChangeDlgProc);
			break;
		}
		case ID_M_CLIENTPCAUTORUN_ON:
		{
			CPacket* pSendPacket=(CPacket*)GetPacketObject();
			pSendPacket->Pack(pSendPacket->cpcautorunon_req);
			IO_Data.pBuffer=pSendPacket->GetPacket();
			IO_Data.BufLen=pSendPacket->GetLength();
			//�Ӱ� ������ ��
			EnterCriticalSection(&cs);
			list<PER_SOCKET_DATA*>::iterator it;
			for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
				IOCP_Send(*it,&IO_Data);
			}
			//�Ӱ� ������ ��������
			LeaveCriticalSection(&cs);
			delete pSendPacket;
			break;
		}
		case ID_M_CLIENTPCAUTORUN_OFF:
		{
			CPacket* pSendPacket=(CPacket*)GetPacketObject();
			pSendPacket->Pack(pSendPacket->cpcautorunoff_req);
			IO_Data.pBuffer=pSendPacket->GetPacket();
			IO_Data.BufLen=pSendPacket->GetLength();
			//�Ӱ� ������ ��
			EnterCriticalSection(&cs);
			list<PER_SOCKET_DATA*>::iterator it;
			for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
				IOCP_Send(*it,&IO_Data);
			}
			//�Ӱ� ������ ��������
			LeaveCriticalSection(&cs);
			delete pSendPacket;
			break;
		}
		case ID_M_CLIENTPCSHUTDOWN:
		{
			if(MessageBox(hDlg,"���ӵ� �ִ� ��� Client PC�� �����Ͻðڽ��ϱ�?","ALL Client PC Shutdown",MB_YESNO)==IDYES) {
				CPacket* pSendPacket=(CPacket*)GetPacketObject();
				pSendPacket->Pack(pSendPacket->cpcshutdown_req);
				IO_Data.pBuffer=pSendPacket->GetPacket();
				IO_Data.BufLen=pSendPacket->GetLength();
				//�Ӱ� ������ ��
				EnterCriticalSection(&cs);
				list<PER_SOCKET_DATA*>::iterator it;
				for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
					IOCP_Send(*it,&IO_Data);
				}
				//�Ӱ� ������ ��������
				LeaveCriticalSection(&cs);
				delete pSendPacket;
			}
			break;
		}
	}
	return TRUE;
}
//���� ���ȭ�� ����
BOOL ServerDlgPaint(HWND hWnd,WPARAM wParam,LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	hdc=BeginPaint(hWnd,&ps);
	Graphics graphics(hdc);
	Image image(L"SUNPCBGS.GIF");
	// ���� ó��
	if(image.GetLastStatus()!=Ok) {
		MessageBox(hServerDlg,"�̹��� ������ ���� �� �����ϴ�.","����",MB_OK);
		return FALSE;
	}
	graphics.DrawImage(&image,0,0,594,309);
	EndPaint(hWnd,&ps);
	return TRUE;
}
BOOL ServerDlgCtlcolorstatic(WPARAM wParam,LPARAM lParam) {
	HBRUSH hStaticBrush=CreateSolidBrush(TRANSPARENT);
	if(lParam==(LPARAM)hStcPcCnt||lParam==(LPARAM)hStcUserCnt) {
		SetTextColor((HDC)wParam,RGB(255,255,255));
		SetBkColor((HDC)wParam,TRANSPARENT);
		return (BOOL)hStaticBrush;
	}
	return FALSE;
}

//���� ���� �Լ�
void OnServerStart() {
	//���� �ʱ�ȭ
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"���� �ʱ�ȭ ����!","ERROR",MB_OK);
		return;
	}
	//Socket ����
	ListenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ListenSocket==INVALID_SOCKET) {
		DisplayText(hEditContent,"[ERROR] Socket ���� ����!\n");
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Socket ���� ����!\n");
	//�ּ� ���� ����
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(PORTNUM);
	//Bind
	if(bind(ListenSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText(hEditContent,"[ERROR] Bind ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Bind ����!\n");
	//Listen
	if(listen(ListenSocket,5)==SOCKET_ERROR) {
		DisplayText(hEditContent,"[ERROR] Listen ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Listen ����!\n");
	//�޽��� ����� non-blocking ����� �������� ����
	WSAAsyncSelect(ListenSocket,hServerDlg,WM_MYLISTEN_NOTIFY,FD_ACCEPT);
	//IOCP ����
	hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(hComPort==NULL) {
		DisplayText(hEditContent,"[ERROR] IOCP ���� ����!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] IOCP ���� ����!\n");
	//WorkerThread ����(CPU����*2��ŭ)
	for(int i=0;i<CPU_Cnt*2;i++) {
		DWORD ThreadId;
		hWorkerThread[i]=CreateThread(NULL,0,WorkerThread,hComPort,0,&ThreadId);
		if(hWorkerThread[i]==NULL) {
			DisplayText(hEditContent,"[ERROR] %d��° WorkerThread ���� ����!\n",i+1);
			return;
		}else DisplayText(hEditContent,"[SUCCESS] %d��° WorkerThread ���� ����!\n",i+1);
	}	
	//��ư ���� ����
	ShowWindow(hBTServerOn,SW_HIDE);
	ShowWindow(hBTServerOff,SW_SHOW);
}
//Accept �߻��� �����ϴ� �Լ�
BOOL OnMyListenNotify(WPARAM wParam,LPARAM lParam) {
	//�Ӱ� ������ ��
	EnterCriticalSection(&cs);

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
		PerSocketData->ClientAddr=ClientAddr;
		strcpy_s(PerSocketData->UserID,"");
		PerSocketData->hChatDlg=CreateDialog(hInst,MAKEINTRESOURCE(IDD_CHAT_DLG),hServerDlg,ChatDlgProc);
		PerSocketData->hChatContent=GetDlgItem(PerSocketData->hChatDlg,IDC_EDIT_CHAT_CONTENT);
		//������ Ŭ���̾�Ʈ�� listPerIO�� ������
		listPerSocket.push_back(PerSocketData);
		//���Ʈ�� ������ IOCP�� ������Ŵ
		CreateIoCompletionPort((HANDLE)ClientSocket,hComPort,(DWORD)PerSocketData,0);
		// ���ʷ� WSARecv() �� WSASend() �� ����ϰԵȴ�.
		// �ڽ��� ���� �������� ��Ĵ�� �ϸ�ȴ�.
		IOCP_Recv(PerSocketData);
		//������IP�� �� �������ִ� PC ���� ȭ�鿡 ���
		SetDlgItemInt(hServerDlg,IDC_STC_PCCNT,(UINT)listPerSocket.size(),FALSE);
		LoginUserListReview(hListLoginUser);
//		DisplayText(hEditContent,"������IP:(%s), ����ڼ�:%d\n",inet_ntoa(ClientAddr.sin_addr),listPerSocket.size());
	}

	//�Ӱ� ������ ��������
	LeaveCriticalSection(&cs);

	return TRUE;
}
//WorkerThread �Լ�
DWORD WINAPI WorkerThread(LPVOID arg) {
	PER_SOCKET_DATA* PerSocketData=NULL;
	PER_IO_DATA* PerIOData=NULL;
	IO_DATA IO_Data;
	BOOL bRet;
	SYSTEMTIME SystemTime;
	char date[20], time[20];

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
				EndDialog(PerSocketData->hChatDlg,0);
				//��ü �ı�
				delete PerSocketData;
				delete PerIOData;
				//���ӵ��ִ� PC ���� ȭ�鿡 ���
				SetDlgItemInt(hServerDlg,IDC_STC_PCCNT,(UINT)listPerSocket.size(),FALSE);
				LoginUserListReview(hListLoginUser);
//				DisplayText(hEditContent,"����ڼ�:%d\n",listPerSocket.size());
			}
			continue;
		}
		// io�� NULL�̸� I/O �Ϸ� ��Ŷ�� �ƴ϶� ����� �޽����̴�.
		if(PerIOData) {
			// ���� �����Ͱ� �ִ�.
			if(PerIOData->Type==IO_TYPE_RECV) {
				GetLocalTime(&SystemTime);
				wsprintf(date, "%d-%d-%d", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
				wsprintf(time, "%d-%d-%d", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
				CPacket* pRecvPacket=(CPacket*)GetPacketObject();
				if(pRecvPacket->Unpack(PerIOData->Buffer,MAXLINE)) {
					MessageBox(hServerDlg,"pRecvPacket->Unpack ����","ERROR",MB_OK);
					return FALSE;
				}
//				DisplayText(hEditContent,"pRecvPacket->pHead->key : %s\n",pRecvPacket->pHead->key);
//				DisplayText(hEditContent,"pRecvPacket->pHead->code : %d\n",pRecvPacket->pHead->code);
//				DisplayText(hEditContent,"pRecvPacket->pHead->cb : %d\n",pRecvPacket->pHead->cb);

				if(pRecvPacket->pHead->cb>=0) {
					switch(pRecvPacket->GetCode()) {
						case pRecvPacket->login_req:  // ���� 
						{
							PLOGIN pLogin=(PLOGIN)pRecvPacket->pData;
//							DisplayText(hEditContent,"pLogin->userid : %s\n",pLogin->userid);
//							DisplayText(hEditContent,"pLogin->userpw : %s\n",pLogin->userpw);

							list<PER_SOCKET_DATA*>::iterator it;
							for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
								if(!strcmp((*it)->UserID,pLogin->userid)) goto NOTLOGIN;
							}
							if(!XmlUserSearch(pLogin->userid,pLogin->userpw)) {
NOTLOGIN:
								CPacket* pSendPacket=(CPacket*)GetPacketObject();
								pSendPacket->Pack(pSendPacket->login_fail);
								IO_Data.pBuffer=pSendPacket->GetPacket();
								IO_Data.BufLen=pSendPacket->GetLength();
								IOCP_Send(PerSocketData,&IO_Data);
								delete pSendPacket;
								break;
							}
							strcpy_s(PerSocketData->UserID,pLogin->userid);
							LoginUserListReview(hListLoginUser);
							CPacket* pSendPacket=(CPacket*)GetPacketObject();
							pSendPacket->Pack(pSendPacket->login_ack);
							IO_Data.pBuffer=pSendPacket->GetPacket();
							IO_Data.BufLen=pSendPacket->GetLength();
							IOCP_Send(PerSocketData,&IO_Data);
							PerSocketData->XmlLogCtrl.XmlLogLoad(pLogin->userid, date, time, inet_ntoa(PerSocketData->ClientAddr.sin_addr));
							delete pSendPacket;
							User_Cnt++;
							SetDlgItemInt(hServerDlg,IDC_STC_USERCNT,User_Cnt,FALSE);
							break;
						}
						case pRecvPacket->logout_req:  //�α׾ƿ�
						{
							strcpy_s(PerSocketData->UserID,"");
							LoginUserListReview(hListLoginUser);
							CPacket* pSendPacket=(CPacket*)GetPacketObject();
							pSendPacket->Pack(pSendPacket->logout_ack);
							IO_Data.pBuffer=pSendPacket->GetPacket();
							IO_Data.BufLen=pSendPacket->GetLength();
							IOCP_Send(PerSocketData,&IO_Data);
							delete pSendPacket;
							User_Cnt--;
							SetDlgItemInt(hServerDlg,IDC_STC_USERCNT,User_Cnt,FALSE);
							EndDialog(PerSocketData->hChatDlg,0);
							break;
						}
						case pRecvPacket->process_req: //���μ��� ����
						{
							char tmppcssid[6];
							PPROCESS pProcess=(PPROCESS)pRecvPacket->pData;
//							DisplayText(hEditContent,"pProcess->bcreate : %d\n",pProcess->create);
//							DisplayText(hEditContent,"pProcess->processid : %d\n",pProcess->processid);
//							DisplayText(hEditContent,"pProcess->processName : %s\n",pProcess->processName);
							_itoa_s(pProcess->processid,tmppcssid,10);
							PerSocketData->XmlLogCtrl.XmlLogInsert(time, tmppcssid, pProcess->processName, pProcess->create);
							break;
						}
						case pRecvPacket->chat_req:  //ä�� ����
						{
							PCHATMSG pChatMsg=(PCHATMSG)pRecvPacket->pData;

							if(PerSocketData->hChatDlg!=NULL) {
								ShowWindow(PerSocketData->hChatDlg, SW_SHOW);
							}
							if(PerSocketData->hChatContent!=NULL) {
								DisplayText(PerSocketData->hChatContent,"[From %s] : %s\n",PerSocketData->UserID,pChatMsg->msg);
							}

							break;
						}
						default:
						{
							DisplayText(hEditContent,"����� ���������� ����\n"); 
							break;
						}
					}
				} else DisplayText(hEditContent,"������ ���� �߻�\n");

//				DisplayText(hEditContent,"pRecvPacket->pTail->key : %s\n",pRecvPacket->pTail->key);
//				DisplayText(hEditContent,"pRecvPacket->pTail->cb : %d\n",pRecvPacket->pTail->cb);
			
				delete pRecvPacket;

				// �ٽ� ���� �� �ְ� I/O ��û�� �Ѵ�.
				IOCP_Recv(PerSocketData);
			}
			else
			// ���� �����Ͱ� �ִ�.
			if(PerIOData->Type==IO_TYPE_SEND) {
				// ó���� ���� ������ ó���Ѵ�.
				DisplayText(hEditContent,"ó���� ���� ������ ó���Ѵ�.\n");
			}
			// IO�� �����Ѵ�.
			delete PerIOData;
		}
	}
	return 0;
}
//Recv �Լ�
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData) {
	//�Ӱ� ������ ��
	EnterCriticalSection(&cs);

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
		DisplayText(hEditContent,"�ѹ��� �ޱ� ����\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O �ޱ� ��û ����
		DisplayText(hEditContent,"IOCP I/O �ޱ� ��û ����\n");
	}
	else {
		DisplayText(hEditContent,"�ޱ� ����\n");
		//���н� ���� �ݱ�
		closesocket(PerSocketData->ClientSocket);
		//��ü �ı�
		delete PerSocketData;
		delete PerIOData;
	}

	//�Ӱ� ������ ��������
	LeaveCriticalSection(&cs);
}
//Send �Լ�
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,IO_DATA* IO_Data) {
	//�Ӱ� ������ ��
	EnterCriticalSection(&cs);

	int nError;
	DWORD dwSend;

	PER_IO_DATA* PerIOData=new PER_IO_DATA;
	ZeroMemory(PerIOData,sizeof(PER_IO_DATA));
	PerIOData->WsaBuf.buf=IO_Data->pBuffer;
	PerIOData->WsaBuf.len=IO_Data->BufLen;
	PerIOData->Type=IO_TYPE_SEND;

	nError=WSASend(PerSocketData->ClientSocket,&PerIOData->WsaBuf,1,&dwSend,0,&PerIOData->Overlapped,NULL);
	if(nError==0) {
		// �ѹ��� ������ ����
		DisplayText(hEditContent,"�ѹ��� ������ ����\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O ������ ��û ����
		DisplayText(hEditContent,"IOCP I/O ������ ��û ����\n");
	}
	else {
		DisplayText(hEditContent,"������ ����\n");
		//���н� ���� �ݱ�
		closesocket(PerSocketData->ClientSocket);
		//��ü �ı�
		delete PerSocketData;
		delete PerIOData;
	}

	//�Ӱ� ������ ��������
	LeaveCriticalSection(&cs);
}

void LoginUserListReview(HWND hList) {
	//�Ӱ� ������ ��
	EnterCriticalSection(&cs);

	ListView_DeleteAllItems(hList);

	LVITEM lvitem;
	int i=0;

	list<PER_SOCKET_DATA*>::iterator it;
	for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
		lvitem.mask=LVIF_TEXT|LVIF_PARAM;
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		lvitem.pszText=(char*)inet_ntoa((*it)->ClientAddr.sin_addr);
		ListView_InsertItem(hList,&lvitem);
		if(strcmp((*it)->UserID,"")) {
			ListView_SetItemText(hList,i,1,(*it)->UserID);
		}
		i++;
	}
	ListView_SetExtendedListViewStyle(hList,LVS_EX_FULLROWSELECT);

	//�Ӱ� ������ ��������
	LeaveCriticalSection(&cs);
}

//ȭ�鿡 ��� �Լ�
void DisplayText(HWND hContent,char *fmt,...) {
	EnterCriticalSection(&cs);  //�Ӱ� ������ ��

	va_list arg;
	va_start(arg,fmt);
	
	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf,MAXLINE+256,fmt,arg);

	int nLength=GetWindowTextLength(hEditContent);
	SendMessage(hContent,EM_SETSEL,nLength,nLength);
	SendMessage(hContent,EM_REPLACESEL,FALSE,(LPARAM)cbuf);
	
	va_end(arg);

	LeaveCriticalSection(&cs);  //�Ӱ� ������ ��������
}


//����ڰ��� ���̾�α� ���ν���
BOOL CALLBACK UserMngDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			return UserMngDlgInit(hDlg);
		}
		case WM_NOTIFY:
		{
			return UserMngDlgNotify(hDlg,wParam,lParam);
		}
		case WM_COMMAND:
		{
			return UserMngDlgCommand(hDlg,wParam,lParam);
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg,0);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL UserMngDlgInit(HWND hDlg) {

	hUserMngDlg=hDlg;

	hListUserMng=GetDlgItem(hDlg,IDC_LIST_USER);

	LVCOLUMN lvCol;
	lvCol.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT; //���� ����
	lvCol.cx=150;
	lvCol.pszText="���̵�"; //�׸��� �̸�
	lvCol.iSubItem=0; //�׸��� ��ȣ
	SendMessage(hListUserMng, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);
	lvCol.cx=150;
	lvCol.pszText="��й�ȣ"; //�׸��� �̸�
	lvCol.iSubItem=1; //�׸��� ��ȣ
	SendMessage(hListUserMng, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);
	
	XmlUserListReview(hListUserMng);

	return TRUE;
}
BOOL UserMngDlgNotify(HWND hDlg,WPARAM wParam,LPARAM lParam) {
	char UserID[20],UserPW[20];
	switch(LOWORD(wParam)) {
		case IDC_LIST_USER:
		{
			if(((LPNMHDR)lParam)->code==NM_DBLCLK) {
				int iSlected=ListView_GetNextItem(hListUserMng,-1,LVNI_FOCUSED);

				if(iSlected==-1) break;

				ListView_GetItemText(hListUserMng,iSlected,0,UserID,20);
				ListView_GetItemText(hListUserMng,iSlected,1,UserPW,20);
				SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,UserID);
				SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,UserPW);
			}
			break;
		}
	}
	return TRUE;
}
BOOL UserMngDlgCommand(HWND hDlg,WPARAM wParam,LPARAM lParam) {
	char UserID[20],UserPW[20];
	switch(LOWORD(wParam)) {
		case IDC_BT_USERMNG_ADD:
		{
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDID,UserID,19);
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDPW,UserPW,19);
			if((!strcmp(UserID,""))||(!strcmp(UserPW,""))) {
				MessageBox(hDlg,"���̵� or ��й�ȣ�� �Է��ϼ���","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserInsert(UserID,UserPW)) MessageBox(hDlg,"�߰� ����","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDPW,"");
			break;
		}
		case IDC_BT_USERMNG_MODIFY:
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,UserID,19);
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,UserPW,19);
			if((!strcmp(UserID,""))||(!strcmp(UserPW,""))) {
				MessageBox(hDlg,"�Ʒ� List���� �����Ͻÿ�","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserModify(UserID,UserPW)) MessageBox(hDlg,"���� ����","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,"");
			break;
		case IDC_BT_USERMNG_DEL:
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,UserID,19);
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,UserPW,19);
			if((!strcmp(UserID,""))||(!strcmp(UserPW,""))) {
				MessageBox(hDlg,"�Ʒ� List���� �����Ͻÿ�","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserDelete(UserID,UserPW)) MessageBox(hDlg,"���� ����","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,"");
			break;
	}

	XmlUserListReview(hListUserMng);

	return TRUE;
}

//������ �н����� ���� ���̾�α� ���ν���
BOOL CALLBACK AdminPWChangeDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	IO_DATA IO_Data;
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			hAdminPWChangeDlg=hDlg;
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam)) {
				case IDC_BT_ADMINPW_MODIFY:
					ADMINPW adminPW;
					GetDlgItemText(hDlg, IDC_EDIT_ADMINPW_MDPW, adminPW.passwd, 20);
					if(!strcmp(adminPW.passwd, "")) {
						MessageBox(hDlg, "Admin Password�� �Է��ϼ���.", "Error", MB_OK);
						break;
					}
					CPacket* pSendPacket=(CPacket*)GetPacketObject();
					pSendPacket->Pack(pSendPacket->adminpw_req, (char*)&adminPW, sizeof(ADMINPW));
					IO_Data.pBuffer=pSendPacket->GetPacket();
					IO_Data.BufLen=pSendPacket->GetLength();
					//�Ӱ� ������ ��
					EnterCriticalSection(&cs);
					list<PER_SOCKET_DATA*>::iterator it;
					for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
						IOCP_Send(*it,&IO_Data);
					}
					//�Ӱ� ������ ��������
					LeaveCriticalSection(&cs);
					delete pSendPacket;
					SetDlgItemText(hDlg, IDC_EDIT_ADMINPW_MDPW, "");
					break;
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			EndDialog(hDlg,0);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CALLBACK ChatDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	IO_DATA IO_Data;
	switch(uMsg) {
		case WM_INITDIALOG:
		{
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam)) {
				case IDC_BT_CHAT_SEND:
				{
					CHATMSG chatmsg;
					CPacket* pSendPacket = (CPacket*)GetPacketObject();

					GetDlgItemText(hDlg,IDC_EDIT_CHAT_SENDMSG,chatmsg.msg,1004);

					pSendPacket->Pack(pSendPacket->chat_req, (char*)&chatmsg, (int)sizeof(CHATMSG));
					IO_Data.pBuffer = pSendPacket->GetPacket();
					IO_Data.BufLen = pSendPacket->GetLength();

					list<PER_SOCKET_DATA*>::iterator it;
					for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
						if((*it)->hChatDlg==hDlg) {
							DisplayText((*it)->hChatContent,"[Send msg] : %s\n",chatmsg.msg);
							IOCP_Send((*it),&IO_Data);
							break;
						}
					}					
					SetDlgItemText(hDlg,IDC_EDIT_CHAT_SENDMSG,"");

					delete pSendPacket;
					break;
				}
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			list<PER_SOCKET_DATA*>::iterator it;
			for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
				if((*it)->hChatDlg==hDlg) {
					SendMessage((*it)->hChatContent, EM_SETSEL, 0, -1);
					SendMessage((*it)->hChatContent, EM_REPLACESEL, FALSE, (LPARAM)"");
					break;
				}
			}
			EndDialog(hDlg,0);
			return TRUE;
		}
	}
	return FALSE;
}