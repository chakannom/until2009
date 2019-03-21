#include "pcsslock.h"

PDEVICE_EXTENSION g_pDevExt = NULL;

NTSTATUS OnCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS OnClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS OnControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);

//DriverEntry 루틴(I/O 매니저에 의해서 드라이버가 설치될때 자동으로 호출되는 함수)
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	NTSTATUS RetStatus = STATUS_SUCCESS;
	UNICODE_STRING DevName, DosDevName;
	PDEVICE_OBJECT pDevObj;

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
	g_pDevExt = pDevObj->DeviceExtension;

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

	//Initialize g_pDevExt
	RtlZeroMemory(g_pDevExt, sizeof(DEVICE_EXTENSION));
	g_pDevExt->pDevObj = pDevObj;

	//Load structure to point to IRP handlers
	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = OnClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnControl;
	
	//Install Hook 
	Hook();

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
	PIO_STACK_LOCATION pIOStack = IoGetCurrentIrpStackLocation(pIrp);

	DbgPrint("OnControl >> Enter(DeviceIoControl)\n");

	switch(pIOStack->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_PCSSLOCK_PCSSINFO:
		{
			DbgPrint("IOCTL_PCSSLOCK_PCSSINFO\n");

			g_pDevExt->hProcessID = *(HANDLE*)pIrp->AssociatedIrp.SystemBuffer;

			DbgPrint("g_pDevExt->hProcessID : %d\n",g_pDevExt->hProcessID);

			break;
		}
		default:
		{
			break;
		}
	}

	pIrp->IoStatus.Status = RetStatus;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("OnControl >> Leave(DeviceIoControl)\n");

	return RetStatus;
}

//driver unload dispatch handler
VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING DosDevName;

	DbgPrint("DriverUnload >> Enter\n");

	//Remove Hook
	UnHook();

	RtlInitUnicodeString(&DosDevName, DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&DosDevName);
	IoDeleteDevice(pDriverObject->DeviceObject);

	DbgPrint("DriverUnload >> Leave\n");
}