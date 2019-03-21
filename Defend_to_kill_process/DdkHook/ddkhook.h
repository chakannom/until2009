#ifndef _DDKHOOK_H_INCLUDE_
#define _DDKHOOK_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(disable:4200)
#pragma warning(disable:4201)
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <initguid.h>
#include <wdm.h>
#include <ntddk.h>
#include "devioctl.h"

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#ifdef __cplusplus
}
#endif 

#define DRIVERNAME "ddkhook"
#define DRIVER_FILE_NAME "ddkhook.sys"
#define AKDRV_SVC_NAME DRIVERNAME  //���� �̸�
#define NT_DEVICE_NAME L"\\Device\\ddkhook"  //����̽� �̸�
#define DOS_DEVICE_NAME L"\\DosDevices\\ddkhook"  //����̽��� �ɺ��� ��ũ �̸�

// DEVICE_EXTENSION ����ü ����
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT DeviceObject;  //device object this extension belongs to
}DEVICE_EXTENSION,*PDEVICE_EXTENSION;

extern PDEVICE_EXTENSION deviceExtension;	

//Service descriptor table entry -- ntolkrnl.exe
#pragma pack(1)
typedef struct _SERVICE_DESCRIPTOR_ENTRY {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; 
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
}SERVICE_DESCRIPTOR_ENTRY,*PSERVICE_DESCRIPTOR_ENTRY;
#pragma pack()

#endif