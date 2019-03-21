#include "pcssmonitor.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	NTSTATUS RetStatus = STATUS_SUCCESS;
	UNICODE_STRING DevName, DosDevName;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;

	DbgPrint("DriverEntry >> Enter\n");

	//Point 'DevName' at the driver name(DEVICE_NAME)
	RtlInitUnicodeString(&DevName, DEVICE_NAME);

	//Create and Initialize device object
	RetStatus = IoCreateDevice(pDriverObject, sizeof(DEVICE_EXTENSION), &DevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
	if(FALSE == NT_SUCCESS(RetStatus)) {
		DbgPrint("Fail IoCreateDevice\n");
		return RetStatus;
	}

	//Assign extension variable
	pDevExt = pDevObj->DeviceExtension;

	//Use IO Buffer
	pDevObj->Flags |= DO_BUFFERED_IO;

	//Point 'DosDevName' at the device name(DOS_DEVICE_NAME)
	RtlInitUnicodeString(&DosDevName, DOS_DEVICE_NAME);

	//Create symbolic link to the user-visible name
	RetStatus= IoCreateSymbolicLink(&DosDevName, &DevName);
	if(FALSE == NT_SUCCESS(RetStatus)) {
		DbgPrint("Fail IoCreateSymbolicLink\n");
		IoDeleteDevice(pDriverObject->DeviceObject);
		return RetStatus;
	}

	//Assign global pointer to the device object for use by the callback functions
	g_pDevObj = pDevObj;

	//Initialize hProcessHandle, pProcessEvent
	pDevExt->hProcessHandle = NULL;
	pDevExt->pProcessEvent = NULL;

	//Load structure to point to IRP handlers
	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = OnClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnControl;

	RetStatus = PsSetCreateProcessNotifyRoutine(ProcessMonCallback, FALSE);
	if(FALSE == NT_SUCCESS(RetStatus)) {
		DbgPrint("Fail PsSetCreateProcessNotifyRoutine\n");
		IoDeleteSymbolicLink(&DosDevName);
		IoDeleteDevice(pDriverObject->DeviceObject);
		return RetStatus;
	}

	DbgPrint("DriverEntry >> Leave\n");
	
	return RetStatus;
}

NTSTATUS OnCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	DbgPrint("OnCreate >> Enter(CreateFile)\n");

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("OnCreate >> Leave(CreateFile)\n");

	return STATUS_SUCCESS;
}

NTSTATUS OnClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	DbgPrint("OnClose >> Enter(CloseFile)\n");

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("OnClose >> Leave(CloseFile)\n");

	return STATUS_SUCCESS;
}

NTSTATUS OnControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	NTSTATUS RetStatus = STATUS_SUCCESS;
	PDEVICE_EXTENSION pDevExt = pDeviceObject->DeviceExtension;
	PIO_STACK_LOCATION pIOStack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG OutBufSize = pIOStack->Parameters.DeviceIoControl.OutputBufferLength;
	PPCSSMON_CALLBACK_INFO pPcssMonInfo;

	DbgPrint("OnControl >> Enter(DeviceIoControl)\n");

	switch(pIOStack->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_PCSSMON_SHARDEVENT:
		{
			DbgPrint("IOCTL_PCSSMON_SHARDEVENT\n");

			pDevExt->hProcessHandle = *(HANDLE*)pIrp->AssociatedIrp.SystemBuffer;

			RetStatus = ObReferenceObjectByHandle(pDevExt->hProcessHandle, EVENT_ALL_ACCESS, *ExEventObjectType, UserMode, &pDevExt->pProcessEvent, NULL);
			if(FALSE == NT_SUCCESS(RetStatus)) {
				DbgPrint("Fail IOCTL_PCSSMON_SHARDEVENT\n");
			}

			break;
		}
		case IOCTL_PCSSMON_PROCINFO:
		{
			if(OutBufSize == sizeof(PCSSMON_CALLBACK_INFO)) {
				pPcssMonInfo = pIrp->AssociatedIrp.SystemBuffer;

				pPcssMonInfo->hParentID = pDevExt->hParentID;
				pPcssMonInfo->hProcessID = pDevExt->hProcessID;
				pPcssMonInfo->bCreate = pDevExt->bCreate;

				RetStatus = STATUS_SUCCESS;
			}
			break;
		}
		default:
		{
			break;
		}
	}

	pIrp->IoStatus.Status = RetStatus;
	pIrp->IoStatus.Information = OutBufSize;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("OnControl >> Leave(DeviceIoControl)\n");

	return RetStatus;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING DosDevName;

	DbgPrint("DriverUnload >> Enter\n");

	PsSetCreateProcessNotifyRoutine(ProcessMonCallback, TRUE);

	RtlInitUnicodeString(&DosDevName, DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&DosDevName);

	IoDeleteDevice(pDriverObject->DeviceObject);

	DbgPrint("DriverUnload >> Leave\n");
}

VOID ProcessMonCallback(HANDLE hParentID, HANDLE hProcessID, BOOLEAN bCreate)
{
	PDEVICE_EXTENSION pDevExt = g_pDevObj->DeviceExtension;

	DbgPrint("ProcessMonCallback >> Enter\n");

	if(bCreate) {
		DbgPrint("Process Start ( ParentID: %d , ProcessID : %d )\n", hParentID, hProcessID);
	} else {
		DbgPrint("Process End ( ParentID: %d , ProcessID : %d )\n", hParentID, hProcessID);
	}

	pDevExt->hParentID = hParentID;
	pDevExt->hProcessID = hProcessID;
	pDevExt->bCreate = bCreate;

	if(pDevExt->pProcessEvent != NULL) {
		KeSetEvent(pDevExt->pProcessEvent, 0, FALSE);
//		KeClearEvent(pDevExt->pProcessEvent);
	}

	DbgPrint("ProcessMonCallback >> Leave\n");
}