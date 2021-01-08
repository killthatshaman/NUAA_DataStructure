#include<iostream>
#include<stdlib.h>
#include<fstream>
using namespace std;
#define LH 1 //��� 
#define EH 0 //�ȸ�
#define RH -1 //�Ҹ� 
#define MAX_NUM 501 
typedef int ElemType; //��int��ΪԪ������


typedef struct BSTNode
{
	ElemType data;
	int bf;//����ƽ������
	struct BSTNode* lchild, * rchild;//���Һ���ָ��
}BSTNode, * BSTree;


typedef struct {
	int head;
	int tail;
	BSTree base[100];
} Queue; //����BFS

void QueuePush(Queue& q, BSTree &e)
{
	q.base[q.tail] = e;
	q.tail++;
}

void QueuePop(Queue& q)
{
	q.head++;
}

BSTree QueueFront(Queue& q)
{
	return q.base[q.head];
}

bool QueueEmpty(Queue q)
{
	return q.head == q.tail;
}


bool SearchBST(BSTree& T, ElemType e,BSTree f,BSTree & p)
{
	if (!T)
	{
		p = f;
		return false;
	}

	if (T->data == e)
	{
		p = T;
		return true;
	}
	else if (T->data > e)
	{
		return SearchBST(T->lchild, e, T, p);
	}
	else
	{
		return SearchBST(T->rchild, e, T, p);
	}
}

void R_Rotate(BSTree& p) 
{
	//����*pΪ���Ķ�������������������������֮��pָ���µ�������㣬��
	//��ת����֮ǰ���������ĸ��ڵ� 
	BSTNode* lc;
	lc = p->lchild;	//lcָ��*p���������ĸ���� 
	p->lchild = lc->rchild;	//lc���������ҽ�Ϊ*p��������
	lc->rchild = p;
	p = lc;		//pָ���µĽ�� 

}

void L_Rotate(BSTree& p) 
{
	//����*pΪ���Ķ�����������������������֮���pָ���µ�������㣬��
	//��ת����֮ǰ���������ĸ����
	BSTNode* rc;
	rc = p->rchild;//rcָ���*p���������ĸ����
	p->rchild = rc->lchild;//rc���������ҽ�Ϊ*p��������
	rc->lchild = p; 
	p = rc;//pָ���µĸ��ڵ� 
}

bool EQ(ElemType e1, ElemType e2)
{
	if (e1 == e2)
		return true;
	return false;
}

int LT(ElemType e1, ElemType e2) 
{
	if (e1 < e2)
		return true;
	return false;
}


void LeftBalance(BSTree& T) 
{
	//����ָ��T��ָ���Ϊ���Ķ�����������ƽ����ת�������㷨����ʱ��ָ��Tָ��
	//�µĸ����
	BSTNode* lc, * rd;
	lc = T->lchild;   //lcָ��*T������������� 
	switch (lc->bf) 
	{		//���*T����������ƽ��ȣ�������Ӧƽ�⴦�� 
		case LH:			//�½�������*T�����ӵ��������ϣ�Ҫ������������ 
			T->bf = lc->bf = EH;
			R_Rotate(T);
			break;
		case RH:	//�½�������*T�����ӵ��������ϣ�Ҫ��˫������ 
			rd = lc->rchild;//rdָ��*T�����ӵ���������
			switch (rd->bf) 
			{
			case LH:
				T->bf = RH;
				lc->bf = EH;
				break;
			case EH:
				T->bf = lc->bf = EH;
				break;
			case RH:
				T->bf = EH;
				lc->bf = LH;
				break;
			}
			rd->bf = EH;
			L_Rotate(T->lchild);  //��*T��������������ƽ�⴦��
			R_Rotate(T);		  //��*T������ƽ�⴦�� 
	}
}

