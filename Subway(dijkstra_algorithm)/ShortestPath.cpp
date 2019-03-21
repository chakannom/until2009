#include "shortestpath.h"
#include "subway_coord.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>


#define SUBWAY_NAME_MAX 20
#define INFINITY 30000
#define TRANS_TIME 10


using namespace std;

static vector <Station_data> Station;
extern vector <SubwayCoord> Coord;
stack <int> Path_Data;
vector <SubwayCoord> Path_Coord;
vector <int> Path_Time;


// ���� ����
int **matrix;


int Subway_Shortest_Path(char begin[],char end[])
{
	// ���� �ڷ� ���� ����
	int begin_num;
	int end_num;
	int time;

	// ������ ����.
	while(!Path_Data.empty())
	{
		Path_Data.pop();
	}

	matrix_seting();
	
	//begin,end�� ������Ŀ� �´� ���ڷ� �ٲپ��ش�.
	begin_num = name2path(begin);
	end_num = name2path(end);

	if(begin_num == -1 || end_num == -1)
	{
		MessageBox(NULL,"�߸� �Է� �ϼ̽��ϴ�","���",MB_OK);
		return -1;
	}

	time = trans_sort_loc(begin_num, end_num);
	
	// matrix�� �޸𸮿��� �����Ѵ�
	for(int i=0;i<(int)Station.size();i++)
		delete [] matrix[i];
	delete [] matrix;

	return time;
}



bool isStation(char *name)
{
	for (int i=0; i<Coord.size(); i++)
	{
		if(strcmp(Coord[i].name, name)==0)
		{
			return true;
		}
	}
	return false;
}



int trans_sort_loc(int begin_num, int end_num )
{
	// ����Ÿ�� ���� ���� ���
	int TransData[2][10];

	TransGet(TransData[0], begin_num);
	TransGet(TransData[1], end_num);

	int begin; // ó���� ���� 
	int end;   // ���� ����
	int min_time = INFINITY; // ���� ����

	for(int i=0; TransData[0][i]!=-1;i++)
	{
		for(int j=0; TransData[1][j]!=-1;j++)
		{
			int time = shortest_path(TransData[0][i], TransData[1][j]);
			if(time<min_time)
			{
				min_time = time;
				begin = TransData[0][i];
				end = TransData[1][j];
			}
			// ������ ����д�
			while(!Path_Data.empty())
				Path_Data.pop();
			Path_Coord.clear();

		}
	}
	return shortest_path(begin, end);
}



int *TransGet(int *TransData, int num)
{
	int count = 0;

	for(int i=0;i<Station.size();i++)
	{
		if(strcmp(Station[i]._Name, Station[num]._Name)==0)
		{
			TransData[count++]=i;
		}
	}

	TransData[count] = -1;

	return TransData;
}



void matrix_seting()
{
	int i,j;

	int size = Station.size();

	// matrix �������� �Ҵ�
	matrix = new int*[size];

	for(i=0;i<size;i++)
		matrix[i] = new int[size];

	// matrix �ʱ�ȭ
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			matrix[i][j] = INFINITY;
			if(i==j)
				matrix[i][j] = 0;
		}
	}
	
	// ������� ������Ŀ� ���� ���� �ִ´�.
	for(i=0; i<size-1;i++)
	{
		if(Station[i]._mLine==Station[i+1]._mLine &&
			Station[i]._sLine==Station[i+1]._sLine)
		{
			matrix[i][i+1] = Station[i]._NextTime;
			matrix[i+1][i] = matrix[i][i+1];
		}
	}
	// ȯ�¿��� ��� ������Ŀ� ���� ����
	for (i=0; i<size; i++)
	{
		// i�� �������� �ڿ� �����ϴ� �� Ȯ���Ѵ�.
		for(j=i+1;j<size;j++)
		{
			if(strcmp(Station[i]._Name, Station[j]._Name)==0)
			{
				// ȯ�¿��̹Ƿ� 10��
				matrix[i][j]=TRANS_TIME;
				matrix[j][i]=TRANS_TIME;
			}
		}
			
	}

	// ���� ��Ȳ ó��. 2ȣ���� �������·� �Ǿ� �־ �����ؾ��Ѵ�..
	int tem1, tem2;

	tem1 = name2path("�Ҽ�",2,1);
	tem2 = name2path("����",2,1);

	matrix[tem1][tem2] = Station[tem1]._NextTime;
	matrix[tem2][tem1] = Station[tem1]._NextTime;
	

	//// ���߿� �� ����� �߰��� �� -��-;

}


int name2path(char name[] ,int mLine = 0, int sLine = 0)
{
	int i;

	for(i=0;i<(int)Station.size();i++)
	{
		// ������ Ȯ��
		if(strcmp(Station[i]._Name, name)==0)
		{
			//�⺻���̸� ù��°���� �׳� ����Ѵ�.
			if(mLine==0 && sLine==0)
				return i;

			// �ƴϸ� ȣ���� �°� ����Ѵ�.
			if(mLine == Station[i]._mLine)
				if(sLine == Station[i]._sLine)
					return i;
		}
	}

	return -1; // ã�� ����
}


