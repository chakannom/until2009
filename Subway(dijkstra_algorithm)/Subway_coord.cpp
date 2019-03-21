#include "Subway_coord.h"

#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

extern vector <SubwayCoord> Coord;

// 좌표값을 받는다.
bool Input_Data_Coord()
{

	ifstream in;

	in.open("data_coord.txt");

	if(!in.is_open())
	{
		MessageBox(NULL,"파일열기 실패","확인!", MB_OK);
		return false;
	}

	SubwayCoord temp;

	while(in>>temp.name)
	{
		if(strcmp(temp.name,"0")==0)
			break;

		
		in>>temp.x;
		in>>temp.y;
		in>>temp.Sign;
		temp.check = false;

		Coord.push_back(temp);
	}

	in.close();

	return true;
}

// Sign 저장 원칙 
/*
0 오른쪽 아래
1 왼쪽 아래
2 오른쪽
3 왼쪽
4 아래쪽

*/