#define    WIN32_LEAN_AND_MEAN
#define    _WIN32_WINNT 0x0400

#include <windows.h>
#include <tchar.h>
#include "../../TaskSwitchCtrlDll/TaskSwitchCtrlDll/TaskSwitchCtrlDll.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
LPCWSTR lpszClass=_T("TaskSwitchCtrl");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,500,120,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage) {
		case WM_KEYDOWN:
			switch(LOWORD(wParam)) {
				case VK_SPACE:
					TaskSwitching_Control(TRUE);
					TaskManager_Control(TRUE);
					break;
				case VK_UP:
					TaskSwitching_Control(FALSE);
					TaskManager_Control(FALSE);
					break;
			}
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd,&ps);
			TextOut(hdc,5,5,_T("WIN key, ALT+TAB, ALT+ESC, CTRL+ESC, CTRL+SHIFT+ESC 막기"),(int)_tcslen(_T("WIN key, ALT+TAB, ALT+ESC, CTRL+ESC, CTRL+SHIFT+ESC 막기")));
			TextOut(hdc,5,25,_T("SPACE BAR KEY는 TRUE!!"),(int)_tcslen(_T("SPACE BAR KEY는 TRUE!!")));
			TextOut(hdc,5,45,_T("UP KEY는 FALSE!!"),(int)_tcslen(_T("UP KEY는 FALSE!!")));
			EndPaint(hWnd,&ps);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}