#include<iostream>
#include<stdlib.h> 
#include<fstream>
#define STACK_INIT_SIZE 500
#define STACKINCREMENT 50
using namespace std;
typedef struct
{
	int x;
	int y;
}Point;

typedef struct {
	Point* base;
	Point* top;
	int stacksize;
} SqStack;

void InitStack(SqStack& s)
{
	s.base = (Point*)malloc(STACK_INIT_SIZE * sizeof(Point));
	s.top = s.base;
	s.stacksize = STACK_INIT_SIZE;
}
void DestroyStack(SqStack& s)
{
	free(s.base);
	s.base = NULL;
	s.top = NULL;
}

bool StackEmpty(SqStack s)
{
	if (s.top == s.base)
		return true;
	return false;
}
int StackLength(SqStack s)
{
	return s.top - s.base;
}
bool GetTop(SqStack s, Point& e)
{
	if (s.top == s.base)
		return false;
	e = *(s.top - 1);
	return true;
}
void Push(SqStack& s, Point e)
{
	if (s.top - s.base == s.stacksize)
	{
		s.base = (Point*)realloc(s.base, (s.stacksize + STACKINCREMENT) * sizeof(Point));
		s.top = s.base + s.stacksize;
		s.stacksize += STACKINCREMENT;
	}
	*s.top = e;
	s.top++;
	return;
}

bool Pop(SqStack& s, Point& e)
{
	if (s.top == s.base)
		return false;
	--s.top;
	e = *s.top;
	return true;
}
int Maze[101][101]; //迷宫 0为道路 1为墙
char ans[101][101];
int row, col; //行数 列数

void InitMaze()
{
	ifstream infile;
	infile.open("data.txt");
	if (!infile)
	{
		cout << "打开文件失败" << endl;
		exit(0);

	}
	infile >> row >> col;
	for(int i = 0 ;i<row;++i)
	{
		for (int j = 0; j < col; ++j)
		{
			infile >> Maze[i][j];
			ans[i][j] = Maze[i][j] + 48;
		}
	}
	infile.close();
	return;
}

bool Available(Point& P)
{
	if (P.x < 0 || P.y < 0 || P.x >= row || P.y >= col || Maze[P.x][P.y] != 0)
		return false;
	return true;
}

bool DFS(SqStack& S, Point& start, Point& end)
{
	Push(S, start); 
	while (!StackEmpty(S))
	{
		Point temp;
		int x, y;
		GetTop(S, temp);
		x = temp.x;
		y = temp.y;
		if (x == end.x && y == end.y)
		{
			return true;
		}
		Maze[x][y] = 2;
		Point next;
		next.x = x;
		next.y = y + 1;
		if (Available(next))
		{
			Push(S, next);
			continue;
		}
		next.x = x + 1;
		next.y = y;
		if (Available(next))
		{
			Push(S, next);
			continue;
		}
		next.x = x;
		next.y = y - 1;
		if (Available(next))
		{
			Push(S, next);
			continue;
		}
		next.x = x - 1;
		next.y = y;
		if (Available(next))
		{
			Push(S, next);
			continue;
		}
		Pop(S, next);
	}
	return false;

}

void PrintRoad(SqStack& S)
{
	while (!StackEmpty(S))
	{
		Point temp;
		Pop(S, temp);
		ans[temp.x][temp.y] = '#';
	}
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << ans[i][j] << " ";
		}
		cout << endl;
	}
}

int main(void) 
{
	SqStack S;
	InitStack(S);
	InitMaze();
	cout << "----------------迷宫为:----------------" << endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << Maze[i][j]<<" ";
		}
		cout << endl;
	}
	cout << "--------------------------------------" << endl;

	Point start;
	Point end;
	while (1)
	{	
		cout << "输入起始点x y(从0开始):";
		cin >> start.x >> start.y;
		if (!Available(start))
		{
			cout << "该点不可用,请重新输入." << endl;
			continue;
		}
		break;
	}
	while (1)
	{
		cout << "输入结束点x y:";
		cin >> end.x >> end.y;
		if (!Available(end))
		{
			cout << "该点不可用,请重新输入." << endl;
			continue;
		}
		break;
	}
	if (DFS(S, start, end))
	{
		cout << "迷宫路线如下:" << endl;
		PrintRoad(S);
	}

	return 0;
}



