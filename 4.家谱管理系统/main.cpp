#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<iomanip>
#define MAX_STR_LEN 101
using namespace std;

typedef struct
{
	int year;
	int month;
	int day;
}Date;

typedef struct 
{
	char name[MAX_STR_LEN]; 
	Date birthday;
	int marry;
	char addr[MAX_STR_LEN];
	int live;
	Date death_date;

}Info;

typedef struct Node 
{
	Info info;
	Node* parent;
	Node* child;
	Node* sibling;

}Person,*Family;

typedef struct {
	int head;
	int tail;
	Family base[100] = { 0 };
} Queue;

void QueuePush(Queue& q, Family e)
{
	q.base[q.tail] = e;
	q.tail++;
}

void QueuePop(Queue& q)
{
	q.head++;
}

Family QueueFront(Queue& q)
{
	return q.base[q.head];
}

bool QueueEmpty(Queue q)
{
	return q.head == q.tail;
}


void CreateFamily(Family& F,fstream & infile,Family p) //������ p�Ǹ��ڵ�
{
	Info info;
	infile >> info.name >> info.birthday.year >> info.birthday.month >> info.birthday.day 
		>> info.marry >> info.addr >> info.live >> info.death_date.year >> info.death_date.month >> info.death_date.day ;
	if (strcmp(info.name, "NULL") == 0 || infile.fail()) //���ļ�����NULL��Ϊ�����Ľ���
		return;

	F = (Family)malloc(sizeof(Person));
	F->info = info;
	F->child = F->sibling = NULL;
	F->parent = p;

	CreateFamily(F->child, infile,F);
	CreateFamily(F->sibling, infile,F->parent);

	
}

void SaveData(Family& F, fstream &outfile)
{
	if (!F)
	{
		outfile << "NULL " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << "NULL " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << endl;
		//NULL�������
		return;
	}
	else 
	{
		outfile << F->info.name << " " << F->info.birthday.year << " " << F->info.birthday.month << " " << F->info.birthday.day << " "
			<< F->info.marry << " " << F->info.addr << " " << F->info.live << " " << F->info.death_date.year << " " << F->info.death_date.month << " " << F->info.death_date.day << " ";

		outfile << endl;
		SaveData(F->child,outfile);
		SaveData(F->sibling,outfile);
	}
	


}

void PrintInfo_Gen(Info& inf)
{

	cout << setw(10) << inf.name << setw(10) << inf.birthday.year << setw(4) << inf.birthday.month << setw(4) << inf.birthday.day
		<< setw(16) << inf.addr << setw(10) << inf.marry << setw(10) << inf.live;
	if (inf.live)
	{
		cout << setw(10) << "----" << setw(4) << "--" << setw(4) << "--";
	}
	else
	{
		cout << setw(10) << inf.death_date.year << setw(4) << inf.death_date.month << setw(4) << inf.death_date.day;
	}
	cout << endl;
}

void PrintInfo(Info& inf)
{

	cout << "����:" << inf.name << endl;
	cout << "��������:" << inf.birthday.year << "��" << inf.birthday.month << "��" << inf.birthday.day << "��" << endl;
	cout << "�Ƿ���:";
	if (inf.marry)
		cout << " �ѻ�" << endl;
	else
		cout << " δ��" << endl;
	cout << "��ַ:" << inf.addr << endl;
	cout << "�Ƿ���:";
	if (inf.live)
		cout << " ����" << endl;
	else
	{
		cout << " ��ȥ��" << endl;
		cout << "ȥ������:" << inf.death_date.year << "��" << inf.death_date.month << "��" << inf.death_date.day << "��" << endl;
	}
	return;
}

void PrintChildInfo(Family& F)
{
	if (!F)
		return;
	Family temp;
	temp = F;
	while (temp)
	{
		PrintInfo(temp->info);
		cout << "-------------------------" << endl;
		temp = temp->sibling;
	}

	return;
}

void Search(Family& F, Family & p,char* name, bool &flag)
{
	if (F == NULL)
		return;
	if (strcmp(name, F->info.name) == 0)
	{
		flag = true;
		p = F;
		return;
	}
	Search(F->child, p, name, flag);
	Search(F->sibling, p, name, flag);

}