void RightBalance(BSTree& T)     //����ָ�����ָ���Ϊ���Ķ���������ƽ����ת����
{
	BSTree rc, ld;
	rc = T->rchild;
	switch (rc->bf)
	{
	case RH:
		T->bf = rc->bf = EH;
		L_Rotate(T);
		break;
	case LH:
		ld = rc->lchild;
		switch (ld->bf)
		{
		case RH:
			T->bf = LH; 
			rc->bf = EH;
			break;
		case EH:
			T->bf = rc->bf = EH; 
			break;
		case LH:
			T->bf = EH; 
			rc->bf = RH; 
			break;
		}
		ld->bf = EH;
		R_Rotate(T->rchild);
		L_Rotate(T);
	}
}

int InsertAVL(BSTree& T, ElemType e, bool& taller) {
	//����ƽ��Ķ���������T�в����ں�e����ͬ�ؼ��ֵĽ�㣬�����һ������Ԫ��
	//Ϊe���½ڵ㣬������1�����򷵻�0.��������ʹ����������ʧȥƽ�⣬����ƽ��
	//��ת������������taller��ӳT�������
	if (T == NULL) 
	{
		//�����½ڵ㣬�������ߡ�����tallerΪTRUE
		T = new BSTNode;
		T->data = e;
		T->lchild = T->rchild = NULL;
		T->bf = EH;
		taller = true;
	}
	else 
	{
		if (EQ(e, T->data)) //�����Ѿ����ں�e����ͬ�ؼ��ֵĽ�� 
		{   
			taller = false;	  //���ٽ��в��� 
			return 0;
		}
		if (LT(e, T->data))//�����TС�������*T���������н������� 
		{	
			if (!InsertAVL(T->lchild, e, taller)) 
				return 0;	//δ����
			if (taller) {					//�Ѳ��뵽*T���������������������� 
				switch (T->bf) 
				{		//���*T��ƽ��� 
					case LH:		//ԭ�����������������ߣ���Ҫ����ƽ�⴦�� 
						LeftBalance(T);
						taller = false;
						break;
					case EH:		//ԭ�����������ȸߣ��������������߶�ʹ������ 
						T->bf = LH;
						taller = true;
						break;
					case RH:		//ԭ�����������������ߣ������������ȸ� 
						T->bf = EH;
						taller = false;
						break;
				}
			}
		}
		else { //Ӧ������*T���������н������� 
			if (!InsertAVL(T->rchild, e, taller)) 
				return 0;//δ���� 
			if (taller)
				switch (T->bf) 
				{
					case LH:
						T->bf = EH;
						taller = false;
						break;
					case EH:
						T->bf = RH;
						taller = true;
						break;
					case RH:
						RightBalance(T);
						taller = false;
						break;
				}
		} 
	}
	return 1;
} 

int DeleteAVL(BSTree& T, ElemType e, bool& shorter) {
	//����ƽ��Ķ���������T�д��ں�e����ͬ�ؼ��ֵĽ�㣬��ɾ�����Ԫ��
	//������1�����򷵻�0������ɾ����ʹ����������ʧȥƽ�⣬����ƽ����ת����
	if (T == NULL) 
	{
		return 0; //û���ҵ���ɾ�ڵ�
	}
	else 
	{
		if (EQ(e, T->data))  //�ҵ���ɾ���ڵ� ��ʼɾ������
		{
			BSTNode* p = NULL;
			if (T->lchild == NULL) //������Ϊ��
			{
				p = T;	
				T = T->rchild; 
				free(p);
				shorter = true;
			}
			else if (T->rchild == NULL) //������Ϊ��
			{
				
				p = T;
				T = T->lchild;
				free(p);
				shorter = true;

			}
			else //�����������������
			{
				p = T->lchild;
				while (p->rchild != NULL) //ת��,Ȼ�����ҵ���ͷ
				{
					p = p->rchild;
				}
				T->data = p->data;

				DeleteAVL(T->lchild, p->data, shorter); //ɾ��ǰ����� 
			}
		}
		else if (LT(e,T->data))
		{
			if (!DeleteAVL(T->lchild, e, shorter)) 
			{
				return 0;
			}
			if (shorter) 
			{
				switch (T->bf) 
				{
				case LH:
					T->bf = EH;
					shorter = true;
					break;
				case EH:
					T->bf = RH;
					shorter = false;
					break;
				case RH:
					RightBalance(T);
					if (T->rchild->bf == EH) 
					{
						shorter = false;
					}
					else
						shorter = true;
					break;
				}
			}
		}
		else//��������������
		{
			
			if (!DeleteAVL(T->rchild, e, shorter))
				return 0;

			if (shorter) 
			{
				switch (T->bf)
				{
				case LH:
					LeftBalance(T);//������ƽ�⴦��
					if (T->lchild->bf == EH) 
					{
						shorter = false;
					}
					else 
						shorter = true;
					break;
				case EH:
					T->bf = LH;
					shorter = false;
					break;
				case RH:
					T->bf = EH;
					shorter = true;
					break;
				}
			}
		}
	}
	return 1;
}


