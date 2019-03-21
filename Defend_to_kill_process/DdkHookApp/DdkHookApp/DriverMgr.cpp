#undef UNICODE
#undef _UNICODE

#include "DriverMgr.h"

SC_HANDLE scm=NULL;

BOOL DriverInstall(LPCTSTR DriverName,LPCTSTR DriverExe) {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	SC_HANDLE Service=CreateService(scm,DriverName,DriverName,SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,DriverExe,NULL,NULL,NULL,NULL,NULL);
	if (Service==NULL) {
		DWORD err=GetLastError();
		if(err==ERROR_SERVICE_EXISTS) {
			return TRUE;
		}
		else {
			char str[64];
			sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",err);
			MessageBox(NULL,str,"Error",MB_OK);
		}
		return FALSE;
	}
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverRemove(LPCTSTR DriverName,LPCTSTR DriverExe) {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenService(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	BOOL ret=DeleteService(Service);
	if(!ret) {
		char str[64];
		sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",GetLastError());
		MessageBox(NULL,str,"Error",MB_OK);
		return FALSE;
	} 
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverStart(LPCTSTR DriverName) {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenService(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	BOOL ret=StartService(Service,0,NULL);
	if(!ret) { 
		DWORD err=GetLastError();
		if(err==ERROR_SERVICE_ALREADY_RUNNING) {
			ret=TRUE;
		}
		else { 
			char str[64];
			sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",err);
			MessageBox(NULL,str,"Error",MB_OK);
			return FALSE;
		}
	}
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverStop(LPCTSTR DriverName) {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenService(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		DWORD err=GetLastError();
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	SERVICE_STATUS serviceStatus;
	BOOL ret=ControlService(Service,SERVICE_CONTROL_STOP,&serviceStatus);
	if(!ret) {
		char str[64];
		sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",GetLastError());
		MessageBox(NULL,str,"Error",MB_OK);
		return FALSE;
	}

	CloseServiceHandle(Service);
	return TRUE;
}