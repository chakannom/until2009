#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

char block[7][5][5]={{{0,0,0,0,0},{0,1,0,0,0},{0,1,0,0,0},{0,1,0,0,0},{0,1,0,0,0}},
		    {{0,0,0,0,0},{0,0,1,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,0,0,0}},
		    {{0,0,0,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,1,1,0,0},{0,0,0,0,0}},
		    {{0,0,0,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,0,0,0}},
		    {{0,0,0,0,0},{0,0,1,0,0},{0,1,1,0,0},{0,1,0,0,0},{0,0,0,0,0}},
		    {{0,0,0,0,0},{0,1,0,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,0,0,0}},
		    {{0,0,0,0,0},{0,1,1,0,0},{0,1,1,0,0},{0,0,0,0,0},{0,0,0,0,0}}};
char box[12][16];   //box[width+2][height+1];
char c_block[5][5];   //���� ����
int c_block_n;   //���� ���� ��ȣ
int x_b,y_b;   //���� ��ǥ
int Time=0;   //�����������½ð�
int score;   //����
int stage[12]={200,160,120,100,80,70,60,50,40,30,20,10};  //���� �������� �ӵ�����
int num;

void in_box()   //box�� ���� �Է�
{
	int x,y;
	for(y=1;y<15;y++)
	{
		box[0][y]=1;
		box[11][y]=1;
	}
	for(x=0;x<12;x++)
	{
		box[x][15]=1;
	}
	for(x=1;x<11;x++)
	{
		for(y=0;y<15;y++)
		{
			box[x][y]=0;
		}
	}
	score=0;
}
void out_box()   //��Ʈ���� ���ȭ��
{
	int x,y;
	for(y=0;y<16;y++)
	{
		gotoxy(20,y+1);
		for(x=0;x<12;x++)
		{
			if(c_block_n!=8 && x>=x_b && x<x_b+5 && y>=y_b && y<y_b+5)
			{
				printf("%c",(c_block[x-x_b][y-y_b]==1)?1:(box[x][y]==1)?'H':' ');
			}
			else
			{
				printf("%c",(box[x][y]==1) ?'H':' ');
			}
		}
		printf("\n");
	}
	gotoxy(17,17);
	printf("KEY : ����Ű & SPASE BAR & ESC");
	gotoxy(34,6);
	printf("NEXT BLOCK");
	for(y=0;y<5;y++)
	{
		gotoxy(35,y+7);
		for(x=0;x<5;x++)
		{
			printf("%c",block[num][x][y]);
		}
		printf("\n");
	}
	gotoxy(34,12);
	printf("STAGE : %d",score/50+1);
	gotoxy(34,13);
	printf("SCORE : %d",score);
}
void ro()   //����ȸ��
{
	int x,y;
	int tmp_block[5][5];
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			tmp_block[4-y][x]=c_block[x][y];
		}
	}
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			c_block[x][y]=tmp_block[x][y];
		}
	}
}
int clash()   //�浹�� �ߴ��� ���ߴ��� üũ��
{
	int x,y;
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			if(box[x_b+x][y_b+y]==1 && c_block[x][y]==1)
			{
			return 0;
			}
		}
	}
	return 1;
}
void down()   //������ ������..
{
	int x,y;
	y_b++;
	if(!clash())
	{
		y_b--;
		for(x=0;x<5;x++)
		{
			for(y=0;y<5;y++)
			{
				if(c_block[x][y]==1)
				{
					box[x_b+x][y_b+y]=1;
				}
			}
		}
		c_block_n=8;
	}
}
void downn()   //������ �������� ��
{
	while(c_block_n!=8)
	{
		down();
	}
}
void left()   //������ ��������..
{
	x_b--;
	if(!clash())
	{
		x_b++;
	}
}
void right()   //������ ����������..
{
	x_b++;
	if(!clash())
	{
		x_b--;
	}
}
void key()   //key����
{
	int a=0;
	a=getch();
	switch(a)
	{
		case 72: //UP
			ro();
			break;
		case 75: //LEFT
			left();
			break;
		case 77: //RIGHT
			right();
			break;
		case 80: //DOWN
			down();
			break;
		case 32: //DOWN.DOWN.DOWN(SPACE BAR)
			downn();
			Time=0;
			break;
		case 27: //EXIT
			exit(1);
	}
}
void del_line()   //������ �� ���� �������� �κ�&�������
{
	int x,y;
	for(y=14;y>0;y--)
	{
		if(box[1][y]==1 && box[2][y]==1 && box[3][y]==1 && box[4][y]==1 && box[5][y]==1 && box[6][y]==1 && box[7][y]==1 && box[8][y]==1 && box[9][y]==1 && box[10][y]==1)
		{
			for(x=y;x>0;x--)
			{
				box[1][x]=box[1][x-1];
				box[2][x]=box[2][x-1];
				box[3][x]=box[3][x-1];
				box[4][x]=box[4][x-1];
				box[5][x]=box[5][x-1];
				box[6][x]=box[6][x-1];
				box[7][x]=box[7][x-1];
				box[8][x]=box[8][x-1];
				box[9][x]=box[9][x-1];
				box[10][x]=box[10][x-1];
			}
			score=score+10;
		}
	}
}
void main()
{
	int a;
	int x,y;
	in_box();
	c_block_n=8;
	randomize();
	num=random(7);
	while(1)
	{
		del_line();
		if(c_block_n==8)
		{
			c_block_n=num;
			num=random(7);
			for(x=0;x<5;x++)
			{
				for(y=0;y<5;y++)
				{
					c_block[x][y]=block[c_block_n][x][y];
				}
			}
			x_b=6/2;
			y_b=0;
			if(!clash())
			{
				gotoxy(27,10);
				printf("GAME OVER");
				gotoxy(23,11);
				printf("press ESC to exit");
				gotoxy(22,12);
				printf("press 1 to new game");
				while(1)
				{
					a=getch();
					if(a==49)
					{
						system("cls");
						gotoxy(25,10);
						printf("NEW GAME LOADING");
						gotoxy(23,12);
						printf("press KEY to new game");
						getch();
						system("cls");
						in_box();
						break;
					}
					else if(a==27)
					{
						exit(1);
					}
					else
					{
						continue;
					}
				}
			}
		}
		Time++;
		out_box();
		if(Time>=stage[score/50])
		{
			down();
			Time=0;
			continue;
		}
		if(kbhit())
		{
		key();
		}
	}
} ???�?   ��??�b??��???�x??????�?   ???�??��???�?   ???�?   ???�??????�?   ??f�?   ???�???�???�????��??�?   ???�?   ��?