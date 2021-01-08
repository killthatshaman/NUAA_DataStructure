//图的信息 书本p174
//6
//a b c d e f
//a b 6
//a c 1
//a d 5
//b c 5
//c d 5
//b e 3
//c e 6
//c f 4
//d f 2
//e f 6
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<string.h>
#include<iomanip>
#define MAX_VERTEX_NUM 40
#define MAX_ARC_NUM 800
#define INFINITY 0x40000000
using namespace std;
typedef struct ArcCell {
	float adj;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
	char vexs[MAX_VERTEX_NUM];
	AdjMatrix arcs;
	int vexnum, arcnum;
}MGraph;

struct {
	char adjvex;
	float lowcost;
}closedge[MAX_VERTEX_NUM];

typedef struct {
	int from;
	int to;
	float len;
}Arc;

int LocateVex(MGraph G, char c)
{
	for (int i = 0; i < MAX_VERTEX_NUM; ++i)
	{
		if (G.vexs[i] == c)
			return i;
	}
	return -1;
}

void CreateUDN(MGraph& G) //无向网
{
	ifstream infile;
	infile.open("graph.txt");
	if (!infile)
	{
		cout << "打开文件失败" << endl;
		exit(0);
	}
	char start, end;
	float len;
	int arcct = 0;
	infile >> G.vexnum;
	for (int i = 0; i < G.vexnum; i++)
	{
		infile >> G.vexs[i];
	}
	for (int i = 0; i < G.vexnum; i++)
		for (int j = 0; j < G.vexnum; j++)
			G.arcs[i][j].adj = INFINITY;
	while (!infile.eof())
	{
		infile >> start >> end >> len;
		arcct++;
		int i = LocateVex(G, start);
		int j = LocateVex(G, end);
		G.arcs[i][j].adj = G.arcs[j][i].adj = len;
		if (infile.fail())
			break;
	}
	G.arcnum = arcct;
	infile.close();

	return;
}


char GetVex(MGraph& G, int v)
{
	return G.vexs[v];
}

void PutVex(MGraph& G, int v, char value)
{
	G.vexs[v] = value;
	return;
}

int FirstAdjVex(MGraph G, int v)
{
	for (int i = 0; i < G.vexnum; i++)
		if (G.arcs[v][i].adj != INFINITY)
			return i;

	return -1;

}

int NextAdjVex(MGraph G, int v, int w)
{

	for (int i = w + 1; i < G.vexnum; i++)
		if (G.arcs[v][i].adj != INFINITY)
			return i;


	return -1;

}

int minimum(MGraph G)
{
	int k = 0;
	int min = -1;
	while (min == -1)
	{
		if (closedge[k].lowcost != 0)
			min = k;
		k++;
	}
	for (int i = k; i < G.vexnum; ++i)
	{
		if (closedge[i].lowcost < closedge[min].lowcost && closedge[i].lowcost != 0)
			min = i;

	}
	return min;
}

float MiniSpanTree_PRIM(MGraph G, char u)
{
	int k;
	float ans = 0;
	k = LocateVex(G, u);
	for (int j = 0; j < G.vexnum; ++j)
	{
		if (j != k)
		{
			closedge[j].adjvex = u;
			closedge[j].lowcost = G.arcs[k][j].adj;
		}
	}
	closedge[k].lowcost = 0;
	for (int i = 1; i < G.vexnum; ++i)
	{
		k = minimum(G);
		cout << closedge[k].adjvex << " - " << G.vexs[k] << endl;
		ans += closedge[k].lowcost;
		closedge[k].lowcost = 0;
		for (int j = 0; j < G.vexnum; ++j)
		{
			if (G.arcs[k][j].adj < closedge[j].lowcost)
			{
				closedge[j].adjvex = G.vexs[k];
				closedge[j].lowcost = G.arcs[k][j].adj;
			}
		}
	}
	return ans;
}

int father[MAX_VERTEX_NUM]; //并查集

int Father(int x)
{
	if (father[x] != x)
		return Father(father[x]);
	else
		return x;
}

void Union(int x, int y)
{
	father[Father(x)] = Father(y);
}

void SortArcs(MGraph& G, Arc* arcs, int arc_num)
{
	bool used[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	for (int i = 0; i < MAX_VERTEX_NUM; ++i)
	{
		for (int j = 0; j < MAX_VERTEX_NUM; ++j)
		{
			used[i][j] = false;
		}
	}

	int ct = 0;
	for (int i = 0; i < G.vexnum; ++i)
	{
		for (int j = 0; j < G.vexnum; ++j)
		{
			if (G.arcs[i][j].adj != INFINITY && !used[i][j])
			{
				arcs[ct].from = i;
				arcs[ct].to = j;
				arcs[ct].len = G.arcs[i][j].adj;
				used[i][j] = used[j][i] = true;
				ct++;
			}
		}
	}
	for (int i = 0; i < arc_num; ++i)
	{
		int min = i;
		Arc temp;
		for (int j = i + 1; j < arc_num; ++j)
		{
			if (arcs[j].len < arcs[min].len)
				min = j;
		}
		if (min != i)
		{
			temp = arcs[min];
			arcs[min] = arcs[i];
			arcs[i] = temp;
		}
	}
	return;
}

float MiniSpanTree_Kruskal(MGraph G)
{
	int k;

	float ans = 0;

	Arc arcs[MAX_ARC_NUM];
	SortArcs(G, arcs ,G.arcnum); //排序边

	for (int i = 0; i < G.vexnum; ++i)
	{
		father[i] = i; //初始化并查集
	}
	int count = 0;
	for (int i = 0; i < G.arcnum; ++i)
	{
		if (count == G.vexnum - 1)
			break;
		if (Father(arcs[i].from) != Father(arcs[i].to))
		{
			Union(arcs[i].from, arcs[i].to);
			ans += arcs[i].len;
			count++;
			cout << GetVex(G,(int)arcs[i].from) << " - " << GetVex(G,(int)arcs[i].to) << endl;
		}
	}
	return ans;
}

int main(void)
{

	MGraph G;
	CreateUDN(G);
	cout << "Prim算法,以a开始:" << endl;
	char u = 'a';
	float ans;
	ans = MiniSpanTree_PRIM(G, u);
	cout << "权值之和 = " << ans << endl;
	cout << "Kruskal算法:" << endl;
	ans = MiniSpanTree_Kruskal(G);
	cout << "权值之和 = " << ans << endl;
	system("pause");
	return 0;
	
}