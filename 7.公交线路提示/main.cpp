#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<bits/stdc++.h>
#define MAX_VERTEX_NUM 5200
#define INFINITY 0x40000000 - 0x1000
using namespace std;
typedef struct node
{
	int num; //有几个直达线路
	int len; //直达最短距离
	int line[10]; //记录直达公交线路
	node() :num(0), len(INFINITY) {}

}node;

typedef struct ArcCell {
	node adj;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
	string vexs[MAX_VERTEX_NUM];       //站点
	AdjMatrix arcs;
	int vexnum, arcnum;
}MGraph;

typedef struct
{
	string station[75];
	int StatNum;

}bus;

bus Bus[991]; //990条线路


int LocateVex(MGraph& G, string str)
{
	for (int i = 0; i < G.vexnum; ++i)
	{
		if (G.vexs[i] == str)
			return i;
	}
	return -1;
}

bool isNumber(char& c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

void CreateUDN(MGraph& G)
{
	fstream infile;
	infile.open("data.txt", ios::in);
	if (!infile)
	{
		cout << "文件打开失败" << endl;
		exit(0);
	}
	while (!infile.eof())
	{
		string line; //线路
		string station; //站点
		infile >> line >> station;
		if (infile.fail())
			break;

		int l = 0;
		int count = 0; //一条线上的站点数
		for (int i = 0; i < line.size(); ++i)
		{
			if (!isNumber(line[i]))
			{
				break;
			}
			else
			{
				l = l * 10 + (line[i] - '0');
			}
		}
		for (int i = 0; i < station.size(); ++i)
		{
			string temp;
			while (station[i] != ',' && i < station.size())
			{
				temp += station[i];
				++i;
			}
			Bus[l].station[count] = temp;
			count++;
			if (LocateVex(G, temp) == -1)
			{
				G.vexs[G.vexnum] = temp;
				G.vexnum++;

			}


		}
		Bus[l].StatNum = count;
		for (int i = 0; i < count; ++i)
		{
			for (int j = i; j < count; ++j)
			{
				int x = LocateVex(G, Bus[l].station[i]);
				int y = LocateVex(G, Bus[l].station[j]);
				int dis = j - i + 1; //两个站点之间的直达距离
				if (G.arcs[x][y].adj.num == 0) //还没有直达线路 新添加直达线路 距离，线路号，数量
				{
					G.arcs[x][y].adj.len = dis;
					G.arcs[x][y].adj.line[G.arcs[x][y].adj.num] = l;
					G.arcs[x][y].adj.num++;
					G.arcs[y][x] = G.arcs[x][y];
					G.arcnum++;
				}
				else
				{
					if (dis < G.arcs[x][y].adj.len) //距离更短 更新最短直达距离 之前的记录清空
					{
						G.arcs[x][y].adj.len = dis;
						memset(G.arcs[x][y].adj.line, 0, 10 * sizeof(int));
						G.arcs[x][y].adj.line[0] = l;
						G.arcnum -= G.arcs[x][y].adj.num; //删去之前记录的边数
						G.arcs[x][y].adj.num = 1;
						G.arcs[y][x] = G.arcs[x][y];
						G.arcnum++;
					}
					else if (dis == G.arcs[x][y].adj.len && G.arcs[x][y].adj.num < 10) //距离相同 添加线路
					{
						G.arcs[x][y].adj.line[G.arcs[x][y].adj.num] = l;
						G.arcs[x][y].adj.num++;
						G.arcs[y][x] = G.arcs[x][y];
						G.arcnum++;
					}
				}

			}
		}


	}
	infile.close();
	return;
}

int Transfer[MAX_VERTEX_NUM];  //记录最少转车次数
int used[MAX_VERTEX_NUM];
int pre[MAX_VERTEX_NUM]; // 用于输出路径

int SearchStatInLine(string station, int line)
{
	for (int i = 0; i < Bus[line].StatNum; ++i)
	{
		if (Bus[line].station[i] == station)
			return i;
	}
	return -1;
}

void DijsktraMiniTrans(MGraph& G, string start, string end) //转车次数最少
{
	int s = LocateVex(G, start);
	int e = LocateVex(G, end);
	if (s == -1 || e == -1)
	{
		cout << "找不到站点" << endl;
		return;
	}
	// memset(Transfer, -1, G.vexnum * sizeof(int));
	// memset(pre, -1, G.vexnum * sizeof(int));
	// memset(used, 0, G.vexnum * sizeof(int));
	for (int i = 0; i < G.vexnum; ++i)
	{
		pre[i] = Transfer[i] = INFINITY;
		used[i] = 0;
	}
	Transfer[s] = 0;
	while (1)
	{
		int min = -1;
		for (int i = 0; i < G.vexnum; ++i) //找转车次数最少的点
		{
			if (!used[i] && (min == -1 || Transfer[min] > Transfer[i]))
			{
				min = i;
			}
		}
		if (min == -1 || min == e)
			break;
		used[min] = 1; //收入已访问过顶点集
		for (int i = 0; i < G.vexnum; ++i)
		{
			if (G.arcs[min][i].adj.num != 0 && !used[i]) //更新路径
			{
				if (Transfer[min] + 1 < Transfer[i])
				{
					Transfer[i] = Transfer[min] + 1;
					pre[i] = min;
				}
			}
		}

	}
	int road[MAX_VERTEX_NUM]; //乘车路径
	memset(road, 0, MAX_VERTEX_NUM * sizeof(int));
	int temp_e = e;
	int temp_s = s;
	int ct = 0;
	while (temp_e != temp_s)
	{
		road[ct] = temp_e;
		temp_e = pre[temp_e];
		ct++;
	}
	int StatCount = 0;
	road[ct] = temp_s;
	cout << "------------------转车次数:" << ct << "------------------" << endl;
	int num = 0;
	for (int i = ct; i > 0; --i)
	{
		int cur, next;
		cur = road[i];
		next = road[i - 1];
		int line = G.arcs[cur][next].adj.line[0];
		int x = SearchStatInLine(G.vexs[cur], line);
		int y = SearchStatInLine(G.vexs[next], line);
		cout << "从 " << Bus[line].station[x] << "乘 " << line << " 号线" << "到" << Bus[line].station[y];
		cout << ",共" << abs(y - x) + 1 << "站";
		StatCount += abs(y - x) + 1;
		cout << endl;

	}
	cout << "总共" << StatCount << "站" << endl;
	return;
}

int MinDis[MAX_VERTEX_NUM];

void DijsktraMiniLen(MGraph& G, string start, string end) //经过站点最少
{
	int s = LocateVex(G, start);
	int e = LocateVex(G, end);
	if (s == -1 || e == -1)
	{
		cout << "找不到站点" << endl;
		return;
	}
	// memset(MinDis, INFINITY, G.vexnum * sizeof(int));
	// memset(pre, INFINITY, G.vexnum * sizeof(int));
	// memset(used, 0, G.vexnum * sizeof(int));
	for (int i = 0; i < G.vexnum; ++i)
	{
		pre[i] = MinDis[i] = INFINITY;
		used[i] = 0;
	}
	MinDis[s] = 0; //记录最短距离
	while (1)
	{
		int min = -1;
		for (int i = 0; i < G.vexnum; ++i)
		{
			if (!used[i] && (min == -1 || MinDis[i] < MinDis[min]))
			{
				min = i;
			}
		}
		if (min == -1 || min == e)
			break;
		used[min] = 1; //收入已访问过顶点集
		for (int i = 0; i < G.vexnum; ++i)
		{
			if (G.arcs[min][i].adj.num > 0 && !used[i])
			{
				if (MinDis[min] + G.arcs[min][i].adj.len < MinDis[i]) //更新最短路
				{
					MinDis[i] = MinDis[min] + G.arcs[min][i].adj.len;
					pre[i] = min;
				}
			}
		}

	}

	int road[MAX_VERTEX_NUM];
	memset(road, 0, sizeof(int) * MAX_VERTEX_NUM);
	int StatCount = 0;
	int temp_e = e;
	int temp_s = s;
	int ct = 0;
	while (temp_e != temp_s)
	{
		road[ct] = temp_e;
		temp_e = pre[temp_e];
		ct++;
	}
	road[ct] = temp_s;
	for (int i = ct; i > 0; --i)
	{
		int cur, next;
		cur = road[i];
		next = road[i - 1];
		int line = G.arcs[cur][next].adj.line[0];
		int x = SearchStatInLine(G.vexs[cur], line);
		int y = SearchStatInLine(G.vexs[next], line);
		cout << "----------------------------------------------------------" << endl;
		cout << "从" << Bus[line].station[x] << "乘" << line << "号线到" << Bus[line].station[y];
		cout << " ,共" << abs(y - x)+ 1 << "站" << endl;
		StatCount += abs(y - x)+ 1;

	}
	cout << "全程共" << StatCount << "站," << "转车" << ct << "次" << endl;
	return;
}

MGraph G;

int main(void)
{
	G.arcnum = G.vexnum = 0;
	cout << "正在读取数据..." << endl;
	CreateUDN(G);
	string start, end;
	int cmd;
	while (1)
	{
		system("cls");
		cout << "1.查询转车次数最少的路线" << endl;
		cout << "2.查询经过站点最少的路线" << endl;
		cout << "3.退出" << endl;
		cin >> cmd;
		if (cmd == 1)
		{
			cout << "输入开始站点:";
			cin >> start;
			cout << "输入结束站点:";
			cin >> end;
			DijsktraMiniTrans(G, start, end);
			system("pause");
		}
		else if (cmd == 2)
		{
			cout << "输入开始站点:";
			cin >> start;
			cout << "输入结束站点:";
			cin >> end;
			DijsktraMiniLen(G, start, end);
			system("pause");
		}
		else if (cmd == 3)
		{
			break;
		}
		else
		{
			cout << "输入有误,请重新输入";
			system("pause");
		}
	}
	return 0;
}