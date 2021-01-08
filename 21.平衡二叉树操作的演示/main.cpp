#include<iostream>
#include<stdlib.h>
#include<fstream>
using namespace std;
#define LH 1 //左高 
#define EH 0 //等高
#define RH -1 //右高 
#define MAX_NUM 501 
typedef int ElemType; //以int作为元素类型


typedef struct BSTNode
{
	ElemType data;
	int bf;//结点的平衡因子
	struct BSTNode* lchild, * rchild;//左右孩子指针
}BSTNode, * BSTree;


typedef struct {
	int head;
	int tail;
	BSTree base[100];
} Queue; //用于BFS

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
	//对以*p为根的二叉排序树进行右旋处理，处理之后p指向新的树根结点，既
	//旋转助理之前的左子树的根节点 
	BSTNode* lc;
	lc = p->lchild;	//lc指向*p的左子树的根结点 
	p->lchild = lc->rchild;	//lc的右子树挂接为*p的左子树
	lc->rchild = p;
	p = lc;		//p指向新的结点 

}

void L_Rotate(BSTree& p) 
{
	//对以*p为根的二叉排序树作左旋处理，处理之后的p指向新的树根结点，既
	//旋转处理之前的右子树的根结点
	BSTNode* rc;
	rc = p->rchild;//rc指向的*p的右子树的根结点
	p->rchild = rc->lchild;//rc的左子树挂接为*p的右子树
	rc->lchild = p; 
	p = rc;//p指向新的根节点 
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
	//对以指针T所指结点为根的二叉树做做左平衡旋转处理，本算法结束时候，指针T指向
	//新的根结点
	BSTNode* lc, * rd;
	lc = T->lchild;   //lc指向*T的左子树根结点 
	switch (lc->bf) 
	{		//检查*T的左子树的平衡度，并作相应平衡处理 
		case LH:			//新结点插入在*T的左孩子的左子树上，要作单右旋处理 
			T->bf = lc->bf = EH;
			R_Rotate(T);
			break;
		case RH:	//新结点插入在*T的左孩子的右子树上，要作双旋处理 
			rd = lc->rchild;//rd指向*T的左孩子的右子树根
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
			L_Rotate(T->lchild);  //对*T的左子树作左旋平衡处理
			R_Rotate(T);		  //对*T作右旋平衡处理 
	}
}

void RightBalance(BSTree& T)     //对以指针Ｔ所指结点为根的二叉树作右平衡旋转处理
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
	//若在平衡的二叉排序树T中不存在和e有相同关键字的结点，则插入一个数据元素
	//为e的新节点，并返回1，否则返回0.若因插入而使二叉排序树失去平衡，则作平衡
	//旋转处理，布尔变量taller反映T长高与否
	if (T == NULL) 
	{
		//插入新节点，树“长高”，置taller为TRUE
		T = new BSTNode;
		T->data = e;
		T->lchild = T->rchild = NULL;
		T->bf = EH;
		taller = true;
	}
	else 
	{
		if (EQ(e, T->data)) //树中已经存在和e有相同关键字的结点 
		{   
			taller = false;	  //不再进行插入 
			return 0;
		}
		if (LT(e, T->data))//如果比T小则继续在*T的左子树中进行搜索 
		{	
			if (!InsertAVL(T->lchild, e, taller)) 
				return 0;	//未插入
			if (taller) {					//已插入到*T的左子树中且左子树长高 
				switch (T->bf) 
				{		//检查*T的平衡度 
					case LH:		//原本左子树比右子树高，需要做左平衡处理 
						LeftBalance(T);
						taller = false;
						break;
					case EH:		//原本左右子树等高，现因左子树增高而使树增高 
						T->bf = LH;
						taller = true;
						break;
					case RH:		//原本右子树比左子树高，现左右子树等高 
						T->bf = EH;
						taller = false;
						break;
				}
			}
		}
		else { //应继续在*T的右子树中进行搜索 
			if (!InsertAVL(T->rchild, e, taller)) 
				return 0;//未插入 
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
	//若在平衡的二叉排序树T中存在和e有相同关键字的结点，则删除这个元素
	//并返回1，否则返回0；若因删除而使二叉排序树失去平衡，则作平衡旋转处理
	if (T == NULL) 
	{
		return 0; //没有找到待删节点
	}
	else 
	{
		if (EQ(e, T->data))  //找到待删除节点 开始删除操作
		{
			BSTNode* p = NULL;
			if (T->lchild == NULL) //左子树为空
			{
				p = T;	
				T = T->rchild; 
				free(p);
				shorter = true;
			}
			else if (T->rchild == NULL) //右子树为空
			{
				
				p = T;
				T = T->lchild;
				free(p);
				shorter = true;

			}
			else //如果左右子树都存在
			{
				p = T->lchild;
				while (p->rchild != NULL) //转左,然后向右到尽头
				{
					p = p->rchild;
				}
				T->data = p->data;

				DeleteAVL(T->lchild, p->data, shorter); //删除前驱结点 
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
		else//进入右子树查找
		{
			
			if (!DeleteAVL(T->rchild, e, shorter))
				return 0;

			if (shorter) 
			{
				switch (T->bf)
				{
				case LH:
					LeftBalance(T);//进行左平衡处理
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


void InOrderTraverse(BSTree& T) //中序遍历
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
		cout << "打开文件失败" << endl;
		exit(0);
	}
	cout << "开始建树..." << endl;
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
	cout << "建树完成，层序遍历平衡二叉树:" << endl;
	LevelOrderTraverse(T);
	cout << endl;
	int cmd;

	while (1)
	{
		cout << "输入操作 1.插入 2.查找 3.删除 4.退出:";
		cin >> cmd;
		if (cmd == 1)
		{
			cout << "输入要插入的值:";
			int d;
			cin >> d;
			InsertAVL(T, d, taller);
			cout << "插入后平衡二叉树为:" << endl;
			LevelOrderTraverse(T);
			cout << endl;
		}
		else if (cmd == 2)
		{
			cout << "输入要查找的值:";
			int d;
			BSTree p;
			cin >> d;
			if (SearchBST(T, d, NULL, p))
			{
				cout << "查找成功" << endl;
			}
			else
				cout << "查找失败" << endl;

		}
		else if (cmd == 3)
		{
			cout << "输入要删除的值:";
			int d;
			cin >> d;
			if (DeleteAVL(T, d, shorter))
			{
				cout << "删除成功" << endl;
				cout << "删除后平衡二叉树为:" << endl;
				LevelOrderTraverse(T);
				cout << endl;
			}
		}
		else if (cmd == 4)
			break;
		else
		{
			cout << "无效命令,请重新输入" << endl;
			continue;
		}

	}
	return 0;
	
}