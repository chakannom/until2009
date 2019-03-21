#include "pcsslock.h"

//��Ʈ�� �������� ���� (IA-32 manual vol3, ch 2.5 CR0 (Control Register Zero) ���������� WP ��Ʈ(16)�� ���� �Ӽ���� ����
#define CR0_WP_MASK 0x0FFFEFFFF 
//ntoskrnl �� export �� �Լ��� �ּҸ� �����ϴ� ��ũ��
#define SYSCALL_INDEX(_Function) *(PULONG)((PUCHAR)_Function+1)
#define ORG_SYSCALL_PTR(_orgFunc) &(((PLONG)KeServiceDescriptorTable.ServiceTableBase)[SYSCALL_INDEX(_orgFunc)])

//direct import KeServiceDescriptorTable
__declspec(dllimport) SERVICE_DESCRIPTOR_ENTRY KeServiceDescriptorTable;

//direct import native api
extern NTSYSAPI NTSTATUS NTAPI ZwOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId OPTIONAL);

//type define for native api
typedef NTSTATUS (*ZWOPENPROCESS)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId OPTIONAL);

//variable to save original api pointer
ZWOPENPROCESS oldZwOpenProcess = NULL;  //save

//our hook procedure 
NTSTATUS newZwOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId OPTIONAL);

VOID ClearWriteProtect(VOID)
{
	__asm
	{
		push  eax;
		mov   eax, cr0;
		and   eax, CR0_WP_MASK;  //WP Ŭ����
		mov   cr0, eax;
		pop   eax;
	}
}

VOID SetWriteProtect(VOID)
{
	__asm
	{
		push  eax;
		mov   eax, cr0;
		or    eax, not CR0_WP_MASK;  //WP ��Ʈ ����
		mov   cr0, eax;
		pop   eax;
	}
}

NTSTATUS newZwOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId OPTIONAL)
{
	NTSTATUS RetStatus = oldZwOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
	if((NULL != ClientId) && (NULL != g_pDevExt->hProcessID)) {
		CLIENT_ID cid = *ClientId;
		if(g_pDevExt->hProcessID == cid.UniqueProcess) {
			DbgPrint("[*] pid : %d protected\n", cid.UniqueProcess);
			RetStatus = STATUS_INVALID_CID;  // An invalid Client ID was specified.							
		}
	}
	return RetStatus;
}

VOID Hook()
{
	ClearWriteProtect();

	//save old function pointer
	oldZwOpenProcess = (ZWOPENPROCESS)ORG_SYSCALL_PTR(ZwOpenProcess);

	//change my function
	oldZwOpenProcess = (ZWOPENPROCESS)InterlockedExchange((LONG *)ORG_SYSCALL_PTR(ZwOpenProcess), (LONG)newZwOpenProcess);
	
	SetWriteProtect();
}

VOID UnHook()
{
	ClearWriteProtect();

	InterlockedExchange((LONG*)ORG_SYSCALL_PTR(ZwOpenProcess), (LONG)oldZwOpenProcess);

	SetWriteProtect();
}