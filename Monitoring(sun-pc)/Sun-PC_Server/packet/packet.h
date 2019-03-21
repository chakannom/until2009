#include <windows.h>

#define BEGIN "BGN"
#define END "END"

#define MAX_PACK_SIZE 1024

//�α��� ������ 
typedef struct _Login {
	char userid[20];   
	char userpw[20];
} LOGIN, *PLOGIN;

//�α׾ƿ� ������
typedef struct _LogOut {
	char userid[20];
} LOGOUT, *PLOGOUT;

//���μ��� ������
typedef struct _Process {
	BOOL create;
	int processid;   
	char processName[25];
} PROCESS, *PPROCESS;

//Admin Password ������
typedef struct _AdminPW {
	char passwd[20];
} ADMINPW, *PADMINPW;

//ä�� ������
typedef struct _ChatMsg {
	char msg[1004];
} CHATMSG, *PCHATMSG;

class CPacket {
public:
	//��Ŷ �ڵ� 
	enum Codes {
		login_req=100, login_ack, login_nok, login_fail,  //�α��� �ڵ�	 
		logout_req=200, logout_ack, logout_nok,  //�α׾ƿ� �ڵ�
		chat_req=300, chat_ack, chat_nok,  //ä�� �ڵ�
		process_req=400, process_ack, process_nok,  //���μ��� �ڵ�
		adminpw_req=500, adminpw_ack, adminpw_nok,  //Admin Pssword �ڵ�
		cpcshutdown_req=600, cpcshutdown_ack, cpcshutdown_nok,  //Client PC Shutdown �ڵ�
		cpcautorunon_req=700, cpcautorunon_ack, cpcautorunon_nok,  //Client PC AutoRun On �ڵ�
		cpcautorunoff_req=800, cpcautorunoff_ack, cpcautorunoff_nok  //Client PC AutoRun Off �ڵ�
	};

	//��Ŷ �ش� ����ü 
	typedef struct _Head {
		char key[4];  // ������ "BNG\n"
		short code;  // �ڵ�
		int cb;  // ������ ũ��
	} HEAD, *PHEAD;

	//��Ŷ ���� ����ü 
	typedef struct _Tail {
		int cb;  // ��Ŷ ũ��
		char key[4];  // ������ "END\n"
	} TAIL, *PTAIL;

public:
	char* pPacket;  // ��Ŷ
	HEAD* pHead;  // �ش�
	char* pData;  // ������
	TAIL* pTail;  // ����
	
public:
	CPacket();  //������
	~CPacket();  //�Ҹ���
	int Pack(short code, char* data=NULL, int datalen=0);  //��Ŷ ����
	int Unpack(char* buf, int buflen);  //��Ŷ ����
	int IsValid();  //��ȿ�� �˻�
	char* GetData() {return pData;}
	int GetCode() {return pHead->code;}
	char* GetPacket() {return pPacket;}
	int GetLength() {return pTail->cb;}
};

//������
CPacket::CPacket() 
	:pPacket(NULL), pHead(NULL), pData(NULL), pTail(NULL) {
}

//�Ҹ���
CPacket::~CPacket() {
	if(pPacket!=NULL)
		delete pPacket;
}

//��Ŷ ����
int CPacket::Pack(short code, char* data, int datalen) {
	try {
		pPacket=new char[sizeof(HEAD)+datalen+sizeof(TAIL)];
		memset(pPacket,0,sizeof(HEAD)+datalen+sizeof(TAIL));

		//Header ����
		pHead=(PHEAD)pPacket;
		strcpy_s(pHead->key,BEGIN);
		pHead->code=code;
		pHead->cb=datalen;
		//Data ����
		if((datalen>0)&&(data!=NULL)) {
			pData=(char*)(pPacket+sizeof(HEAD));
			memcpy(pData,data,datalen);
		} else if((data==NULL)&&(datalen==0)) {
		} else if((data==NULL)||(datalen<=0)) {
			throw("������ ���� ����");
		}
		//Tail ����
		pTail=(PTAIL)(pPacket+sizeof(HEAD)+datalen);
		strcpy_s(pTail->key,END);
		pTail->cb=sizeof(HEAD)+datalen+sizeof(TAIL);
	
		//��ȿ�� ����
		IsValid();

	} catch (.../*char* str*/) {
		delete pPacket;
		pPacket=NULL; pHead=NULL; pTail=NULL; pData=NULL; 
		return 103;
	}

	return 0;
}

//��Ŷ ����
int CPacket::Unpack(char* buf, int buflen) {
	try {
		pPacket=new char[buflen];
		memcpy(pPacket, buf, buflen);

		//Header ����
		pHead=(PHEAD)pPacket;
		//Data ���� 
		if(pHead->cb>0)
			pData=(char*)(pPacket+sizeof(HEAD));
		//Tail ����
		pTail=(PTAIL)(pPacket+sizeof(HEAD)+pHead->cb);

		//��ȿ�� ����
		IsValid();

	} catch(...) {
		delete pPacket;
		pPacket=NULL; pHead=NULL; pTail=NULL; pData=NULL; 
		return 103;
	} 

	return 0;
}

//��ȿ�� �˻�
int CPacket::IsValid() {
	//�ش��� ������ �����ڿ� ��ġ���� �ʴ� ���
	if(strcmp(BEGIN,pHead->key)&&strcmp(END,pTail->key))
		throw;
	
	//��Ŷ�� ũ��� ���۹��� ����Ÿ�� ũ�Ⱑ �ٸ� ��� 
	int tlen=pHead->cb+sizeof(HEAD)+sizeof(TAIL);
	if(tlen!=pTail->cb)
		throw;
	
	//��Ŷ�� �ּ� �ִ� ũ�⸦ �̴� �Ǵ� �ʰ��� ���
	if((tlen<sizeof(HEAD)+sizeof(TAIL))||(tlen>MAX_PACK_SIZE))
		throw;

	return 0;
}