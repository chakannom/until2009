#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "conadlib.h" //gotoxy함수를 쓰기 위해서

#define ESC 27
#define LEFT 75	
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SNAKE_SIZE 50 //스네이크 크기
#define SNAKE_SPEED 5 //스네이크 속도
#define SAVE_SIZE 50 //저정공간크기

char Snake[SNAKE_SIZE];//스네이크
char Map[23][23];//맵
int x[SAVE_SIZE]={0},y[SAVE_SIZE]={0};//좌표 저장하는 배열
int key=3;//방향을 저장
int x_j=1,y_j=1;//좌표
int len=3;//스네이크 길이
char fname[13]={"map[0].txt"};//맵 변화를 위한 배열
int food_num;//먹이 갯수

void IN_MAP() //맵 입력
{

	FILE *fi;
	int x,y;
	fi = fopen(fname, "r");
	for(x=0;x<23;x++)
	{
		for(y=0;y<23;y++)
		{
			fscanf(fi, "%d", &Map[x][y]);
		}
	}
	fclose(fi);
}


void OUT_MAP() //맵 출력
{
	int x,y;
	for(x=0;x<23;x++)
	{
		gotoxy(15,x+3);
		for(y=0;y<23;y++)
		{
			if(Map[x][y]==0)
			{
				printf("  ");
			}
			else if(Map[x][y]==1)
			{
				printf("▣");
			}
			else if(Map[x][y]==2)
			{
				printf("■");
			}
			else if(Map[x][y]==3)
			{
				printf("★");
			}
			else if(Map[x][y]==4)
			{
				printf("●");
			}
		}
		printf("\n");
	}
}

void FOOD() //먹이의 갯수를 세는 부분
{
	int x,y;
	for(x=0;x<23;x++)
	{
		for(y=0;y<23;y++)
		{
			if(Map[x][y]==3)
			{
				food_num++;
			}
		}
	}
}

void SNAKE_DIRECT() //스네이크 방향조절
{
	if(key==1)
	{
		x_j=x_j-1;
	}
	else if(key==2)
	{
		y_j=y_j-1;
	}
	else if(key==3)
	{
		y_j=y_j+1;
	}
	else if(key==4)
	{
		x_j=x_j+1;
	}
}

int CLASH() //충돌을 체크해주는 부분
{
	int i;
	for(i=0;i<23;i++)
	{
		if(Map[x[0]][y[0]]==1) //벽에 부딪혔을 경우
		{
			return 0;
		}
	}
	for(i=3;i<len+1;i++)
	{
		if(x[0]==x[i]&&y[0]==y[i]) //몸에 부딪혔을 경우
		{
			return 0;
		}
	}
	return 1;
}

void GAME_INITIALIZE() // 게임 준비를 위한 초기화 부분
{
	int i;
	for(i=0;i<SAVE_SIZE;i++)
	{
		x[i]=0;
		y[i]=0;
	}
	x_j=y_j=1;
	key=3;
	len=3;
	for(i=1;i<SNAKE_SIZE;i++)//스네이크 길이 제어
	{
		Snake[i]=0;
	}
}

void GAME_START() //초기화면
{
	gotoxy(0,2);
	printf("□□■■■□□□■■□□□■■□□□■■■□□□■■□□□■■□■■■■■■■\n");
	printf("□■■■■■□□■■□□□■■□□■■■■■□□■■□□□■■□■■■■■■■\n");
	printf("■■■□■■■□■■□□■■■□□■■□■■□□■■□□■■■□■■□□□□□\n");
	printf("■■□□□□□□■■□□■■■□□■■□■■□□■■□■■■□□■■□□□□□\n");
	printf("■■□□□□□□■■□■■■■□■■■□■■■□■■■■■□□□■■□□□□□\n");
	printf("■■■■□□□□■■□■□■■□■■□□□■■□■■■■□□□□■■■■■■■\n");
	printf("□■■■■■□□■■□■□■■□■■□□□■■□■■■□□□□□■■■■■■■\n");
	printf("□□□□■■■□■■□■□■■□■■■■■■■□■■■■□□□□■■□□□□□\n");
	printf("□□□□□■■□■■■■□■■□■■■■■■■□■■■■■□□□■■□□□□□\n");
	printf("□□□□□■■□■■■□□■■□■■□□□■■□■■□■■■□□■■□□□□□\n");
	printf("■■■□■■■□■■■□□■■□■■□□□■■□■■□□■■■□■■□□□□□\n");
	printf("□■■■■■□□■■□□□■■□■■□□□■■□■■□□□■■□■■■■■■■\n");
	printf("□□■■■□□□■■□□□■■□■■□□□■■□■■□□□■■□■■■■■■■\n");
	gotoxy(28,16);
	printf("NEW GAME LOADING");
	gotoxy(25,18);
	printf("press KEY to GAME START");
	getch();
	system("cls");
}