void SearchFamilyByName(Family& F, char * name,bool & flag)
{
	if (F == NULL)
		return ;
	if (strcmp(name,F->info.name) == 0 )
	{
		flag = true;
		cout << "--------------������Ϣ--------------" << endl;
		PrintInfo(F->info);
		if (F->parent)
		{
			cout << "--------------������Ϣ--------------" << endl;
			PrintInfo(F->parent->info);
		}
		else
		{
			cout << "--------------�����ڸ���--------------" << endl;
		}

		if (F->child)
		{
			cout << "--------------������Ϣ--------------" << endl;
			PrintChildInfo(F->child);
		}
		else
		{
			cout << "--------------�����ں���--------------" << endl;
		}
		return;
	}

	SearchFamilyByName(F->child, name,flag);
	SearchFamilyByName(F->sibling, name,flag);

}

void SearchFamilyByDate(Family& F, Date& d, bool& flag)
{
	if (F == NULL)
		return;
	if (d.year == F->info.birthday.year && d.month == F->info.birthday.month && d.day == F->info.birthday.day)
	{
		flag = true;
		cout << "--------------������Ϣ--------------" << endl;
		PrintInfo(F->info);
		return;
	}
	SearchFamilyByDate(F->child, d, flag);
	SearchFamilyByDate(F->sibling, d, flag);

}

bool isChild(Family& A, Family& B) //A��B�ĺ���
{
	if (A == NULL || B == NULL)
		return false;
	if (A->parent == B)
		return true;
	return false;
}

bool isSibling(Family& A, Family& B) //A��B���ֵ�
{
	if (A == NULL || B == NULL)
	{
		if (A == B)
			return true;
		else
			return false;
	}
	if (A->parent == B->parent)
		return true;
	return false;
}

void Relationship(Family & F)
{
	Family pA,pB;
	bool flag = false;
	cout << "�����һ���˵�����:" << endl;
	char A[MAX_STR_LEN];
	cin >> A;
	cout << "����ڶ����˵�����:" << endl;
	char B[MAX_STR_LEN];
	cin >> B;

	Search(F, pA, A, flag);
	if (!flag)
	{
		cout << "������" << A << "�����" << endl;
		return;
	}
	flag = false;
	Search(F, pB, B, flag);
	if (!flag)
	{
		cout << "������" << B << "�����" << endl;
		return;
	}
	if (isChild(pB,pA))
	{
		cout << B << "��" << A << "�ĺ���" << endl;
		return;
	}
	if (isChild(pA, pB))
	{
		cout << A << "��" << B << "�ĺ���" << endl;

		return;
	}
	if (isSibling(pA, pB))
	{
		cout << A << "��" << B << "�����ֵ�" << endl;
		return;
	}
	if (isSibling(pA->parent, pB->parent))
	{
		cout << A << "��" << B << "�����ֵ�" << endl;
		return;
	}
	if (isChild(pB->parent, pA))
	{
		cout << B << "��" << A << "������" << endl;
		return;
	}
	if (isChild(pA->parent, pB))
	{
		cout << A << "��" << B << "������" << endl;
		return;
	}
	cout << A << "��" << B << "֮���ϵ̫Զ��" << endl;
	return;
}

void AddSibling(Family& F,Family & p)
{
	if (!F)
		return;
	Family temp = F->sibling;
	Family pre = F;
	while (temp)
	{
		pre = temp;
		temp = temp->sibling;
	}
	pre->sibling = p;
	return;
	
}

void AddChild(Family& F)
{
	cout << "����ó�Ա����:" << endl;
	char name[MAX_STR_LEN];
	Family p;
	bool flag = false;
	cin >> name;
	Search(F, p, name, flag);
	if (!flag)
	{
		cout << "�ó�Ա������" << endl;
		return;
	}
	Info childinfo;
	cout << "���뺢������:";
	cin >> childinfo.name;
	cout << "���뺢������(�� �� ��):";
	cin >> childinfo.birthday.year >> childinfo.birthday.month >> childinfo.birthday.day;
	cout << "���뺢�ӵ�ַ:";
	cin >> childinfo.addr;
	cout << "���뺢���Ƿ��ѻ�(1�����ѻ�,0����δ��):";
	cin >> childinfo.marry;
	cout << "���뺢���Ƿ���:(1������,0������ȥ��):";
	cin >> childinfo.live;
	if (!childinfo.live)
	{
		cout << "���뺢��ȥ������:(�� �� ��)";
		cin >> childinfo.death_date.year >> childinfo.death_date.month >> childinfo.death_date.day;
	}
	else
	{
		childinfo.death_date.year = childinfo.death_date.month = childinfo.death_date.day = 0;
	}
	Family temp = (Family)malloc(sizeof(Person));
	temp->info = childinfo;
	temp->child = temp->sibling = NULL;
	temp->parent = p;
	if(!p->child)
	{
		p->child = temp;
	}
	else
	{
		AddSibling(p->child,temp);
	}
	cout << "������" << endl;
	return;
}

