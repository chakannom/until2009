#include "ddkhook.h"
#include "filefunc.h"

NTSTATUS __stdcall DdkHookDummy(PDEVICE_OBJECT DeviceObject,PIRP Irp);
NTSTATUS __stdcall DdkHook_CreateFile(PDEVICE_OBJECT DeviceObject,PIRP Irp);
NTSTATUS __stdcall DdkHook_WriteFile(PDEVICE_OBJECT DeviceObject,PIRP Irp);
NTSTATUS __stdcall DdkHook_CloseHandle(PDEVICE_OBJECT DeviceObject,PIRP Irp);
VOID __stdcall DdkHookUnload(PDRIVER_OBJECT DriverObject);
void IrpStringOut(PIRP Irp);

extern HANDLE ProcessID;
extern void hook(void);
extern void unhook(void);

PDEVICE_EXTENSION deviceExtension=NULL;

//DriverEntry 루틴(I/O 매니저에 의해서 드라이버가 설치될때 자동으로 호출되는 함수)
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath) {
	NTSTATUS returnStatus=STATUS_SUCCESS;
	PDEVICE_OBJECT deviceObject;
	UNICODE_STRING ntName;
	UNICODE_STRING win32Name;
	ProcessID=NULL;

	//STEP 1(device 이름 생성)
	RtlInitUnicodeString(&ntName,NT_DEVICE_NAME);
	returnStatus=IoCreateDevice(DriverObject,sizeof(DEVICE_EXTENSION),&ntName,FILE_DEVICE_UNKNOWN,FILE_DEVICE_SECURE_OPEN,TRUE,&deviceObject);
	if(FALSE==NT_SUCCESS(returnStatus)) {
		KdPrint(("%s >> IoCreateDevice fail\n",__FUNCTION__));
		return returnStatus;
	}

	// STEP 2(device extension 초기화)
	deviceExtension=(PDEVICE_EXTENSION)deviceObject->DeviceExtension;
	RtlZeroMemory(deviceExtension,sizeof(DEVICE_EXTENSION));
	deviceExtension->DeviceObject=deviceObject;


	// STEP 3(symbolic link 생성)
	RtlInitUnicodeString(&win32Name,DOS_DEVICE_NAME);	
	returnStatus=IoCreateSymbolicLink(&win32Name,&ntName);
	if(FALSE==NT_SUCCESS(returnStatus)) {
		KdPrint(("%s >> IoCreateSymbolicLink() failed\n",__FUNCTION__));
		IoDeleteDevice(DriverObject ->DeviceObject);
		return returnStatus;
	}

	//IRP의 SystemBuffer를 사용하겠다는 의미
	deviceObject->Flags|=DO_BUFFERED_IO;

	// STEP 4(initialize function pointers)
	for(int i=0;i<=IRP_MJ_MAXIMUM_FUNCTION;i++) {
		DriverObject->MajorFunction[i]=DdkHookDummy;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE]=DdkHook_CreateFile;
	DriverObject->MajorFunction[IRP_MJ_WRITE]=DdkHook_WriteFile;
	DriverObject->MajorFunction[IRP_MJ_CLOSE]=DdkHook_CloseHandle;
	DriverObject->DriverUnload=DdkHookUnload;
	
	// hook 설치 
	hook();

	return returnStatus;
}

//dummy dispatch handler
NTSTATUS __stdcall DdkHookDummy(PDEVICE_OBJECT DeviceObject,PIRP Irp) {
	NTSTATUS returnStatus=STATUS_SUCCESS;
	PIO_STACK_LOCATION pStack;

	KdPrint(("%s >> enter\n",__FUNCTION__));

	IrpStringOut(Irp);

	pStack=IoGetCurrentIrpStackLocation(Irp);	
	UCHAR MajorFunction=pStack->MajorFunction;

	Irp->IoStatus.Status=returnStatus;
	Irp->IoStatus.Information=0;
	
	IoCompleteRequest(Irp,IO_NO_INCREMENT);
	
	KdPrint(("%s >> leave\n",__FUNCTION__));

	return returnStatus;
}

// Application가 보내는 CreateFile()함수에 대응되는 처리기
NTSTATUS __stdcall DdkHook_CreateFile(PDEVICE_OBJECT DeviceObject,PIRP Irp) {
	NTSTATUS returnStatus=STATUS_SUCCESS;
	PIO_STACK_LOCATION pStack;

	KdPrint(("%s >> enter\n",__FUNCTION__));

	IrpStringOut(Irp);

	pStack=IoGetCurrentIrpStackLocation(Irp);

	if(!AllocateStructForFileContext(pStack->FileObject)) returnStatus = STATUS_INVALID_PARAMETER;
	
	KdPrint(("%s >> leave\n",__FUNCTION__));

	Irp->IoStatus.Status=returnStatus;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return returnStatus;
}

