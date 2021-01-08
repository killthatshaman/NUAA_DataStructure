//样例输入
//1 1 2 2 2 1 1 2 2 0
//样例输出
//22
#include<iostream>
#include<stdlib.h>
#include<algorithm>
using namespace std;
int main(void)
{
	int ans = 0;
	int sc = 1;
	int x;
	cin >> x;
	while (x)
	{
		if (x == 1)
		{
			sc = 1;
			ans += sc;
		}
		else if (x == 2)
		{
			if (sc == 1)
			{
				sc = 2;
				ans += sc;
			}
			else
			{
				sc += 2;
				ans += sc;
			}
		}
		cin >> x;
	}
	cout << ans;
}