void EditInfo(Family& F)
{
	cout << "����ó�Ա����:" << endl;
	char name[MAX_STR_LEN];
	Family p;
	bool flag = false;
	cin >> name;
	Search(F, p, name, flag);
	if (!flag)
	{
		cout << "�ó�Ա������" << endl;
		return;
	}
	cout << "----------------�ó�Աԭ����ϢΪ----------------" << endl;
	PrintInfo(p->info);
	cout << "-----------------------------------------------" << endl;
	Info newinfo;
	cout << "����������:";
	cin >> newinfo.name;
	cout << "����������(�� �� ��):";
	cin >> newinfo.birthday.year >> newinfo.birthday.month >> newinfo.birthday.day;
	cout << "�����µ�ַ:";
	cin >> newinfo.addr;
	cout << "�������ѻ����(1�����ѻ�,0����δ��):";
	cin >> newinfo.marry;
	cout << "�����½������:(1������,0������ȥ��):";
	cin >> newinfo.live;
	if (!newinfo.live)
	{
		cout << "����ȥ������(�� �� ��):";
		cin >> newinfo.death_date.year >> newinfo.death_date.month >> newinfo.death_date.day;
	}
	else
	{
		newinfo.death_date.year = newinfo.death_date.month = newinfo.death_date.day = 0;
	}
	p->info = newinfo;
	cout << "�޸ĳɹ�" << endl;
	return;
}

void DeleteAll(Family& F)
{
	if (!F)
		return;

	DeleteAll(F->child);
	DeleteAll(F->sibling);
	free(F);
	F = NULL;
	return;
}

void DeletePerson(Family& F)
{
	DeleteAll(F->child);
	if (F->parent->child == F)
	{
		F->parent->child = F->sibling;
		free(F);
		return;
	}
	else
	{
		Family ptr, pre;
		pre = F->parent->child;
		ptr = F->parent->child->sibling;
		while (ptr != NULL)
		{
			if (ptr == F)
			{
				pre->sibling = ptr->sibling;
				free(ptr);
				return;
			}
			pre = ptr;
			ptr = ptr->sibling;
		}
	}
	return;
}

void PrintGenerates(Family& F, int gen)
{
	Queue q1, q2;
	int depth = 1;
	cout << "--------------------------------------" << "��" << gen << "����Ա��Ϣ" 
		<< "----------------------------------------------" << endl;
	cout << setw(10) << "����" << setw(10) << "�������" << setw(4) << "��" << setw(4) << "��"
		<< setw(16) << "��ס��ַ" << setw(10) << "�Ƿ���" << setw(10) << "�Ƿ���"
		<< setw(10) << "ȥ�����" << setw(4) << "��" << setw(4) << "��" << endl;

	q1.head = q1.tail = q2.head = q2.tail = 0;
	QueuePush(q1, F);
	while (!QueueEmpty(q1) || !QueueEmpty(q2))
	{
		Family t;
		if (!QueueEmpty(q1))
		{
			while (!QueueEmpty(q1))
			{
				t = QueueFront(q1);
				QueuePop(q1);
				if (depth == gen)
					PrintInfo_Gen(t->info);
				if (t->child != NULL)
				{
					QueuePush(q2, t->child);
				}
				if (t->sibling != NULL)
				{
					QueuePush(q1, t->sibling);
				}
			}
		}
		else
		{
			while (!QueueEmpty(q2))
			{
				t = QueueFront(q2);
				QueuePop(q2);
				if (depth == gen)
					PrintInfo_Gen(t->info);
				if (t->child != NULL)
				{
					QueuePush(q1, t->child);
				}
				if (t->sibling != NULL)
				{
					QueuePush(q2, t->sibling);
				}
			}
		}
		depth++;

	}
	cout << endl;
	return;
}