// Application가 보내는 WriteFile()함수에 대응되는 처리기
NTSTATUS __stdcall DdkHook_WriteFile(PDEVICE_OBJECT DeviceObject,PIRP Irp) {
	NTSTATUS returnStatus=STATUS_SUCCESS;
	PIO_STACK_LOCATION pStack;
	ULONG dwRequestSize;
	char * pClientBuffer;

	KdPrint(("%s >> enter\n",__FUNCTION__));

	IrpStringOut(Irp);

	pStack=IoGetCurrentIrpStackLocation(Irp);

	dwRequestSize=pStack->Parameters.Write.Length;

	pClientBuffer=(char*)Irp->AssociatedIrp.SystemBuffer; // DO_BUFFERED_IO

	ProcessID=(HANDLE)atoi(pClientBuffer);

	switch(GetTypeCreate(pStack->FileObject)) {
		case STRINGTYPE:
		{
			PCONTEXT_FOR_STRING pContextForString;
			pContextForString=(PCONTEXT_FOR_STRING)pStack->FileObject->FsContext;
			dwRequestSize=((int)dwRequestSize>MAXSTRINGBUFFERCOUNT)?0:dwRequestSize;

			if(!dwRequestSize) {
				returnStatus=STATUS_INVALID_PARAMETER;
			}
			else {
				memcpy(pContextForString->StringBuffer,pClientBuffer,dwRequestSize);
				pContextForString->CurrentRemainBufferSize=dwRequestSize;
			}
			break;
		}
		default:
			break;
	}
	Irp->IoStatus.Information=dwRequestSize; // 실제수행된 크기를 적는다..
	
	KdPrint(("%s >> leave\n",__FUNCTION__));

	Irp->IoStatus.Status=returnStatus;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return returnStatus;
} 

// Application가 보내는 CloseHandle()함수에 대응되는 처리기
NTSTATUS __stdcall DdkHook_CloseHandle(PDEVICE_OBJECT DeviceObject,PIRP Irp) {
	NTSTATUS returnStatus=STATUS_SUCCESS;
	PIO_STACK_LOCATION pStack;

	KdPrint(("%s >> enter\n",__FUNCTION__));

	IrpStringOut(Irp);

	pStack=IoGetCurrentIrpStackLocation(Irp);

	FreeStructForFileContext(pStack->FileObject); // 사용된 FsContext를 제거합니다
	
	KdPrint(("%s >> leave\n",__FUNCTION__));

	Irp->IoStatus.Status=returnStatus;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return returnStatus;
}

//driver unload dispatch handler
VOID __stdcall DdkHookUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING Win32NameString;

	KdPrint(("%s >> enter\n",__FUNCTION__));	

	// 설치된 훅을 제거
	unhook();

	// symbolic link 제거 및 디바이스 객체 삭제
	RtlInitUnicodeString(&Win32NameString,DOS_DEVICE_NAME);	
	IoDeleteSymbolicLink(&Win32NameString);	
	IoDeleteDevice(DriverObject->DeviceObject);

	KdPrint(("%s >> leave\n",__FUNCTION__));
}

