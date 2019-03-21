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


// 전역 변수
int **matrix;


int Subway_Shortest_Path(char begin[],char end[])
{
	// 관련 자료 구조 선언
	int begin_num;
	int end_num;
	int time;

	// 스택을 비운다.
	while(!Path_Data.empty())
	{
		Path_Data.pop();
	}

	matrix_seting();
	
	//begin,end를 인접행렬에 맞는 숫자로 바꾸어준다.
	begin_num = name2path(begin);
	end_num = name2path(end);

	if(begin_num == -1 || end_num == -1)
	{
		MessageBox(NULL,"잘못 입력 하셨습니다","경고",MB_OK);
		return -1;
	}

	time = trans_sort_loc(begin_num, end_num);
	
	// matrix를 메모리에서 삭제한다
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
	// 갈아타는 역이 같은 경우
	int TransData[2][10];

	TransGet(TransData[0], begin_num);
	TransGet(TransData[1], end_num);

	int begin; // 처음역 저장 
	int end;   // 끝역 저장
	int min_time = INFINITY; // 길이 저장

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
			// 스택을 비워둔다
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

	// matrix 동적으로 할당
	matrix = new int*[size];

	for(i=0;i<size;i++)
		matrix[i] = new int[size];

	// matrix 초기화
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			matrix[i][j] = INFINITY;
			if(i==j)
				matrix[i][j] = 0;
		}
	}
	
	// 만들어진 인접행렬에 값을 집어 넣는다.
	for(i=0; i<size-1;i++)
	{
		if(Station[i]._mLine==Station[i+1]._mLine &&
			Station[i]._sLine==Station[i+1]._sLine)
		{
			matrix[i][i+1] = Station[i]._NextTime;
			matrix[i+1][i] = matrix[i][i+1];
		}
	}
	// 환승역일 경우 인접행렬에 값을 저장
	for (i=0; i<size; i++)
	{
		// i를 기준으로 뒤에 존제하는 지 확인한다.
		for(j=i+1;j<size;j++)
		{
			if(strcmp(Station[i]._Name, Station[j]._Name)==0)
			{
				// 환승역이므로 10분
				matrix[i][j]=TRANS_TIME;
				matrix[j][i]=TRANS_TIME;
			}
		}
			
	}

	// 예외 상황 처리. 2호선은 루프형태로 되어 있어서 연결해야한다..
	int tem1, tem2;

	tem1 = name2path("뚝섬",2,1);
	tem2 = name2path("성수",2,1);

	matrix[tem1][tem2] = Station[tem1]._NextTime;
	matrix[tem2][tem1] = Station[tem1]._NextTime;
	

	//// 나중에 또 생기면 추가할 것 -ㅁ-;

}


int name2path(char name[] ,int mLine = 0, int sLine = 0)
{
	int i;

	for(i=0;i<(int)Station.size();i++)
	{
		// 같은지 확인
		if(strcmp(Station[i]._Name, name)==0)
		{
			//기본값이면 첫번째것을 그냥 출력한다.
			if(mLine==0 && sLine==0)
				return i;

			// 아니면 호선에 맞게 출력한다.
			if(mLine == Station[i]._mLine)
				if(sLine == Station[i]._sLine)
					return i;
		}
	}

	return -1; // 찾기 실패
}


int name2path(char name[])
{
	int i;

	for(i=0;i<(int)Station.size();i++)
	{
		// 같은지 확인
		if(strcmp(Station[i]._Name, name)==0)
		{
			return i;
		}
	}

	return -1; // 찾기 실패
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
	int *length = new int[size];          /* 정점까지의 거리 */
	int *checked = new int[size];                 /* 확정 플래그 */
	int *index = new int[size];         /* 이전 정점을 가리키는 포인터 */

	// 시간 표기 정리
	Path_Time.clear();

	for (i=0;i<size;i++) {
		length[i] = size;
		checked[i] = 0;
	}

	length[start] = 0;
	index[start] = 0;        /* 시작점은 표시하지 않는다 */

	for (i=0;i<size;i++) {
		min = size;             /* 최단거리인 정점을 찾는다 */

		for (j=0;j<size;j++) {
			if (checked[j]==0 && length[j]<min) {
				p = j;
				min = length[j];
			}
		}

		checked[p]=1;            /* 최단거리인 정점을 확정한다 */
		
		// 끝 점을 찾았으면 루프 종료
		if(p==end)
			break;


		if (min==size) {
			printf("그래프가 연결되어 있지 않다.\n");
			return 1;
		}

		/* p를 경유해서 k에 이르는 거리가 지금까지의 최단 경로보다 작으면 갱신 */
		for (j=0;j<size;j++) {
			if((length[p]+matrix[p][j])<length[j]) {
				length[j]=length[p]+matrix[p][j];
				index[j]=p;
			}
		}
	}


	// 스택에 넣는다.
	p = end;

	Path_Data.push(p);

	
	// 환승역  처리
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

	// 의정부북부 예외처리
	if(start==0)
		Path_Data.push(0);
	
	while(!Path_Data.empty())
	{
		char strStation[SUBWAY_NAME_MAX];
		sign = false;

		strcpy(strStation,path2name(Path_Data.top()));
		Path_Time.push_back(length[Path_Data.top()]);

		// 환승 강조 처리
		for(i=0; i<check.size();i++) {
			if(check[i]==Path_Data.top()) {
				sign = true;
				break;
			}
		}
		Path_Data.pop();

		
		// 자료를 벡터에 넣는다
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

	// 길이를 저장
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
		MessageBox(NULL,"파일이 없습니다","경고",MB_OK);
		exit(1);
	}


	while(in >> mLine)
	{
		// 호선을 입력 받는다.

		if (mLine == 0)
			break;

		in >> sLine;

		while(in >> sta_temp._Num)
		{

			// 전철역을 만들기 위해 선행작업을 한다
			// 번호, 이름, 전역시간, 담역시간, 호선번호, 부호선번호, 전역번호, 담역번호, 갈아타는역
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
			// 입력 완료

			// 전철역을 호선에다 집어넣는다.
			Station.push_back(sta_temp);
		}

	}

	in.close();
}
