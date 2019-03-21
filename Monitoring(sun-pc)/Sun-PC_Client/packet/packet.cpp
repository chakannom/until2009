#undef UNICODE
#undef _UNICODE

#define DLLTYPE
#define  WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0400 

#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) { 
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

#include "packet.h"

#ifdef __cplusplus  // C++ �����Ϸ��� �������̸� extern "C" Ű���带 ����Ѵ�.
	extern "C" {
#endif 
	// DLL�� �ͽ���Ʈ�ϴ� �Լ��� �����Ѵ�.
	__declspec(dllexport) void* GetPacketObject()
	{
		return new CPacket;
	}
	// �ͽ���Ʈ �Լ��� �� �߰��Ϸ��� �� �κп��� �����ϸ� �ȴ�.
#ifdef __cplusplus
}
#endif