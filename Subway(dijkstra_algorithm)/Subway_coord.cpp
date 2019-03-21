#include "Subway_coord.h"

#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

extern vector <SubwayCoord> Coord;

// ��ǥ���� �޴´�.
bool Input_Data_Coord()
{

	ifstream in;

	in.open("data_coord.txt");

	if(!in.is_open())
	{
		MessageBox(NULL,"���Ͽ��� ����","Ȯ��!", MB_OK);
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

// Sign ���� ��Ģ 
/*
0 ������ �Ʒ�
1 ���� �Ʒ�
2 ������
3 ����
4 �Ʒ���

*/