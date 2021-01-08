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
	int num; //�м���ֱ����·
	int len; //ֱ����̾���
	int line[10]; //��¼ֱ�﹫����·
	node() :num(0), len(INFINITY) {}

}node;

typedef struct ArcCell {
	node adj;
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
	string vexs[MAX_VERTEX_NUM];       //վ��
	AdjMatrix arcs;
	int vexnum, arcnum;
}MGraph;

typedef struct
{
	string station[75];
	int StatNum;

}bus;

bus Bus[991]; //990����·


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
		cout << "�ļ���ʧ��" << endl;
		exit(0);
	}
	while (!infile.eof())
	{
		string line; //��·
		string station; //վ��
		infile >> line >> station;
		if (infile.fail())
			break;

		int l = 0;
		int count = 0; //һ�����ϵ�վ����
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
				int dis = j - i + 1; //����վ��֮���ֱ�����
				if (G.arcs[x][y].adj.num == 0) //��û��ֱ����· �����ֱ����· ���룬��·�ţ�����
				{
					G.arcs[x][y].adj.len = dis;
					G.arcs[x][y].adj.line[G.arcs[x][y].adj.num] = l;
					G.arcs[x][y].adj.num++;
					G.arcs[y][x] = G.arcs[x][y];
					G.arcnum++;
				}
				else
				{
					if (dis < G.arcs[x][y].adj.len) //������� �������ֱ����� ֮ǰ�ļ�¼���
					{
						G.arcs[x][y].adj.len = dis;
						memset(G.arcs[x][y].adj.line, 0, 10 * sizeof(int));
						G.arcs[x][y].adj.line[0] = l;
						G.arcnum -= G.arcs[x][y].adj.num; //ɾȥ֮ǰ��¼�ı���
						G.arcs[x][y].adj.num = 1;
						G.arcs[y][x] = G.arcs[x][y];
						G.arcnum++;
					}
					else if (dis == G.arcs[x][y].adj.len && G.arcs[x][y].adj.num < 10) //������ͬ �����·
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

int Transfer[MAX_VERTEX_NUM];  //��¼����ת������
int used[MAX_VERTEX_NUM];
int pre[MAX_VERTEX_NUM]; // �������·��

int SearchStatInLine(string station, int line)
{
	for (int i = 0; i < Bus[line].StatNum; ++i)
	{
		if (Bus[line].station[i] == station)
			return i;
	}
	return -1;
}

void DijsktraMiniTrans(MGraph& G, string start, string end) //ת����������
{
	int s = LocateVex(G, start);
	int e = LocateVex(G, end);
	if (s == -1 || e == -1)
	{
		cout << "�Ҳ���վ��" << endl;
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
		for (int i = 0; i < G.vexnum; ++i) //��ת���������ٵĵ�
		{
			if (!used[i] && (min == -1 || Transfer[min] > Transfer[i]))
			{
				min = i;
			}
		}
		if (min == -1 || min == e)
			break;
		used[min] = 1; //�����ѷ��ʹ����㼯
		for (int i = 0; i < G.vexnum; ++i)
		{
			if (G.arcs[min][i].adj.num != 0 && !used[i]) //����·��
			{
				if (Transfer[min] + 1 < Transfer[i])
				{
					Transfer[i] = Transfer[min] + 1;
					pre[i] = min;
				}
			}
		}

	}
	int road[MAX_VERTEX_NUM]; //�˳�·��
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
	cout << "------------------ת������:" << ct << "------------------" << endl;
	int num = 0;
	for (int i = ct; i > 0; --i)
	{
		int cur, next;
		cur = road[i];
		next = road[i - 1];
		int line = G.arcs[cur][next].adj.line[0];
		int x = SearchStatInLine(G.vexs[cur], line);
		int y = SearchStatInLine(G.vexs[next], line);
		cout << "�� " << Bus[line].station[x] << "�� " << line << " ����" << "��" << Bus[line].station[y];
		cout << ",��" << abs(y - x) + 1 << "վ";
		StatCount += abs(y - x) + 1;
		cout << endl;

	}
	cout << "�ܹ�" << StatCount << "վ" << endl;
	return;
}

int MinDis[MAX_VERTEX_NUM];

void DijsktraMiniLen(MGraph& G, string start, string end) //����վ������
{
	int s = LocateVex(G, start);
	int e = LocateVex(G, end);
	if (s == -1 || e == -1)
	{
		cout << "�Ҳ���վ��" << endl;
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
	MinDis[s] = 0; //��¼��̾���
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
		used[min] = 1; //�����ѷ��ʹ����㼯
		for (int i = 0; i < G.vexnum; ++i)
		{
			if (G.arcs[min][i].adj.num > 0 && !used[i])
			{
				if (MinDis[min] + G.arcs[min][i].adj.len < MinDis[i]) //�������·
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
		cout << "��" << Bus[line].station[x] << "��" << line << "���ߵ�" << Bus[line].station[y];
		cout << " ,��" << abs(y - x)+ 1 << "վ" << endl;
		StatCount += abs(y - x)+ 1;

	}
	cout << "ȫ�̹�" << StatCount << "վ," << "ת��" << ct << "��" << endl;
	return;
}

MGraph G;

int main(void)
{
	G.arcnum = G.vexnum = 0;
	cout << "���ڶ�ȡ����..." << endl;
	CreateUDN(G);
	string start, end;
	int cmd;
	while (1)
	{
		system("cls");
		cout << "1.��ѯת���������ٵ�·��" << endl;
		cout << "2.��ѯ����վ�����ٵ�·��" << endl;
		cout << "3.�˳�" << endl;
		cin >> cmd;
		if (cmd == 1)
		{
			cout << "���뿪ʼվ��:";
			cin >> start;
			cout << "�������վ��:";
			cin >> end;
			DijsktraMiniTrans(G, start, end);
			system("pause");
		}
		else if (cmd == 2)
		{
			cout << "���뿪ʼվ��:";
			cin >> start;
			cout << "�������վ��:";
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
			cout << "��������,����������";
			system("pause");
		}
	}
	return 0;
}