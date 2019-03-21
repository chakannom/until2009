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

#ifdef __cplusplus  // C++ 컴파일러가 실행중이면 extern "C" 키워드를 사용한다.
	extern "C" {
#endif 
	// DLL이 익스포트하는 함수를 정의한다.
	__declspec(dllexport) void* GetPacketObject()
	{
		return new CPacket;
	}
	// 익스포트 함수를 더 추가하려면 이 부분에서 정의하면 된다.
#ifdef __cplusplus
}
#endif