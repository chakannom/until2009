#include <windows.h>

#define BEGIN "BGN"
#define END "END"

#define MAX_PACK_SIZE 1024

//로그인 데이터 
typedef struct _Login {
	char userid[20];   
	char userpw[20];
} LOGIN, *PLOGIN;

//로그아웃 데이터
typedef struct _LogOut {
	char userid[20];
} LOGOUT, *PLOGOUT;

//프로세스 데이터
typedef struct _Process {
	BOOL create;
	int processid;   
	char processName[25];
} PROCESS, *PPROCESS;

//Admin Password 데이터
typedef struct _AdminPW {
	char passwd[20];
} ADMINPW, *PADMINPW;

//채팅 데이터
typedef struct _ChatMsg {
	char msg[1004];
} CHATMSG, *PCHATMSG;

class CPacket {
public:
	//패킷 코드 
	enum Codes {
		login_req=100, login_ack, login_nok, login_fail,  //로그인 코드	 
		logout_req=200, logout_ack, logout_nok,  //로그아웃 코드
		chat_req=300, chat_ack, chat_nok,  //채팅 코드
		process_req=400, process_ack, process_nok,  //프로세스 코드
		adminpw_req=500, adminpw_ack, adminpw_nok,  //Admin Pssword 코드
		cpcshutdown_req=600, cpcshutdown_ack, cpcshutdown_nok,  //Client PC Shutdown 코드
		cpcautorunon_req=700, cpcautorunon_ack, cpcautorunon_nok,  //Client PC AutoRun On 코드
		cpcautorunoff_req=800, cpcautorunoff_ack, cpcautorunoff_nok  //Client PC AutoRun Off 코드
	};

	//패킷 해더 구조체 
	typedef struct _Head {
		char key[4];  // 구분자 "BNG\n"
		short code;  // 코드
		int cb;  // 데이터 크기
	} HEAD, *PHEAD;

	//패킷 테일 구조체 
	typedef struct _Tail {
		int cb;  // 패킷 크기
		char key[4];  // 구분자 "END\n"
	} TAIL, *PTAIL;

public:
	char* pPacket;  // 패킷
	HEAD* pHead;  // 해더
	char* pData;  // 데이터
	TAIL* pTail;  // 테일
	
public:
	CPacket();  //생성자
	~CPacket();  //소멸자
	int Pack(short code, char* data=NULL, int datalen=0);  //패킷 결합
	int Unpack(char* buf, int buflen);  //패킷 분해
	int IsValid();  //유효성 검사
	char* GetData() {return pData;}
	int GetCode() {return pHead->code;}
	char* GetPacket() {return pPacket;}
	int GetLength() {return pTail->cb;}
};

//생성자
CPacket::CPacket() 
	:pPacket(NULL), pHead(NULL), pData(NULL), pTail(NULL) {
}

//소멸자
CPacket::~CPacket() {
	if(pPacket!=NULL)
		delete pPacket;
}

//패킷 결합
int CPacket::Pack(short code, char* data, int datalen) {
	try {
		pPacket=new char[sizeof(HEAD)+datalen+sizeof(TAIL)];
		memset(pPacket,0,sizeof(HEAD)+datalen+sizeof(TAIL));

		//Header 섹션
		pHead=(PHEAD)pPacket;
		strcpy_s(pHead->key,BEGIN);
		pHead->code=code;
		pHead->cb=datalen;
		//Data 섹션
		if((datalen>0)&&(data!=NULL)) {
			pData=(char*)(pPacket+sizeof(HEAD));
			memcpy(pData,data,datalen);
		} else if((data==NULL)&&(datalen==0)) {
		} else if((data==NULL)||(datalen<=0)) {
			throw("데이터 정보 오류");
		}
		//Tail 섹션
		pTail=(PTAIL)(pPacket+sizeof(HEAD)+datalen);
		strcpy_s(pTail->key,END);
		pTail->cb=sizeof(HEAD)+datalen+sizeof(TAIL);
	
		//유효성 검증
		IsValid();

	} catch (.../*char* str*/) {
		delete pPacket;
		pPacket=NULL; pHead=NULL; pTail=NULL; pData=NULL; 
		return 103;
	}

	return 0;
}

//패킷 분해
int CPacket::Unpack(char* buf, int buflen) {
	try {
		pPacket=new char[buflen];
		memcpy(pPacket, buf, buflen);

		//Header 섹션
		pHead=(PHEAD)pPacket;
		//Data 섹션 
		if(pHead->cb>0)
			pData=(char*)(pPacket+sizeof(HEAD));
		//Tail 섹션
		pTail=(PTAIL)(pPacket+sizeof(HEAD)+pHead->cb);

		//유효성 검증
		IsValid();

	} catch(...) {
		delete pPacket;
		pPacket=NULL; pHead=NULL; pTail=NULL; pData=NULL; 
		return 103;
	} 

	return 0;
}

//유효성 검사
int CPacket::IsValid() {
	//해더와 테일의 구분자와 일치하지 않는 경우
	if(strcmp(BEGIN,pHead->key)&&strcmp(END,pTail->key))
		throw;
	
	//패킷의 크기와 전송받은 데이타의 크기가 다른 경우 
	int tlen=pHead->cb+sizeof(HEAD)+sizeof(TAIL);
	if(tlen!=pTail->cb)
		throw;
	
	//패킷의 최소 최대 크기를 미달 또는 초과한 경우
	if((tlen<sizeof(HEAD)+sizeof(TAIL))||(tlen>MAX_PACK_SIZE))
		throw;

	return 0;
}