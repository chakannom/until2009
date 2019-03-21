#include "ddkhook.h"

#ifdef __cplusplus
extern "C" {
#endif

//direct import KeServiceDescriptorTable
__declspec(dllimport) SERVICE_DESCRIPTOR_ENTRY KeServiceDescriptorTable;

//direct import native api
extern NTSYSAPI NTSTATUS NTAPI ZwOpenProcess(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PCLIENT_ID ClientId OPTIONAL);

//type define for native api
typedef NTSTATUS (*ZWOPENPROCESS)(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PCLIENT_ID ClientId OPTIONAL);

#ifdef __cplusplus
}
#endif

//ntoskrnl 이 export 한 함수의 주소를 리턴하는 매크로
#define SYSCALL_INDEX(_Function) *(PULONG)((PUCHAR)_Function+1)
#define ORG_SYSCALL_PTR(_orgFunc) &(((PLONG)KeServiceDescriptorTable.ServiceTableBase)[SYSCALL_INDEX(_orgFunc)])

//응용프로그램에서 전달된 Process ID
HANDLE ProcessID;

//variable to save original api pointer
ZWOPENPROCESS oldZwOpenProcess=NULL;  //save

//our hook procedure 
NTSTATUS newZwOpenProcess(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PCLIENT_ID ClientId OPTIONAL);
 
//콘트롤 레지스터 관련 (IA-32 manual vol3, ch 2.5 CR0 (Control Register Zero) 레지스터의 WP 비트(16)는 쓰기 속성제어에 사용됨
#define CR0_WP_MASK 0x0FFFEFFFF 

VOID ClearWriteProtect(VOID) {
  __asm
  {
    push  eax;
    mov   eax, cr0;
    and   eax, CR0_WP_MASK;  //WP 클리어
    mov   cr0, eax;
    pop   eax;
  }
}

VOID SetWriteProtect(VOID) {
  __asm
  {
    push  eax;
    mov   eax, cr0;
    or    eax, not CR0_WP_MASK;  //WP 비트 세팅
    mov   cr0, eax;
    pop   eax;
  }
}

NTSTATUS newZwOpenProcess(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes,PCLIENT_ID ClientId OPTIONAL) {
	NTSTATUS status=oldZwOpenProcess(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);
	if((NULL!=ProcessID)&&(NULL!=ClientId)&&(NULL!=deviceExtension)) {
		CLIENT_ID cid=*ClientId;
		if(ProcessID==cid.UniqueProcess) {
			KdPrint(("[*] pid : %d protected\n",cid.UniqueProcess));
			status=STATUS_INVALID_CID;  // An invalid Client ID was specified.							
		}
	}
	return status;
}

void hook(void) {
	ClearWriteProtect();

	//save old function pointer
	oldZwOpenProcess=(ZWOPENPROCESS)ORG_SYSCALL_PTR(ZwOpenProcess);

	//change my function
	oldZwOpenProcess=(ZWOPENPROCESS)InterlockedExchange((LONG *)ORG_SYSCALL_PTR(ZwOpenProcess),(LONG)newZwOpenProcess);
	
	SetWriteProtect();
}

void unhook(void) {
	ClearWriteProtect();
	InterlockedExchange((LONG*)ORG_SYSCALL_PTR(ZwOpenProcess),(LONG)oldZwOpenProcess);
	SetWriteProtect();
}