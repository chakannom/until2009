#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>
#include <windows.h>

#define SUBWAY_NAME_MAX 20
#define INFINITY 30000
#define TRANS_TIME 10

using namespace std;

// 구조체 선언
struct Station_data {
	int _PrevTime;
	int _NextTime;
	char _Name[SUBWAY_NAME_MAX];
	char _Num[10];
	int _mLine;
	int _sLine;
	char _PrevNum[8];
	char _NextNum[8];
	bool _TransStation;
};

// 함수 프로토 타입
int Subway_Shortest_Path(char begin[],char end[]);
void matrix_seting();
int name2path(char [] );
int name2path(char [] , int, int );
char *path2name(int path);
int shortest_path(int start, int end);
void input_data_station();
int *TransGet(int *TransData, int num);
int trans_sort_loc(int begin_num, int end_num );
bool isStation(char *name);