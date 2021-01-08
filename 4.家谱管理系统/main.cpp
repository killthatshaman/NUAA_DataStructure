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


void CreateFamily(Family& F,fstream & infile,Family p) //先序建树 p是父节点
{
	Info info;
	infile >> info.name >> info.birthday.year >> info.birthday.month >> info.birthday.day 
		>> info.marry >> info.addr >> info.live >> info.death_date.year >> info.death_date.month >> info.death_date.day ;
	if (strcmp(info.name, "NULL") == 0 || infile.fail()) //在文件中以NULL作为建树的结束
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
		//NULL代表结束
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

	cout << "姓名:" << inf.name << endl;
	cout << "出生日期:" << inf.birthday.year << "年" << inf.birthday.month << "月" << inf.birthday.day << "日" << endl;
	cout << "是否结婚:";
	if (inf.marry)
		cout << " 已婚" << endl;
	else
		cout << " 未婚" << endl;
	cout << "地址:" << inf.addr << endl;
	cout << "是否健在:";
	if (inf.live)
		cout << " 健在" << endl;
	else
	{
		cout << " 已去世" << endl;
		cout << "去世日期:" << inf.death_date.year << "年" << inf.death_date.month << "月" << inf.death_date.day << "日" << endl;
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
		cout << "--------------本人信息--------------" << endl;
		PrintInfo(F->info);
		if (F->parent)
		{
			cout << "--------------父亲信息--------------" << endl;
			PrintInfo(F->parent->info);
		}
		else
		{
			cout << "--------------不存在父亲--------------" << endl;
		}

		if (F->child)
		{
			cout << "--------------孩子信息--------------" << endl;
			PrintChildInfo(F->child);
		}
		else
		{
			cout << "--------------不存在孩子--------------" << endl;
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
		cout << "--------------本人信息--------------" << endl;
		PrintInfo(F->info);
		return;
	}
	SearchFamilyByDate(F->child, d, flag);
	SearchFamilyByDate(F->sibling, d, flag);

}

bool isChild(Family& A, Family& B) //A是B的孩子
{
	if (A == NULL || B == NULL)
		return false;
	if (A->parent == B)
		return true;
	return false;
}

bool isSibling(Family& A, Family& B) //A是B的兄弟
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
	cout << "输入第一个人的姓名:" << endl;
	char A[MAX_STR_LEN];
	cin >> A;
	cout << "输入第二个人的姓名:" << endl;
	char B[MAX_STR_LEN];
	cin >> B;

	Search(F, pA, A, flag);
	if (!flag)
	{
		cout << "不存在" << A << "这个人" << endl;
		return;
	}
	flag = false;
	Search(F, pB, B, flag);
	if (!flag)
	{
		cout << "不存在" << B << "这个人" << endl;
		return;
	}
	if (isChild(pB,pA))
	{
		cout << B << "是" << A << "的孩子" << endl;
		return;
	}
	if (isChild(pA, pB))
	{
		cout << A << "是" << B << "的孩子" << endl;

		return;
	}
	if (isSibling(pA, pB))
	{
		cout << A << "和" << B << "是亲兄弟" << endl;
		return;
	}
	if (isSibling(pA->parent, pB->parent))
	{
		cout << A << "和" << B << "是堂兄弟" << endl;
		return;
	}
	if (isChild(pB->parent, pA))
	{
		cout << B << "是" << A << "的孙子" << endl;
		return;
	}
	if (isChild(pA->parent, pB))
	{
		cout << A << "是" << B << "的孙子" << endl;
		return;
	}
	cout << A << "和" << B << "之间关系太远了" << endl;
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
	cout << "输入该成员姓名:" << endl;
	char name[MAX_STR_LEN];
	Family p;
	bool flag = false;
	cin >> name;
	Search(F, p, name, flag);
	if (!flag)
	{
		cout << "该成员不存在" << endl;
		return;
	}
	Info childinfo;
	cout << "输入孩子姓名:";
	cin >> childinfo.name;
	cout << "输入孩子生日(年 月 日):";
	cin >> childinfo.birthday.year >> childinfo.birthday.month >> childinfo.birthday.day;
	cout << "输入孩子地址:";
	cin >> childinfo.addr;
	cout << "输入孩子是否已婚(1代表已婚,0代表未婚):";
	cin >> childinfo.marry;
	cout << "输入孩子是否健在:(1代表健在,0代表已去世):";
	cin >> childinfo.live;
	if (!childinfo.live)
	{
		cout << "输入孩子去世日期:(年 月 日)";
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
	cout << "添加完成" << endl;
	return;
}

void EditInfo(Family& F)
{
	cout << "输入该成员姓名:" << endl;
	char name[MAX_STR_LEN];
	Family p;
	bool flag = false;
	cin >> name;
	Search(F, p, name, flag);
	if (!flag)
	{
		cout << "该成员不存在" << endl;
		return;
	}
	cout << "----------------该成员原来信息为----------------" << endl;
	PrintInfo(p->info);
	cout << "-----------------------------------------------" << endl;
	Info newinfo;
	cout << "输入新姓名:";
	cin >> newinfo.name;
	cout << "输入新生日(年 月 日):";
	cin >> newinfo.birthday.year >> newinfo.birthday.month >> newinfo.birthday.day;
	cout << "输入新地址:";
	cin >> newinfo.addr;
	cout << "输入新已婚情况(1代表已婚,0代表未婚):";
	cin >> newinfo.marry;
	cout << "输入新健在情况:(1代表健在,0代表已去世):";
	cin >> newinfo.live;
	if (!newinfo.live)
	{
		cout << "输入去世日期(年 月 日):";
		cin >> newinfo.death_date.year >> newinfo.death_date.month >> newinfo.death_date.day;
	}
	else
	{
		newinfo.death_date.year = newinfo.death_date.month = newinfo.death_date.day = 0;
	}
	p->info = newinfo;
	cout << "修改成功" << endl;
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
	cout << "--------------------------------------" << "第" << gen << "代成员信息" 
		<< "----------------------------------------------" << endl;
	cout << setw(10) << "姓名" << setw(10) << "出生年份" << setw(4) << "月" << setw(4) << "日"
		<< setw(16) << "居住地址" << setw(10) << "是否结婚" << setw(10) << "是否健在"
		<< setw(10) << "去世年份" << setw(4) << "月" << setw(4) << "日" << endl;

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
	cout << "1.图形显示家族信息" << endl;
	cout << "2.显示第n代人的所有信息" << endl;
	cout << "3.按照姓名查询，输出成员信息（包括其本人、父亲、孩子的信息）" << endl;
	cout << "4.按照出生日期查询成员名单" << endl;
	cout << "5.输入两人姓名，确定其关系" << endl;
	cout << "6.给某成员添加孩子" << endl;
	cout << "7.删除某成员（若其还有后代，则一并删除）" << endl;
	cout << "8.修改某成员信息" << endl;
	cout << "9.保存家族信息并退出" << endl;
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
		cout << "打开文件失败" << endl;
		exit(0);
	}
	CreateFamily(F, infile, NULL);
	int cmd;
	cout.setf(ios::left);
	while (1)
	{
		system("cls");
		menu();
		cout << "输入选项:";
		cin >> cmd;
		if (cmd == 1)
		{
			LevelOrderTraverse(F);
			system("pause");
			continue;
		}
		else if (cmd == 2)
		{
			cout << "输入代数N:";
			int n;
			cin >> n;
			PrintGenerates(F, n);
			system("pause");
			continue;
		}
		else if (cmd == 3)
		{
			cout << "输入姓名:";
			char name[MAX_STR_LEN];
			cin >> name;
			bool flag = false;
			SearchFamilyByName(F, name, flag);
			if (!flag)
			{
				cout << "查找失败,不存在这个人" << endl;
			}
			system("pause");
			continue;

		}
		else if (cmd == 4)
		{
			Date birth;
			cout << "输入生日(年 月 日): ";
			cin >> birth.year >> birth.month >> birth.day;
			bool flag = false;
			SearchFamilyByDate(F, birth, flag);
			if (!flag)
			{
				cout << "查找失败,不存在该生日的人" << endl;

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
			cout << "输入要删除的姓名:";
			bool flag = false;
			cin >> name;
			Family ptr;
			Search(F, ptr, name, flag);
			if (!flag)
			{
				cout << "查无此人" << endl;
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
			cout << "输入有误,请重新输入" << endl;
			system("pause");
			continue;
		}

	}
	return 0;
}