void LevelOrderTraverse(Family& F)
{
	if (!F)
		return;
	int curLevelcount = 1;
	int nextLevelcount = 0;
	Queue q1,q2;
	q1.head = q1.tail = q2.head = q2.tail = 0;
	QueuePush(q1, F);
	while (!QueueEmpty(q1) || !QueueEmpty(q2))
	{
		Family t;
		if (!QueueEmpty(q1))
		{
			while (!QueueEmpty(q1))
			{
				t = QueueFront(q1);
				QueuePop(q1);
				cout << t->info.name << " ";
				if (t->child != NULL)
				{
					QueuePush(q2, t->child);
				}
				if (t->sibling != NULL)
				{
					QueuePush(q1, t->sibling);
				}
			}
			cout << endl;
		}
		else
		{
			while (!QueueEmpty(q2))
			{
				t = QueueFront(q2);
				QueuePop(q2);
				cout << t->info.name << " ";
				if (t->child != NULL)
				{
					QueuePush(q1, t->child);
				}
				if (t->sibling != NULL)
				{
					QueuePush(q2, t->sibling);
				}
			}
			cout << endl;
		}


	}
	cout << endl;
	return;
}

void menu()
{	
	cout << "-------------------------------------------------------" << endl;
	cout << "1.ͼ����ʾ������Ϣ" << endl;
	cout << "2.��ʾ��n���˵�������Ϣ" << endl;
	cout << "3.����������ѯ�������Ա��Ϣ�������䱾�ˡ����ס����ӵ���Ϣ��" << endl;
	cout << "4.���ճ������ڲ�ѯ��Ա����" << endl;
	cout << "5.��������������ȷ�����ϵ" << endl;
	cout << "6.��ĳ��Ա��Ӻ���" << endl;
	cout << "7.ɾ��ĳ��Ա�����仹�к������һ��ɾ����" << endl;
	cout << "8.�޸�ĳ��Ա��Ϣ" << endl;
	cout << "9.���������Ϣ���˳�" << endl;
	cout << "-------------------------------------------------------" << endl;
	return;
	
}


int main(void)
{
	fstream infile;
	Family F;
	infile.open("data.txt", ios::in);
	if (!infile)
	{
		cout << "���ļ�ʧ��" << endl;
		exit(0);
	}
	CreateFamily(F, infile, NULL);
	int cmd;
	cout.setf(ios::left);
	while (1)
	{
		system("cls");
		menu();
		cout << "����ѡ��:";
		cin >> cmd;
		if (cmd == 1)
		{
			LevelOrderTraverse(F);
			system("pause");
			continue;
		}
		else if (cmd == 2)
		{
			cout << "�������N:";
			int n;
			cin >> n;
			PrintGenerates(F, n);
			system("pause");
			continue;
		}
		else if (cmd == 3)
		{
			cout << "��������:";
			char name[MAX_STR_LEN];
			cin >> name;
			bool flag = false;
			SearchFamilyByName(F, name, flag);
			if (!flag)
			{
				cout << "����ʧ��,�����������" << endl;
			}
			system("pause");
			continue;

		}
		else if (cmd == 4)
		{
			Date birth;
			cout << "��������(�� �� ��): ";
			cin >> birth.year >> birth.month >> birth.day;
			bool flag = false;
			SearchFamilyByDate(F, birth, flag);
			if (!flag)
			{
				cout << "����ʧ��,�����ڸ����յ���" << endl;

			}
			system("pause");
			continue;
		}
		else if (cmd == 5)
		{
			Relationship(F);
			system("pause");
			continue;
		}
		else if (cmd == 6)
		{
			AddChild(F);
			system("pause");
			continue;
		}
		else if (cmd == 7)
		{
			char name[MAX_STR_LEN];
			cout << "����Ҫɾ��������:";
			bool flag = false;
			cin >> name;
			Family ptr;
			Search(F, ptr, name, flag);
			if (!flag)
			{
				cout << "���޴���" << endl;
				system("pause");
				continue;
			}

			DeletePerson(ptr);
			system("pause");
			continue;
		}
		else if (cmd == 8)
		{
			EditInfo(F);
			system("pause");
			continue;
		}
		else if (cmd == 9)
		{
			fstream outfile;
			outfile.open("data.txt", ios::out);
			SaveData(F, outfile);
			outfile.close();
			break;
		}
		else 
		{
			cout << "��������,����������" << endl;
			system("pause");
			continue;
		}

	}
	return 0;
}