int name2path(char name[])
{
	int i;

	for(i=0;i<(int)Station.size();i++)
	{
		// ������ Ȯ��
		if(strcmp(Station[i]._Name, name)==0)
		{
			return i;
		}
	}

	return -1; // ã�� ����
}



char *path2name(int path)
{
	return Station[path]._Name;
}

Station_data path2station(int path)
{
	return Station[path];
}


int shortest_path(int start, int end)
{
	int i,j,p,min;
	int last_len;
	int size = Station.size();
	int *length = new int[size];          /* ���������� �Ÿ� */
	int *checked = new int[size];                 /* Ȯ�� �÷��� */
	int *index = new int[size];         /* ���� ������ ����Ű�� ������ */

	// �ð� ǥ�� ����
	Path_Time.clear();

	for (i=0;i<size;i++) {
		length[i] = size;
		checked[i] = 0;
	}

	length[start] = 0;
	index[start] = 0;        /* �������� ǥ������ �ʴ´� */

	for (i=0;i<size;i++) {
		min = size;             /* �ִܰŸ��� ������ ã�´� */

		for (j=0;j<size;j++) {
			if (checked[j]==0 && length[j]<min) {
				p = j;
				min = length[j];
			}
		}

		checked[p]=1;            /* �ִܰŸ��� ������ Ȯ���Ѵ� */
		
		// �� ���� ã������ ���� ����
		if(p==end)
			break;


		if (min==size) {
			printf("�׷����� ����Ǿ� ���� �ʴ�.\n");
			return 1;
		}

		/* p�� �����ؼ� k�� �̸��� �Ÿ��� ���ݱ����� �ִ� ��κ��� ������ ���� */
		for (j=0;j<size;j++) {
			if((length[p]+matrix[p][j])<length[j]) {
				length[j]=length[p]+matrix[p][j];
				index[j]=p;
			}
		}
	}


	// ���ÿ� �ִ´�.
	p = end;

	Path_Data.push(p);

	
	// ȯ�¿�  ó��
	vector <int> check;
	bool sign;

	while (index[p]!=0) {
		if(strcmp(path2name(p),path2name(index[p]))!=0) {
			Path_Data.push(index[p]);
		} else {
			check.push_back(p);
		}

		p=index[p];
	}

	// �����κϺ� ����ó��
	if(start==0)
		Path_Data.push(0);
	
	while(!Path_Data.empty())
	{
		char strStation[SUBWAY_NAME_MAX];
		sign = false;

		strcpy(strStation,path2name(Path_Data.top()));
		Path_Time.push_back(length[Path_Data.top()]);

		// ȯ�� ���� ó��
		for(i=0; i<check.size();i++) {
			if(check[i]==Path_Data.top()) {
				sign = true;
				break;
			}
		}
		Path_Data.pop();

		
		// �ڷḦ ���Ϳ� �ִ´�
		for(i=0;i<Coord.size(); i++)
		{
			if(strcmp(Coord[i].name,strStation)==0) {
				if(sign) {
					SubwayCoord temp;
					temp = Coord[i];
					temp.check = true;
					Path_Coord.push_back(temp);
				}
				else
					Path_Coord.push_back(Coord[i]);
			}
		}
		sign = false;

	}

	// ���̸� ����
	last_len = length[end];
	
	delete [] length;
	delete [] checked;
	delete [] index;

	return last_len;
}



void input_data_station()
{
	ifstream in;
	Station_data sta_temp;
	int nVar1;
	int mLine;
	int sLine;
		
	in.open("data.txt",ios_base::in);
	if (!in.is_open()) {
		MessageBox(NULL,"������ �����ϴ�","���",MB_OK);
		exit(1);
	}


	while(in >> mLine)
	{
		// ȣ���� �Է� �޴´�.

		if (mLine == 0)
			break;

		in >> sLine;

		while(in >> sta_temp._Num)
		{

			// ��ö���� ����� ���� �����۾��� �Ѵ�
			// ��ȣ, �̸�, �����ð�, �㿪�ð�, ȣ����ȣ, ��ȣ����ȣ, ������ȣ, �㿪��ȣ, ����Ÿ�¿�
			if(strcmp(sta_temp._Num,"0")==0) {
				break;
			}
			in >> sta_temp._Name;
			in >> sta_temp._PrevTime;
			in >> sta_temp._NextTime;
			in >> sta_temp._mLine;
			in >> sta_temp._sLine;
			in >> sta_temp._PrevNum;
			in >> sta_temp._NextNum;
			in >> nVar1;
			if (nVar1==1)
				sta_temp._TransStation = true;
			else
				sta_temp._TransStation = false;
			// �Է� �Ϸ�

			// ��ö���� ȣ������ ����ִ´�.
			Station.push_back(sta_temp);
		}

	}

	in.close();
}
