#include "Sun-PC_Server.h"
#include "resource.h"
#include "XmlUserControl.h"
#include "../packet/packet.cpp"
extern "C" void* GetMyPacketObject();


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	//프로그램 중복 실행 방지(뮤택스 사용)
	HANDLE hMutex=CreateMutex(NULL,FALSE,"SUN-PC_SERVER-58BF3D18-A4CC-49a4-9C90-81F86420A546");
	if(GetLastError()==ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex);
		MessageBox(NULL,"SUN-PC_SERVER가 이미 실행중입니다.","ERROR",MB_OK);
		hMutex=NULL;
		return 0;
	}
	hInst=hInstance;
	//현재 파일위치를 FilePath변수에 저장함
	GetModuleFileName(NULL,FPath.filepath,MAX_PATH);
	_splitpath_s(FPath.filepath,FPath.drv,FPath.path,FPath.fname,FPath.ext);
	sprintf_s(FPath.drv_path,"%s%s",FPath.drv,FPath.path);
	SetCurrentDirectory(FPath.drv_path);  //프로그램의 현재 디렉토리를 세팅함
	//시스템정보획득(CPU개수를 위한 부분)
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	CPU_Cnt=SystemInfo.dwNumberOfProcessors;
	hWorkerThread=new HANDLE[CPU_Cnt*2];
	//임계영역 초기화
	InitializeCriticalSection(&cs);
	//GDI+ 라이브러리를 초기화
	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if(GdiplusStartup(&gpToken,&gpsi,NULL)!=Ok) {
		MessageBox(NULL,"GDI+ 라이브러리를 초기화할 수 없습니다.","알림",MB_OK);
		return 0;
	}
	//다이얼로그 생성 및 실행
	DialogBox(hInst,MAKEINTRESOURCE(IDD_SERVER_DLG),NULL,ServerDlgProc);
	//GDI+ 라이브러리 종료
	GdiplusShutdown(gpToken);
	//윈속 종료
	closesocket(ListenSocket);
	WSACleanup();
	//임계영역 정리
	DeleteCriticalSection(&cs);

	delete hWorkerThread;
	return 0;
}
//서버 다이얼로그 프로시져
BOOL CALLBACK ServerDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	switch(uMsg) {
		case WM_INITDIALOG:  //다이얼로그 생성시
		{
			return ServerDlgInit(hDlg);
		}
		case WM_NOTIFY:
		{
			return ServerDlgNotify(hDlg,wParam,lParam);
		}
		case WM_COMMAND:  //명령이 있을시
		{
			return ServerDlgCommand(hDlg,wParam,lParam);
		}
		case WM_MYLISTEN_NOTIFY:  //새로운 클라이언트가 접속시
		{
			return OnMyListenNotify(wParam,lParam);
		}
		case WM_PAINT:  //이미지 출력시
		{
			return ServerDlgPaint(hDlg,wParam,lParam);  //서버 배경화면 설정
		}
		case WM_CTLCOLORSTATIC:
		{
			return ServerDlgCtlcolorstatic(wParam,lParam);
		}
		case WM_CLOSE:  //다이얼로그 닫을시
		{
			EndDialog(hDlg,0);
			return TRUE;
		}
	}
	return FALSE;
}
//ServerDlg 초기화
BOOL ServerDlgInit(HWND hDlg) {

	hServerDlg=hDlg;

	//ServerDlg에 있는 컨트롤러 적당한 위치에 배치
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
	lvCol.fmt=LVCFMT_LEFT; //왼쪽 정렬
	lvCol.cx=120;
	lvCol.pszText="Clinet IP"; //항목의 이름
	lvCol.iSubItem=0; //항목의 번호
	SendMessage(hListLoginUser, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);
	lvCol.cx=120;
	lvCol.pszText="Login ID"; //항목의 이름
	lvCol.iSubItem=1; //항목의 번호
	SendMessage(hListLoginUser, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);

	/////////완성되면 삭제할 부분//////
	hEditContent=GetDlgItem(hDlg,IDC_EDIT_CONTENT);
	/////////완성되면 삭제할 부분//////

	//Xml 초기화
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
//ServerDlg 커멘드
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
			//윈속 종료
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
			//버튼 상태 변경
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
			//임계 영역에 들어감
			EnterCriticalSection(&cs);
			list<PER_SOCKET_DATA*>::iterator it;
			for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
				IOCP_Send(*it,&IO_Data);
			}
			//임계 영역을 빠져나옴
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
			//임계 영역에 들어감
			EnterCriticalSection(&cs);
			list<PER_SOCKET_DATA*>::iterator it;
			for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
				IOCP_Send(*it,&IO_Data);
			}
			//임계 영역을 빠져나옴
			LeaveCriticalSection(&cs);
			delete pSendPacket;
			break;
		}
		case ID_M_CLIENTPCSHUTDOWN:
		{
			if(MessageBox(hDlg,"접속되 있는 모든 Client PC를 종료하시겠습니까?","ALL Client PC Shutdown",MB_YESNO)==IDYES) {
				CPacket* pSendPacket=(CPacket*)GetPacketObject();
				pSendPacket->Pack(pSendPacket->cpcshutdown_req);
				IO_Data.pBuffer=pSendPacket->GetPacket();
				IO_Data.BufLen=pSendPacket->GetLength();
				//임계 영역에 들어감
				EnterCriticalSection(&cs);
				list<PER_SOCKET_DATA*>::iterator it;
				for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
					IOCP_Send(*it,&IO_Data);
				}
				//임계 영역을 빠져나옴
				LeaveCriticalSection(&cs);
				delete pSendPacket;
			}
			break;
		}
	}
	return TRUE;
}
//서버 배경화면 설정
BOOL ServerDlgPaint(HWND hWnd,WPARAM wParam,LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	hdc=BeginPaint(hWnd,&ps);
	Graphics graphics(hdc);
	Image image(L"SUNPCBGS.GIF");
	// 에러 처리
	if(image.GetLastStatus()!=Ok) {
		MessageBox(hServerDlg,"이미지 파일을 읽을 수 없습니다.","에러",MB_OK);
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

//서버 시작 함수
void OnServerStart() {
	//윈속 초기화
	WSADATA WsaData;
	if(WSAStartup(MAKEWORD(2,2),&WsaData)!=0) {
		MessageBox(NULL,"윈속 초기화 실패!","ERROR",MB_OK);
		return;
	}
	//Socket 생성
	ListenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(ListenSocket==INVALID_SOCKET) {
		DisplayText(hEditContent,"[ERROR] Socket 생성 실패!\n");
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Socket 생성 성공!\n");
	//주소 정보 설정
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(SOCKADDR_IN));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(PORTNUM);
	//Bind
	if(bind(ListenSocket,(PSOCKADDR)&ServerAddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
		DisplayText(hEditContent,"[ERROR] Bind 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Bind 성공!\n");
	//Listen
	if(listen(ListenSocket,5)==SOCKET_ERROR) {
		DisplayText(hEditContent,"[ERROR] Listen 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] Listen 성공!\n");
	//메시지 방식의 non-blocking 모드의 소켓으로 변경
	WSAAsyncSelect(ListenSocket,hServerDlg,WM_MYLISTEN_NOTIFY,FD_ACCEPT);
	//IOCP 생성
	hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(hComPort==NULL) {
		DisplayText(hEditContent,"[ERROR] IOCP 생성 실패!\n");
		closesocket(ListenSocket);
		return;
	}else DisplayText(hEditContent,"[SUCCESS] IOCP 생성 성공!\n");
	//WorkerThread 생성(CPU개수*2만큼)
	for(int i=0;i<CPU_Cnt*2;i++) {
		DWORD ThreadId;
		hWorkerThread[i]=CreateThread(NULL,0,WorkerThread,hComPort,0,&ThreadId);
		if(hWorkerThread[i]==NULL) {
			DisplayText(hEditContent,"[ERROR] %d번째 WorkerThread 생성 실패!\n",i+1);
			return;
		}else DisplayText(hEditContent,"[SUCCESS] %d번째 WorkerThread 생성 성공!\n",i+1);
	}	
	//버튼 상태 변경
	ShowWindow(hBTServerOn,SW_HIDE);
	ShowWindow(hBTServerOff,SW_SHOW);
}
//Accept 발생시 수행하는 함수
BOOL OnMyListenNotify(WPARAM wParam,LPARAM lParam) {
	//임계 영역에 들어감
	EnterCriticalSection(&cs);

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
		PerSocketData->ClientAddr=ClientAddr;
		strcpy_s(PerSocketData->UserID,"");
		PerSocketData->hChatDlg=CreateDialog(hInst,MAKEINTRESOURCE(IDD_CHAT_DLG),hServerDlg,ChatDlgProc);
		PerSocketData->hChatContent=GetDlgItem(PerSocketData->hChatDlg,IDC_EDIT_CHAT_CONTENT);
		//접속한 클라이언트를 listPerIO에 저장함
		listPerSocket.push_back(PerSocketData);
		//억셉트된 소켓을 IOCP에 참여시킴
		CreateIoCompletionPort((HANDLE)ClientSocket,hComPort,(DWORD)PerSocketData,0);
		// 최초로 WSARecv() 나 WSASend() 를 등록하게된다.
		// 자신이 정한 프로토콜 방식대로 하면된다.
		IOCP_Recv(PerSocketData);
		//접속자IP와 총 접솓되있는 PC 개수 화면에 출력
		SetDlgItemInt(hServerDlg,IDC_STC_PCCNT,(UINT)listPerSocket.size(),FALSE);
		LoginUserListReview(hListLoginUser);
//		DisplayText(hEditContent,"접속자IP:(%s), 사용자수:%d\n",inet_ntoa(ClientAddr.sin_addr),listPerSocket.size());
	}

	//임계 영역을 빠져나감
	LeaveCriticalSection(&cs);

	return TRUE;
}
//WorkerThread 함수
DWORD WINAPI WorkerThread(LPVOID arg) {
	PER_SOCKET_DATA* PerSocketData=NULL;
	PER_IO_DATA* PerIOData=NULL;
	IO_DATA IO_Data;
	BOOL bRet;
	SYSTEMTIME SystemTime;
	char date[20], time[20];

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
				EndDialog(PerSocketData->hChatDlg,0);
				//객체 파괴
				delete PerSocketData;
				delete PerIOData;
				//접속되있는 PC 개수 화면에 출력
				SetDlgItemInt(hServerDlg,IDC_STC_PCCNT,(UINT)listPerSocket.size(),FALSE);
				LoginUserListReview(hListLoginUser);
//				DisplayText(hEditContent,"사용자수:%d\n",listPerSocket.size());
			}
			continue;
		}
		// io가 NULL이면 I/O 완료 패킷이 아니라 사용자 메시지이다.
		if(PerIOData) {
			// 받은 데이터가 있다.
			if(PerIOData->Type==IO_TYPE_RECV) {
				GetLocalTime(&SystemTime);
				wsprintf(date, "%d-%d-%d", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
				wsprintf(time, "%d-%d-%d", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
				CPacket* pRecvPacket=(CPacket*)GetPacketObject();
				if(pRecvPacket->Unpack(PerIOData->Buffer,MAXLINE)) {
					MessageBox(hServerDlg,"pRecvPacket->Unpack 실패","ERROR",MB_OK);
					return FALSE;
				}
//				DisplayText(hEditContent,"pRecvPacket->pHead->key : %s\n",pRecvPacket->pHead->key);
//				DisplayText(hEditContent,"pRecvPacket->pHead->code : %d\n",pRecvPacket->pHead->code);
//				DisplayText(hEditContent,"pRecvPacket->pHead->cb : %d\n",pRecvPacket->pHead->cb);

				if(pRecvPacket->pHead->cb>=0) {
					switch(pRecvPacket->GetCode()) {
						case pRecvPacket->login_req:  // 인증 
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
						case pRecvPacket->logout_req:  //로그아웃
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
						case pRecvPacket->process_req: //프로세스 정보
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
						case pRecvPacket->chat_req:  //채팅 내용
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
							DisplayText(hEditContent,"연결된 프로토콜이 없음\n"); 
							break;
						}
					}
				} else DisplayText(hEditContent,"데이터 오류 발생\n");

//				DisplayText(hEditContent,"pRecvPacket->pTail->key : %s\n",pRecvPacket->pTail->key);
//				DisplayText(hEditContent,"pRecvPacket->pTail->cb : %d\n",pRecvPacket->pTail->cb);
			
				delete pRecvPacket;

				// 다시 받을 수 있게 I/O 요청을 한다.
				IOCP_Recv(PerSocketData);
			}
			else
			// 보낸 데이터가 있다.
			if(PerIOData->Type==IO_TYPE_SEND) {
				// 처리할 일이 있으면 처리한다.
				DisplayText(hEditContent,"처리할 일이 있으면 처리한다.\n");
			}
			// IO를 삭제한다.
			delete PerIOData;
		}
	}
	return 0;
}
//Recv 함수
void IOCP_Recv(PER_SOCKET_DATA* PerSocketData) {
	//임계 영역에 들어감
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
		// 한번에 받기 성공
		DisplayText(hEditContent,"한번에 받기 성공\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O 받기 요청 성공
		DisplayText(hEditContent,"IOCP I/O 받기 요청 성공\n");
	}
	else {
		DisplayText(hEditContent,"받기 실패\n");
		//실패시 소켓 닫기
		closesocket(PerSocketData->ClientSocket);
		//객체 파괴
		delete PerSocketData;
		delete PerIOData;
	}

	//임계 영역을 빠져나감
	LeaveCriticalSection(&cs);
}
//Send 함수
void IOCP_Send(PER_SOCKET_DATA* PerSocketData,IO_DATA* IO_Data) {
	//임계 영역에 들어감
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
		// 한번에 보내기 성공
		DisplayText(hEditContent,"한번에 보내기 성공\n");
	}
	else
	if(nError==SOCKET_ERROR&&WSAGetLastError()==WSA_IO_PENDING) {
		// IOCP I/O 보내기 요청 성공
		DisplayText(hEditContent,"IOCP I/O 보내기 요청 성공\n");
	}
	else {
		DisplayText(hEditContent,"보내기 실패\n");
		//실패시 소켓 닫기
		closesocket(PerSocketData->ClientSocket);
		//객체 파괴
		delete PerSocketData;
		delete PerIOData;
	}

	//임계 영역을 빠져나감
	LeaveCriticalSection(&cs);
}

void LoginUserListReview(HWND hList) {
	//임계 영역에 들어감
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

	//임계 영역을 빠져나감
	LeaveCriticalSection(&cs);
}

//화면에 출력 함수
void DisplayText(HWND hContent,char *fmt,...) {
	EnterCriticalSection(&cs);  //임계 영역에 들어감

	va_list arg;
	va_start(arg,fmt);
	
	char cbuf[MAXLINE+256];
	vsprintf_s(cbuf,MAXLINE+256,fmt,arg);

	int nLength=GetWindowTextLength(hEditContent);
	SendMessage(hContent,EM_SETSEL,nLength,nLength);
	SendMessage(hContent,EM_REPLACESEL,FALSE,(LPARAM)cbuf);
	
	va_end(arg);

	LeaveCriticalSection(&cs);  //임계 영역에 빠져나감
}


//사용자관리 다이얼로그 프로시져
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
	lvCol.fmt=LVCFMT_LEFT; //왼쪽 정렬
	lvCol.cx=150;
	lvCol.pszText="아이디"; //항목의 이름
	lvCol.iSubItem=0; //항목의 번호
	SendMessage(hListUserMng, LVM_INSERTCOLUMN,lvCol.iSubItem,(LPARAM)&lvCol);
	lvCol.cx=150;
	lvCol.pszText="비밀번호"; //항목의 이름
	lvCol.iSubItem=1; //항목의 번호
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
				MessageBox(hDlg,"아이디 or 비밀번호를 입력하세요","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserInsert(UserID,UserPW)) MessageBox(hDlg,"추가 실패","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_ADDPW,"");
			break;
		}
		case IDC_BT_USERMNG_MODIFY:
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,UserID,19);
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,UserPW,19);
			if((!strcmp(UserID,""))||(!strcmp(UserPW,""))) {
				MessageBox(hDlg,"아래 List에서 선택하시오","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserModify(UserID,UserPW)) MessageBox(hDlg,"수정 실패","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,"");
			break;
		case IDC_BT_USERMNG_DEL:
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,UserID,19);
			GetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,UserPW,19);
			if((!strcmp(UserID,""))||(!strcmp(UserPW,""))) {
				MessageBox(hDlg,"아래 List에서 선택하시오","ERROR",MB_OK);
				return FALSE;
			}

			if(!XmlUserDelete(UserID,UserPW)) MessageBox(hDlg,"삭제 실패","ERROR",MB_OK);

			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDID,"");
			SetDlgItemText(hDlg,IDC_EDIT_USERMNG_MDPW,"");
			break;
	}

	XmlUserListReview(hListUserMng);

	return TRUE;
}

//관리자 패스워드 변경 다이얼로그 프로시져
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
						MessageBox(hDlg, "Admin Password를 입력하세요.", "Error", MB_OK);
						break;
					}
					CPacket* pSendPacket=(CPacket*)GetPacketObject();
					pSendPacket->Pack(pSendPacket->adminpw_req, (char*)&adminPW, sizeof(ADMINPW));
					IO_Data.pBuffer=pSendPacket->GetPacket();
					IO_Data.BufLen=pSendPacket->GetLength();
					//임계 영역에 들어감
					EnterCriticalSection(&cs);
					list<PER_SOCKET_DATA*>::iterator it;
					for(it=listPerSocket.begin();it!=listPerSocket.end();it++) {
						IOCP_Send(*it,&IO_Data);
					}
					//임계 영역을 빠져나옴
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