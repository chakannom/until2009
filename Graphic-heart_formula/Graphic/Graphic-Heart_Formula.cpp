// Graphic.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Graphic-Heart_Formula.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawAxes(HDC hdc, POINT point); //좌표축 그리기
void DrawRectangle(HDC hdc); // 타원 그리기
void DrawCardioid(HDC hdc); // 심장형 곡선 그리기
void DrawHeart1(HDC hdc); // 하트1 그리기
void DrawHeart2(HDC hdc); // 하트2 그리기
void DrawHeart3(HDC hdc); // 하트3 그리기

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GRAPHIC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHIC));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHIC));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GRAPHIC);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 616, 438, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		DrawRectangle(hdc);
		DrawCardioid(hdc);
		DrawHeart1(hdc);
		DrawHeart2(hdc);
		DrawHeart3(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawAxes(HDC hdc, POINT point) {
	MoveToEx(hdc, point.x, point.y-100, NULL);
	LineTo(hdc, point.x, point.y+100);
	MoveToEx(hdc, point.x-100, point.y, NULL);
	LineTo(hdc, point.x+100, point.y);
}

void DrawRectangle(HDC hdc) {
	POINT point = {100, 100};
	double PI = 3.141592;

	DrawAxes(hdc, point);

	for(int i=0;i<360;i++) {
		MoveToEx(hdc, point.x+80*cos(i*PI/180), point.y+80*sin(i*PI/180), NULL);
		LineTo(hdc, point.x+80*cos((i+1)*PI/180), point.y+80*sin((i+1)*PI/180));
	}
}

void DrawCardioid(HDC hdc) {
	POINT point = {300, 100};
	POINT revision = {0, -50};
	double PI = 3.141592;

	DrawAxes(hdc, point);

	for(double i=0;i<360;i=i+0.1) {
		int r= 65+65*sin(i*PI/180);
		MoveToEx(hdc, point.x+r*cos(i*PI/180)+revision.x, point.y+r*sin(i*PI/180)+revision.y, NULL);
		LineTo(hdc, point.x+r*cos((i+0.1)*PI/180)+revision.x, point.y+r*sin((i+0.1)*PI/180)+revision.y);
	}
}

void DrawHeart1(HDC hdc) {
	double r=0, t=0; //극 좌표
	double x=0, y=0; //평면 좌표
	double e=0; //에러수 체크

	POINT point = {500, 100};
	DrawAxes(hdc, point);

	//원주율 * 2
	while(t<6.28) {
		if(t>3.14) r = sqrt(30.0/(17.0+8*sin(2.0*t)));
		else r = sqrt(30.0/(17.0-8*sin(2.0*t)));

		//평면 좌표계를 극좌표계로 치환한다.
		//x=r*cos(0); y=r*sin(0);
		x = r*sin(t);
		y = -r*cos(t);

		//에러체크
		//e = 17.0*x-16.0*fabs(x)*y+17.0*y*y-255;

		//해당 평면좌표에 픽셀을 찍는다.
		SetPixel(hdc, (x*50)+point.x,(y*50)+point.y,RGB(0,0,0));

		//정밀도(숫자가 작아질수록 점이 정밀해준다.)
		t += 0.0001;
	}
}

void DrawHeart2(HDC hdc) {

}

void DrawHeart3(HDC hdc) {
	double t=0;
	double x=0, y=0; //평면 좌표

	POINT point = {300, 300};
	DrawAxes(hdc, point);

	//원주율 * 2
	while(t<6.28) {
		//평면 좌표계
		x = 16*sin(-t)*sin(t)*sin(t);
		y = -(13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t));

		//해당 평면좌표에 픽셀을 찍는다.
		SetPixel(hdc, (x*5)+point.x,(y*5)+point.y,RGB(0,0,0));

		//정밀도(숫자가 작아질수록 점이 정밀해준다.)
		t += 0.0001;
	}
}

