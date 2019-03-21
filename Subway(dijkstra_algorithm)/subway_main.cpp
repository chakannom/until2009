// ---------------- 확 인 ---------------------
// 링커에 Msimg32.lib 파일을 첨부해야 컴파일 됩니다.


#include <windows.h>
#include <vector>
#include <stack>
#include "resource.h"
#include "Subway_coord.h"
#include "shortestpath.h"
#include "searchname.h"


#define MAX 50
#define FONT_SIZE 12
#define DESTENCE 5

using namespace std;


HINSTANCE g_hInst;
HWND hMainWnd;
LPSTR lpszClass = "Subway Main";
// 좌표값저장
vector <SubwayCoord> Coord;
extern vector <SubwayCoord> Path_Coord;
extern vector <int> Path_Time;


// 대화상자에서 넘겨진 값 저장
char Search_Name_Start[SUBWAY_NAME_MAX];
char Search_Name_End[SUBWAY_NAME_MAX];


// 출력 폰트 색
int font_min_R;
int font_min_G;
int font_min_B;


// 함수 프로토 타입
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,WPARAM wParam, LPARAM lParam);
void ShowButton(HDC hdc, char *name);
void ShowButton(HDC hdc);
void View_Subway(HDC hdc);
char *CheckPos(LPARAM lParam);
void InvalidButton(char *name);
void ViewTime(HDC hdc);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdParam, int nCmdShow)
{
	WNDCLASS WndClass;
	HWND hWnd;
	MSG Message;

	g_hInst = hInstance;
	
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = NULL;
	RegisterClass(&WndClass);

	// 입력 실패시 에러
	// 좌표
	if(!Input_Data_Coord())
	{
		return 1;
	}
	// 역 정보
	input_data_station();

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	10,10,1030,704,NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	hMainWnd = hWnd;

	while (GetMessage(&Message, 0,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	// 그림 처리 관련 변수
	HDC hdc;
	PAINTSTRUCT ps;

	// 버튼 처리
	static bool bButton1 = false;
	static bool bButton2 = false;
	static bool bView = false;
	static char *BeginStation;
	static char *EndStation;


	switch(iMessage)
	{
	case WM_CREATE:
		RECT rt;
		SetRect(&rt, 0,0,1030,704);
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, TRUE); 
		MoveWindow(hWnd, 10,10,rt.right - rt.left, rt.bottom - rt.top, FALSE);	

		// 분을 출력할 폰트 색상
		font_min_R = 136;
        font_min_G = 154;
		font_min_B = 20;


		return 0;

	case WM_LBUTTONDOWN:
		if(!bButton1 && !bButton2)
		{
			BeginStation = CheckPos(lParam);

			if(BeginStation!=NULL)
			{
				if(bView)
				{
					Path_Coord.clear();
					bView=false;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				else
				{
					InvalidButton(BeginStation);
				}
				bButton1 = true;
			}
		}	
		else if(bButton1 && !bButton2)
		{
			EndStation = CheckPos(lParam);

			if(EndStation!=NULL)
			{
				if(strcmp(BeginStation,EndStation)==0)
				{
					InvalidButton(BeginStation);
					bButton1 = false;
				}
				else
				{
					InvalidButton(EndStation);
					bButton2 = true;
				
					// 최단경로 구함
					char Mes[200];
					sprintf(Mes,"%s역부터 %s역까지는 %d분 입니다.",BeginStation,EndStation,Subway_Shortest_Path(BeginStation, EndStation));
					MessageBox(hWnd, Mes, "확인",MB_OK);
					

					bView = true;
					
					bButton1=false;
					bButton2=false;
					InvalidateRect(hWnd,NULL,FALSE);
					
				}
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);


		View_Subway(hdc);
		if(bButton1)
			ShowButton(hdc, BeginStation);
		if(bButton2)
			ShowButton(hdc, EndStation);
		if(!Path_Coord.empty())
		{
			ShowButton(hdc);
			ViewTime(hdc);
		}
		EndPaint(hWnd,&ps);

		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case ID_SEARCH_NAME:
			if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SEARCH_NAME), hWnd, SearchNameProc)==IDOK)
			{
				if(Subway_Shortest_Path(Search_Name_Start,Search_Name_End)!=-1)
				{
					bView = true;
					bButton1 = false;
					bButton2 = false;
					InvalidateRect(hWnd,NULL,FALSE);
				}
			}
			break;

		case ID_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;
	}
	
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}