void main()
{
	int Time=0;
	int temp_1[SAVE_SIZE]={0},temp_2[SAVE_SIZE]={0};
	int a;
	int i;
	int stage_int=0;
	char stage_char[2];
	
	GAME_START();//초기화면
	IN_MAP();//맵 입력
	FOOD();//먹이의 갯수 세는 부분
	Snake[0]=4;//스네이크 머리
	while(1)
	{
		
		OUT_MAP();//맵 출력
		if(len==3+food_num) // stage를 깼을 경우
		{
			stage_int+=1; //단계를 올려줌
			if(stage_int==5) //stage를 모두 깼을 경우
			{
				gotoxy(30,15);
				printf("Congratulation");
				while(1)
				{
					if(getch()==ESC)
					{
						exit(1);
					}
					else
					{
						continue;
					}
				}
			}
			while(1) //한단계가 올라 갔을 경우 실행
			{
				gotoxy(35,10);
				printf("YOU WIN\n");
				gotoxy(29,13);
				printf("NEXT STAGE LOADING..");
				gotoxy(27,15);
				printf("press KEY to START GAME");
				if(getch())
				{
					food_num=0;
					itoa(stage_int,stage_char,10);
					fname[4]=stage_char[0];
					GAME_INITIALIZE();//초기화
					IN_MAP();
					FOOD();
					break;
				}
				else
				{
					continue;
				}
			}
		}

		x[0]=x_j;
		y[0]=y_j;

		for(i=1;i<len;i++)//스네이크 길이 제어
		{
			Snake[i]=2;
		}
		if(Map[x[0]][y[0]]==3)//먹이 먹을 때 마다 스네이크 길이 길어지는 부분
		{
			len++;
		}
		if(!CLASH())//충돌체크
		{
			gotoxy(33,10);
			printf("GAME OVER");
			gotoxy(29,11);
			printf("press ESC to exit");
			gotoxy(28,12);
			printf("press 1 to new game");
			while(1)
			{
				a=getch();
				if(a==49)
				{
						system("cls");
						gotoxy(29,10);
						printf("NEW GAME LOADING");
						gotoxy(27,12);
						printf("press KEY to new game");
						getch();
						system("cls");
						IN_MAP();
						GAME_INITIALIZE();//초기화
						break;
				}
				else if(a==ESC)
				{
						exit(1);
				}
				else
				{
					continue;
				}
			}
		}
		for(i=0;i<len+1;i++)// 스네이크를 화면에 출력
		{
			Map[x[i]][y[i]]=Snake[i];
		}
		Time=Time+1;
		if(Time==SNAKE_SPEED)//스네이크 스피드 제어
		{
			Time=0;//시간 초기화
			for(i=0;i<SAVE_SIZE;i++) //좌표바꿈을 위한 임지 저장부분
			{
				temp_1[i]=x[i];
				temp_2[i]=y[i];
			}
			for(i=0;i<SAVE_SIZE-1;i++) //좌표 바꾸는 부분
			{
				x[i+1]=temp_1[i];
				y[i+1]=temp_2[i];
			}
			SNAKE_DIRECT(); //스네이크 방향조절
		}
		if(kbhit()) //키 체크
		{
			switch(getch())
			{
				case UP: //UP
					key=1;
					break;
				case LEFT: //LEFT
					key=2;
					break;
				case RIGHT: //RIGHT
					key=3;
					break;
				case DOWN: //DOWN
					key=4;
					break;
				case ESC: //EXIT
					exit(1);
			}
		}
	}
}