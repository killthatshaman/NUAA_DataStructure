#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<string.h>
#include<iomanip>
#define MAX_ASCII 129 //ascii���һ��128���ַ�
using namespace std;

typedef struct {
	int count; //���ִ���
	char code; //�ַ�
}Codechar;

typedef struct {
	int weight;
	int parent, lchild, rchild;
	char ch;
}HTNode, * HuffmanTree;
typedef char** HuffmanCode;

void Select(HuffmanTree HT, int n, int& s1, int& s2)
{
	int i = 1;
	int j = 1;
	s1 = s2 = 0;
	while (!s1)
	{
		if (HT[j].parent == 0)
			s1 = j;
		++j;
	}
	for (i = 1; i <= n; ++i)
	{
		if (HT[i].parent != 0)
			continue;
		if (HT[i].weight < HT[s1].weight)
			s1 = i;
	}

	while (!s2)
	{
		if (HT[j].parent == 0 && j != s1)
			s2 = j;
		++j;
	}
	for (i = 1; i <= n; ++i)
	{
		if (HT[i].parent != 0 || i == s1)
			continue;
		if (HT[i].weight < HT[s2].weight)
			s2 = i;

	}
	return;
}

void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, Codechar *w, int n)
{

	int i, j, m, s1, s2, start;
	char* cd;
	unsigned int c, f;
	HuffmanTree p;
	if (n <= 1) return;
	m = 2 * n - 1;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));  // 0�ŵ�Ԫδ��
	for (i = 1; i <= n; ++i)
		HT[i] = { w[i - 1].count,0,0,0,w[i-1].code };

	for (i = n + 1; i <= m; ++i)
		HT[i] = { 0,0,0,0,0 };

	for (i = n + 1; i <= m; i++)  // ����������
	{
		Select(HT, i - 1, s1, s2);  // ��HT[1..i-1]��ѡ��parentΪ0��weight��С��������㣬����ŷֱ�Ϊs1��s2��
		HT[s1].parent = i;  HT[s2].parent = i;
		HT[i].lchild = s1;  HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}

	//��ÿ���ַ��Ĺ���������
	HC = (HuffmanCode)malloc((n + 1) * sizeof(char*)); //�ַ������ͷָ��
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= n; ++i)                        // ����ַ������������
	{
		start = n - 1;                          // ���������λ��
		for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) //��Ҷ�ӵ��������
		{
			if (HT[f].lchild == c) cd[--start] = '0';
			else cd[--start] = '1';
		}

		HC[i] = (char*)malloc((n - start) * sizeof(char));  // Ϊ��i���ַ��������ռ�
		strcpy(HC[i], &cd[start]);
	}
	free(cd);   // �ͷŹ����ռ�

}

void HuffmanDeCode(HuffmanCode& HC, int n, int *lens,int ct,Codechar * seleted)
{
	fstream infile,outfile;
	char hcode[MAX_ASCII];
	char temp_hcode[MAX_ASCII];
	string ans;
	memset(hcode, 0, MAX_ASCII * sizeof(char));
	memset(temp_hcode, 0, MAX_ASCII * sizeof(char));
	infile.open("code.dat", ios::in ); //��ȡ�������ļ����н���
	outfile.open("recode.txt", ios::out);
	for (int i = 0; i < ct; ++i)
	{
		infile.read((char*)temp_hcode,  lens[i]);
		//temp_hcode[i] = '\x00';

		//for (int w = 0; w < lens[i]; ++w)
		//{
		//	_itoa(temp_hcode[w], &hcode[w], 10);
		//}
		for (int j = 1; j <= n; ++j)
		{
			if (strcmp(temp_hcode, HC[j]) == 0)
			{
				outfile << seleted[j - 1].code;
				ans += seleted[j - 1].code;
				memset(temp_hcode, 0, MAX_ASCII * sizeof(char));
				break;
			}
		}
	}
	cout << "������Ϊ:" << endl;
	cout << ans << endl;
	infile.close();
	outfile.close();
	return;
}
int main()
{
	fstream file;
	
	file.open("source.txt", ios::in);
	if (!file)
	{
		cout << "�ļ���ʧ��" << endl;
		exit(0);
	}
	char source[10000];
	int ct = 0;
	int n = 0; //�ַ�����
	Codechar chs[MAX_ASCII]; //���128���ַ�
	Codechar seleted[MAX_ASCII];
	for (int i = 0; i < MAX_ASCII; ++i)
	{
		chs[i].code = (char)i;
		chs[i].count = 0;
	}

	while (!file.eof())
	{
		file.get(source[ct]);
		if (chs[source[ct]].count == 0) n++;
		chs[source[ct]].count++;
		ct++;
		if (file.fail())
			break;
	}
	file.close();
	HuffmanTree HT;
	HuffmanCode HC;
	bool used[MAX_ASCII];
	for (int i = 0; i < MAX_ASCII; ++i)
	{
		used[i] = false;
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < MAX_ASCII; ++j)
		{
			if (chs[j].count != 0 && !used[j])
			{
				seleted[i].code = chs[j].code;
				seleted[i].count = chs[j].count;
				used[j] = true;
				break;
			}
		}
	}
	HuffmanCoding(HT, HC, seleted, n);
	//��������Huffman.txt��
	file.open("Huffman.txt", ios::out);
	file.setf(ios::left);
	file << setw(8) << "�ַ�" << setw(10) << "���ִ���" << setw(10) << "Huffman����" << endl;
	cout << setw(18) << "�ַ�" << setw(20) << "���ִ���" << setw(20) << "Huffman����" << endl;

	for (int i = 1; i <= n; ++i)
	{
		file << setw(8) << seleted[i-1].code << setw(10) << seleted[i-1].count << setw(10) << HC[i] << endl;
		cout << setw(18) << seleted[i-1].code << setw(20) << seleted[i-1].count << setw(20) << HC[i] << endl;
	}
	cout << "Huffman.txt�������" << endl;
	file.close();

	cout << "��ʼ�����������浽code.dat��" << endl;
	system("pause");
	int* lens = new int[ct]; //��¼ÿ���ַ��ĳ���
	file.open("code.dat", ios::out);
	for (int i = 0; i < ct; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			if (seleted[j - 1].code == source[i])
			{
				
				//for (int w = 0; w < strlen(HC[j]); ++w)
				//{
				//	int temp_char = (int)(HC[j][w] - '0');
				//	file.write((char *)&temp_char, sizeof(int));
				//	
				//}
				file << HC[j];
				lens[i] = strlen(HC[j]);
				break;
			}
		}	
	}
	file.close();
	cout << "code.dat�ļ������ɹ�" << endl;

	cout << "��ʼ����" << endl;
	HuffmanDeCode(HC,n,lens,ct,seleted);
	system("pause");
	
	cout << "�������" << endl;
	delete[]lens;

	return 0;
}