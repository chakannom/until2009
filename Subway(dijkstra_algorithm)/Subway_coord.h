#pragma once


#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define MAX_SUBWAY 25

typedef struct _SubwayCoord {
	char name[MAX_SUBWAY];
	int x;
	int y;
	int Sign;
	bool check;
} SubwayCoord;

// ������ Ÿ��
bool Input_Data_Coord();