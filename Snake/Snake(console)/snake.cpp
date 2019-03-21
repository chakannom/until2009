#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "conadlib.h" //gotoxy�Լ��� ���� ���ؼ�

#define ESC 27
#define LEFT 75	
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SNAKE_SIZE 50 //������ũ ũ��
#define SNAKE_SPEED 5 //������ũ �ӵ�
#define SAVE_SIZE 50 //��������ũ��

char Snake[SNAKE_SIZE];//������ũ
char Map[23][23];//��
int x[SAVE_SIZE]={0},y[SAVE_SIZE]={0};//��ǥ �����ϴ� �迭
int key=3;//������ ����
int x_j=1,y_j=1;//��ǥ
int len=3;//������ũ ����
char fname[13]={"map[0].txt"};//�� ��ȭ�� ���� �迭
int food_num;//���� ����

void IN_MAP() //�� �Է�
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


void OUT_MAP() //�� ���
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
				printf("��");
			}
			else if(Map[x][y]==2)
			{
				printf("��");
			}
			else if(Map[x][y]==3)
			{
				printf("��");
			}
			else if(Map[x][y]==4)
			{
				printf("��");
			}
		}
		printf("\n");
	}
}

void FOOD() //������ ������ ���� �κ�
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

void SNAKE_DIRECT() //������ũ ��������
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

int CLASH() //�浹�� üũ���ִ� �κ�
{
	int i;
	for(i=0;i<23;i++)
	{
		if(Map[x[0]][y[0]]==1) //���� �ε����� ���
		{
			return 0;
		}
	}
	for(i=3;i<len+1;i++)
	{
		if(x[0]==x[i]&&y[0]==y[i]) //���� �ε����� ���
		{
			return 0;
		}
	}
	return 1;
}

void GAME_INITIALIZE() // ���� �غ� ���� �ʱ�ȭ �κ�
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
	for(i=1;i<SNAKE_SIZE;i++)//������ũ ���� ����
	{
		Snake[i]=0;
	}
}

void GAME_START() //�ʱ�ȭ��
{
	gotoxy(0,2);
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
	printf("����������������������������������������\n");
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
	
	GAME_START();//�ʱ�ȭ��
	IN_MAP();//�� �Է�
	FOOD();//������ ���� ���� �κ�
	Snake[0]=4;//������ũ �Ӹ�
	while(1)
	{
		
		OUT_MAP();//�� ���
		if(len==3+food_num) // stage�� ���� ���
		{
			stage_int+=1; //�ܰ踦 �÷���
			if(stage_int==5) //stage�� ��� ���� ���
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
			while(1) //�Ѵܰ谡 �ö� ���� ��� ����
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
					GAME_INITIALIZE();//�ʱ�ȭ
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

		for(i=1;i<len;i++)//������ũ ���� ����
		{
			Snake[i]=2;
		}
		if(Map[x[0]][y[0]]==3)//���� ���� �� ���� ������ũ ���� ������� �κ�
		{
			len++;
		}
		if(!CLASH())//�浹üũ
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
						GAME_INITIALIZE();//�ʱ�ȭ
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
		for(i=0;i<len+1;i++)// ������ũ�� ȭ�鿡 ���
		{
			Map[x[i]][y[i]]=Snake[i];
		}
		Time=Time+1;
		if(Time==SNAKE_SPEED)//������ũ ���ǵ� ����
		{
			Time=0;//�ð� �ʱ�ȭ
			for(i=0;i<SAVE_SIZE;i++) //��ǥ�ٲ��� ���� ���� ����κ�
			{
				temp_1[i]=x[i];
				temp_2[i]=y[i];
			}
			for(i=0;i<SAVE_SIZE-1;i++) //��ǥ �ٲٴ� �κ�
			{
				x[i+1]=temp_1[i];
				y[i+1]=temp_2[i];
			}
			SNAKE_DIRECT(); //������ũ ��������
		}
		if(kbhit()) //Ű üũ
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