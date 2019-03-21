#ifndef PCSSLOCK_H_INCLUDE
#define PCSSLOCK_H_INCLUDE

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

#define MAXLINE 256
#define DEVICE_NAME L"\\Device\\PcssLock"  //Device Name
#define DOS_DEVICE_NAME L"\\DosDevices\\dosPcssLock"  //Device's symbolic Link Name

#define IOCTL_PCSSLOCK_PCSSINFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0800, METHOD_BUFFERED, FILE_WRITE_ACCESS)

// DEVICE_EXTENSION 구조체 정의
typedef struct _DEVICE_EXTENSION
{
	PDEVICE_OBJECT pDevObj;
	HANDLE hProcessID;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//Service descriptor table entry -- ntolkrnl.exe
#pragma pack(1)
typedef struct _SERVICE_DESCRIPTOR_ENTRY
{
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; 
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} SERVICE_DESCRIPTOR_ENTRY, *PSERVICE_DESCRIPTOR_ENTRY;
#pragma pack()

extern VOID Hook();
extern VOID UnHook();

extern PDEVICE_EXTENSION g_pDevExt;

#endif  //PCSSLOCK_H_INCLUDE