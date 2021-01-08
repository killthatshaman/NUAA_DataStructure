//输入样例:
//7 4
//3 1 2 3
//2 1 4
//3 5 6 7
//1 6
//输出样例 :
//4
#include<stdlib.h>
#include<iostream>
#define MAX_STUDENT 30001
#define MAX_CLUB 1001
using namespace std;

int father[MAX_STUDENT]; 

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
int main(void)
{
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; ++i)
	{
		father[i] = i;
	}
	for (int i = 0; i < m; ++i)
	{
		int cnum,first;
		cin >> cnum;
		cin >> first;
		for (int j = 1; j < cnum; ++j)
		{
			int cur;
			cin >> cur;
			Union(cur, first);
		}

	}
	int ans = 0;
	for (int i = 0; i < n; ++i)
	{
		int temp = 0;
		if (Father(i) == i)
		{
			for (int j = 0; j < n; ++j)
			{
				if (Father(j) == i)
				{
					temp++;
				}
			}
			if (temp > ans)
				ans = temp;
		}
	}
	cout << ans;

}