void ShowButton(HDC hdc)
{
				
	HDC MemDC;
	HBITMAP hBitButton1, hBitButton2, hOldBitmap;
	BITMAP bit;
	int bx1, by1;
	int bx2, by2;

	MemDC = CreateCompatibleDC(hdc);

	hBitButton1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BUTTON1));
	hBitButton2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BUTTON2));

	hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitButton1);

	// 버튼의 크기 구하기
	GetObject(hBitButton1, sizeof(BITMAP), &bit);
	bx1 = bit.bmWidth;
	by1 = bit.bmHeight;

	// 버튼의 크기 구하기
	GetObject(hBitButton2, sizeof(BITMAP), &bit);
	bx2 = bit.bmWidth;
	by2 = bit.bmHeight;

	for(int i=0;i<Path_Coord.size();i++)
	{
		if(!Path_Coord[i].check) {
			SelectObject(MemDC, hBitButton1);
			TransparentBlt(hdc, Path_Coord[i].x-5, Path_Coord[i].y-5, bx1, by1, MemDC,0,0,bx1,by1,RGB(255,255,255));
		} else {
			SelectObject(MemDC, hBitButton2);
			TransparentBlt(hdc, Path_Coord[i].x-5, Path_Coord[i].y-5, bx2, by2, MemDC,0,0,bx2,by2,RGB(255,255,255));
		}
	}

	
	SelectObject(MemDC, hOldBitmap);
	DeleteObject(hBitButton1);
	DeleteObject(hBitButton2);
	DeleteDC(MemDC);	
}





void InvalidButton(char *name)
{
	int tx, ty;
	RECT rt;

	for(int i=0;i<Coord.size();i++)
	{
		if(strcmp(name,Coord[i].name)==0)
		{
			 tx = Coord[i].x;
			 ty = Coord[i].y;
		}
	}

	SetRect(&rt, tx-7, ty-7, tx+7, ty+7);

    InvalidateRect(hMainWnd, &rt, FALSE);
}





void ViewTime(HDC hdc)
{
	int i;
	HFONT numFont, oldFont;
	

	numFont = CreateFont(FONT_SIZE,0,0,0,FW_BOLD,0,0,0,HANGEUL_CHARSET,0,0,0,0,"돋음");
	oldFont = (HFONT)SelectObject(hdc, numFont);
	
	SetBkMode(hdc,TRANSPARENT);
	SetTextAlign(hdc, TA_CENTER );

	
	for(i=0;i<Path_Time.size()&&i<Path_Coord.size();i++)
	{
		char Mes[30];
		sprintf(Mes, "%d", Path_Time[i]);

		int bx,by;

		switch(Path_Coord[i].Sign)
		{
		case 0:
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
			bx = Path_Coord[i].x + DESTENCE - 1;
			by = Path_Coord[i].y + DESTENCE - 1;
			break;

		case 1:
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			bx = Path_Coord[i].x - DESTENCE;
			by = Path_Coord[i].y + DESTENCE;
			break;

		case 2:
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
			bx = Path_Coord[i].x + DESTENCE;
			by = Path_Coord[i].y - 5;
			break;

		case 3:
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			bx = Path_Coord[i].x - DESTENCE;
			by = Path_Coord[i].y - 5;
			break;

		default:
			SetTextAlign(hdc, TA_CENTER | TA_TOP);
			bx = Path_Coord[i].x;
			by = Path_Coord[i].y + DESTENCE;	
		}

		SetTextColor(hdc, RGB(255,255,255));

		// 외각선 넣기
		for(int j=-2;j<=2;j++)
			for(int k=-2; k<=2;k++)
				TextOut(hdc, bx + j, by + k,Mes,strlen(Mes));

		SetTextColor(hdc, RGB(font_min_R,font_min_G,font_min_B));
		TextOut(hdc, bx, by ,Mes,strlen(Mes));
	}

	SelectObject(hdc,oldFont);
	DeleteObject(numFont);

}




void ShowButton(HDC hdc, char *name)
{
	HDC MemDC;
	HBITMAP hBitButton, hOldBitmap;
	BITMAP bit;
	int tx, ty;
	int bx, by;

	for(int i=0;i<Coord.size();i++)
	{
		if(strcmp(name,Coord[i].name)==0)
		{
			 tx = Coord[i].x;
			 ty = Coord[i].y;
		}
	}

	MemDC = CreateCompatibleDC(hdc);
	hBitButton = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BUTTON1));
	hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitButton);

	GetObject(hBitButton, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	TransparentBlt(hdc, tx-6, ty-6, bx, by, MemDC,0,0,bx,by,RGB(255,255,255));
	SelectObject(MemDC, hOldBitmap);
	DeleteObject(hBitButton);
	DeleteDC(MemDC);

}





void View_Subway(HDC hdc)
{
	HBITMAP bitSubwayMap,oldMap;
	HDC MemDC;
	BITMAP bit;
	int bx, by;


	MemDC = CreateCompatibleDC(hdc);
	bitSubwayMap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SUBWAYMAIN));
	oldMap = (HBITMAP)SelectObject(MemDC, bitSubwayMap);

	GetObject(bitSubwayMap, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	// 출력
	BitBlt(hdc, 0,0, bx,by, MemDC, 0,0, SRCCOPY);

	SelectObject(MemDC, oldMap);
	DeleteObject(bitSubwayMap);
	DeleteDC(MemDC);
	
}





char *CheckPos(LPARAM lParam)
{
	int i;

	for(i=0; i<Coord.size(); i++)
	{
		if(Coord[i].x+5 > LOWORD(lParam) && Coord[i].x-5 < LOWORD(lParam)
			&& Coord[i].y+5 > HIWORD(lParam) && Coord[i].y-5 < HIWORD(lParam))
		{
			return Coord[i].name;
		}

	}
	return NULL;
}

void init_magnifier()
{
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = g_hInst;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = "ChildClass";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
}