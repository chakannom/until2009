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

// 프로토 타입
bool Input_Data_Coord();