void InOrderTraverse(BSTree& T) //�������
{

	if (T == NULL)
		return;

	InOrderTraverse(T->lchild);
	cout << T->data << " ";
	InOrderTraverse(T->rchild);

}

void LevelOrderTraverse(BSTree& T)
{
	if (!T)
		return;
	int curLevelcount = 1;
	int nextLevelcount = 0;
	Queue q;
	q.head = q.tail = 0;
	QueuePush(q, T);
	BSTree temp;
	while (!QueueEmpty(q))
	{
		temp = QueueFront(q);
		QueuePop(q);
		cout << temp->data << " ";
		curLevelcount--;
		if (temp->lchild != NULL)
		{
			QueuePush(q,temp->lchild);
			nextLevelcount++;
		}
		if (temp->rchild != NULL)
		{
			QueuePush(q, temp->rchild);
			nextLevelcount++;
		}
		if (curLevelcount == 0)
		{
			cout << endl;
			curLevelcount = nextLevelcount;
			nextLevelcount = 0;
		}
	}
		
}
int main(void)
{
	ifstream infile;
	infile.open("data.txt");
	if (!infile)
	{
		cout << "���ļ�ʧ��" << endl;
		exit(0);
	}
	cout << "��ʼ����..." << endl;
	int count = 0;
	int num[MAX_NUM];
	while (!infile.eof())
	{
		infile >> num[count];
		if (infile.fail())
			break;
		count++;
	}
	infile.close();
	BSTree T = NULL;
	bool taller = false;
	bool shorter = false;
	for (int i = 0; i < count; ++i)
	{
		InsertAVL(T, num[i], taller);
	}
	cout << "������ɣ��������ƽ�������:" << endl;
	LevelOrderTraverse(T);
	cout << endl;
	int cmd;

	while (1)
	{
		cout << "������� 1.���� 2.���� 3.ɾ�� 4.�˳�:";
		cin >> cmd;
		if (cmd == 1)
		{
			cout << "����Ҫ�����ֵ:";
			int d;
			cin >> d;
			InsertAVL(T, d, taller);
			cout << "�����ƽ�������Ϊ:" << endl;
			LevelOrderTraverse(T);
			cout << endl;
		}
		else if (cmd == 2)
		{
			cout << "����Ҫ���ҵ�ֵ:";
			int d;
			BSTree p;
			cin >> d;
			if (SearchBST(T, d, NULL, p))
			{
				cout << "���ҳɹ�" << endl;
			}
			else
				cout << "����ʧ��" << endl;

		}
		else if (cmd == 3)
		{
			cout << "����Ҫɾ����ֵ:";
			int d;
			cin >> d;
			if (DeleteAVL(T, d, shorter))
			{
				cout << "ɾ���ɹ�" << endl;
				cout << "ɾ����ƽ�������Ϊ:" << endl;
				LevelOrderTraverse(T);
				cout << endl;
			}
		}
		else if (cmd == 4)
			break;
		else
		{
			cout << "��Ч����,����������" << endl;
			continue;
		}

	}
	return 0;
	
}