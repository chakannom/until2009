// Graphic.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Graphic-Heart_Formula.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawAxes(HDC hdc, POINT point); //��ǥ�� �׸���
void DrawRectangle(HDC hdc); // Ÿ�� �׸���
void DrawCardioid(HDC hdc); // ������ � �׸���
void DrawHeart1(HDC hdc); // ��Ʈ1 �׸���
void DrawHeart2(HDC hdc); // ��Ʈ2 �׸���
void DrawHeart3(HDC hdc); // ��Ʈ3 �׸���

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GRAPHIC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHIC));

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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
	double r=0, t=0; //�� ��ǥ
	double x=0, y=0; //��� ��ǥ
	double e=0; //������ üũ

	POINT point = {500, 100};
	DrawAxes(hdc, point);

	//������ * 2
	while(t<6.28) {
		if(t>3.14) r = sqrt(30.0/(17.0+8*sin(2.0*t)));
		else r = sqrt(30.0/(17.0-8*sin(2.0*t)));

		//��� ��ǥ�踦 ����ǥ��� ġȯ�Ѵ�.
		//x=r*cos(0); y=r*sin(0);
		x = r*sin(t);
		y = -r*cos(t);

		//����üũ
		//e = 17.0*x-16.0*fabs(x)*y+17.0*y*y-255;

		//�ش� �����ǥ�� �ȼ��� ��´�.
		SetPixel(hdc, (x*50)+point.x,(y*50)+point.y,RGB(0,0,0));

		//���е�(���ڰ� �۾������� ���� �������ش�.)
		t += 0.0001;
	}
}

void DrawHeart2(HDC hdc) {

}

void DrawHeart3(HDC hdc) {
	double t=0;
	double x=0, y=0; //��� ��ǥ

	POINT point = {300, 300};
	DrawAxes(hdc, point);

	//������ * 2
	while(t<6.28) {
		//��� ��ǥ��
		x = 16*sin(-t)*sin(t)*sin(t);
		y = -(13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t));

		//�ش� �����ǥ�� �ȼ��� ��´�.
		SetPixel(hdc, (x*5)+point.x,(y*5)+point.y,RGB(0,0,0));

		//���е�(���ڰ� �۾������� ���� �������ش�.)
		t += 0.0001;
	}
}