// IRP를 디버그프린트함
void IrpStringOut(PIRP Irp) {
	KIRQL Irql;
	PIO_STACK_LOCATION pStack;
	
	pStack=IoGetCurrentIrpStackLocation(Irp);

	switch(pStack->MajorFunction) {
		case IRP_MJ_CREATE:
			KdPrint(("[IRP_MJ_CREATE]\n"));
			break;
		case IRP_MJ_CREATE_NAMED_PIPE:
			KdPrint(("[IRP_MJ_CREATE_NAMED_PIPE]\n"));
			break;
		case IRP_MJ_CLOSE:
			KdPrint(("[IRP_MJ_CLOSE]\n"));
			break;
		case IRP_MJ_READ:
			KdPrint(("[IRP_MJ_READ]\n"));
			break;
		case IRP_MJ_WRITE:
			KdPrint(("[IRP_MJ_WRITE]\n"));
			break;
		case IRP_MJ_QUERY_INFORMATION:
			KdPrint(("[IRP_MJ_QUERY_INFORMATION]\n"));
			break;
		case IRP_MJ_SET_INFORMATION:
			KdPrint(("[IRP_MJ_SET_INFORMATION]\n"));
			break;
		case IRP_MJ_QUERY_EA:
			KdPrint(("[IRP_MJ_QUERY_EA]\n"));
			break;
		case IRP_MJ_SET_EA:
			KdPrint(("[IRP_MJ_SET_EA]\n"));
			break;
		case IRP_MJ_FLUSH_BUFFERS:
			KdPrint(("[IRP_MJ_FLUSH_BUFFERS]\n"));
			break;
		case IRP_MJ_QUERY_VOLUME_INFORMATION:
			KdPrint(("[IRP_MJ_QUERY_VOLUME_INFORMATION]\n"));
			break;
		case IRP_MJ_SET_VOLUME_INFORMATION:
			KdPrint(("[IRP_MJ_SET_VOLUME_INFORMATION]\n"));
			break;
		case IRP_MJ_DIRECTORY_CONTROL:
			KdPrint(("[IRP_MJ_DIRECTORY_CONTROL]\n"));
			break;
		case IRP_MJ_FILE_SYSTEM_CONTROL:
			KdPrint(("[IRP_MJ_FILE_SYSTEM_CONTROL]\n"));
			break;
		case IRP_MJ_DEVICE_CONTROL:
			KdPrint(("[IRP_MJ_DEVICE_CONTROL]\n"));
			KdPrint(("ControlCode = %8X\n",pStack->Parameters.DeviceIoControl.IoControlCode));
			break;
		case IRP_MJ_INTERNAL_DEVICE_CONTROL:
			KdPrint(("[IRP_MJ_INTERNAL_DEVICE_CONTROL]\n"));
			break;
		case IRP_MJ_SHUTDOWN:
			KdPrint(("[IRP_MJ_SHUTDOWN]\n"));
			break;
		case IRP_MJ_LOCK_CONTROL:
			KdPrint(("[IRP_MJ_LOCK_CONTROL]\n"));
			break;
		case IRP_MJ_CLEANUP:
			KdPrint(("[IRP_MJ_CLEANUP]\n"));
			break;
		case IRP_MJ_CREATE_MAILSLOT:
			KdPrint(("[IRP_MJ_CREATE_MAILSLOT]\n"));
			break;
		case IRP_MJ_QUERY_SECURITY:
			KdPrint(("[IRP_MJ_QUERY_SECURITY]\n"));
			break;
		case IRP_MJ_SET_SECURITY:
			KdPrint(("[IRP_MJ_SET_SECURITY]\n"));
			break;
		case IRP_MJ_POWER:
			KdPrint(("[IRP_MJ_POWER]\n"));
			switch(pStack->MinorFunction) {
				case IRP_MN_WAIT_WAKE:
					KdPrint((" - IRP_MN_WAIT_WAKE\n"));
					break;
				case IRP_MN_POWER_SEQUENCE:
					KdPrint((" - IRP_MN_POWER_SEQUENCE\n"));
					break;
				case IRP_MN_SET_POWER:
					KdPrint((" - IRP_MN_SET_POWER\n"));
					break;
				case IRP_MN_QUERY_POWER:
					KdPrint((" - IRP_MN_QUERY_POWER\n"));
					break;
				default:
					KdPrint((" - ANY MN FUNCTION\n"));
					break;
			}
			break;
		case IRP_MJ_SYSTEM_CONTROL:
			KdPrint(("[IRP_MJ_SYSTEM_CONTROL]\n"));
			switch(pStack->MinorFunction) {
				case IRP_MN_QUERY_ALL_DATA:
					KdPrint((" - IRP_MN_QUERY_ALL_DATA\n"));
					break;
				case IRP_MN_QUERY_SINGLE_INSTANCE:
					KdPrint((" - IRP_MN_QUERY_SINGLE_INSTANCE\n"));
					break;
				case IRP_MN_CHANGE_SINGLE_INSTANCE:
					KdPrint((" - IRP_MN_CHANGE_SINGLE_INSTANCE\n"));
					break;
				case IRP_MN_CHANGE_SINGLE_ITEM:
					KdPrint((" - IRP_MN_CHANGE_SINGLE_ITEM\n"));
					break;
				case IRP_MN_ENABLE_EVENTS:
					KdPrint((" - IRP_MN_ENABLE_EVENTS\n"));
					break;
				case IRP_MN_DISABLE_EVENTS:
					KdPrint((" - IRP_MN_DISABLE_EVENTS\n"));
					break;
				case IRP_MN_ENABLE_COLLECTION:
					KdPrint((" - IRP_MN_ENABLE_COLLECTION\n"));
					break;
				case IRP_MN_DISABLE_COLLECTION:
					KdPrint((" - IRP_MN_DISABLE_COLLECTION\n"));
					break;
				case IRP_MN_REGINFO:
					KdPrint((" - IRP_MN_REGINFO\n"));
					break;
				case IRP_MN_EXECUTE_METHOD:
					KdPrint((" - IRP_MN_EXECUTE_METHOD\n"));
					break;
				case IRP_MN_REGINFO_EX:
					KdPrint((" - IRP_MN_REGINFO_EX\n"));
					break;
				default:
					KdPrint((" - ANY MN FUNCTION\n"));
					break;
			}
			break;
		case IRP_MJ_DEVICE_CHANGE:
			KdPrint(("[IRP_MJ_DEVICE_CHANGE]\n"));
			break;
		case IRP_MJ_QUERY_QUOTA:
			KdPrint(("[IRP_MJ_QUERY_QUOTA]\n"));
			break;
		case IRP_MJ_SET_QUOTA:
			KdPrint(("[IRP_MJ_SET_QUOTA]\n"));
			break;
		case IRP_MJ_PNP:
			KdPrint(("[IRP_MJ_PNP]\n"));
			switch(pStack->MinorFunction) {
				case IRP_MN_START_DEVICE:
					KdPrint((" - IRP_MN_START_DEVICE\n"));
					break;
				case IRP_MN_QUERY_REMOVE_DEVICE:
					KdPrint((" - IRP_MN_QUERY_REMOVE_DEVICE\n"));
					break;
				case IRP_MN_REMOVE_DEVICE:
					KdPrint((" - IRP_MN_REMOVE_DEVICE\n"));
					break;
				case IRP_MN_CANCEL_REMOVE_DEVICE:
					KdPrint((" - IRP_MN_CANCEL_REMOVE_DEVICE\n"));
					break;
				case IRP_MN_STOP_DEVICE:
					KdPrint((" - IRP_MN_STOP_DEVICE\n"));
					break;
				case IRP_MN_QUERY_STOP_DEVICE:
					KdPrint((" - IRP_MN_QUERY_STOP_DEVICE\n"));
					break;
				case IRP_MN_CANCEL_STOP_DEVICE:
					KdPrint((" - IRP_MN_CANCEL_STOP_DEVICE\n"));
					break;
				case IRP_MN_QUERY_DEVICE_RELATIONS:
					KdPrint((" - IRP_MN_QUERY_DEVICE_RELATIONS\n"));
					break;
				case IRP_MN_QUERY_INTERFACE:
					KdPrint((" - IRP_MN_QUERY_INTERFACE\n"));
					break;
				case IRP_MN_QUERY_CAPABILITIES:
					KdPrint((" - IRP_MN_QUERY_CAPABILITIES\n"));
					break;
				case IRP_MN_QUERY_RESOURCES:
					KdPrint((" - IRP_MN_QUERY_RESOURCES\n"));
					break;
				case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:
					KdPrint((" - IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n"));
					break;
				case IRP_MN_QUERY_DEVICE_TEXT:
					KdPrint((" - IRP_MN_QUERY_DEVICE_TEXT\n"));
					break;
				case IRP_MN_FILTER_RESOURCE_REQUIREMENTS:
					KdPrint((" - IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n"));
					break;
				case IRP_MN_READ_CONFIG:
					KdPrint((" - IRP_MN_READ_CONFIG\n"));
					break;
				case IRP_MN_WRITE_CONFIG:
					KdPrint((" - IRP_MN_WRITE_CONFIG\n"));
					break;
				case IRP_MN_EJECT:
					KdPrint((" - IRP_MN_EJECT\n"));
					break;
				case IRP_MN_SET_LOCK:
					KdPrint((" - IRP_MN_SET_LOCK\n"));
					break;
				case IRP_MN_QUERY_ID:
					KdPrint((" - IRP_MN_QUERY_ID\n"));
					break;
				case IRP_MN_QUERY_PNP_DEVICE_STATE:
					KdPrint((" - IRP_MN_QUERY_PNP_DEVICE_STATE\n"));
					break;
				case IRP_MN_QUERY_BUS_INFORMATION:
					KdPrint((" - IRP_MN_QUERY_BUS_INFORMATION\n"));
					break;
				case IRP_MN_DEVICE_USAGE_NOTIFICATION:
					KdPrint((" - IRP_MN_DEVICE_USAGE_NOTIFICATION\n"));
					break;
				case IRP_MN_SURPRISE_REMOVAL:
					KdPrint((" - IRP_MN_SURPRISE_REMOVAL\n"));
					break;
				default:
					KdPrint((" - ANY MN FUNCTION\n"));
					break;
			}
			break;
		default:
			KdPrint(("[ANY IRP] = 0x%8X\n",Irp));
			break;
	}
}