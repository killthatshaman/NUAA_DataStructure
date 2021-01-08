//样例输入
//5 2
//4 3 3
//2 2 7
//样例输出
//1 4 3 2 5
#include<iostream>
#include<fstream>
#include<iomanip>
#include<algorithm>

using namespace std;
#define MAX_SIZE 2048

typedef struct {
    int num; //位置
    bool flag; //状态
}key;

typedef struct {
    int num; //编号
    int time; 
    int flag;//0:借 1:还
}event;//事件

typedef struct queue{
    int head;
    int tail;
    event base[MAX_SIZE];
    queue() :head(0), tail(0) {};
} Queue;

void QueuePush(Queue& q, event e)
{
    q.base[q.tail] = e;
    q.tail++;
}

void QueuePop(Queue& q)
{
    q.head++;
}

event QueueFront(Queue& q)
{
    return q.base[q.head];
}

bool QueueEmpty(Queue q)
{
    return q.head == q.tail;
}
int main(void)
{
    Queue Q;
    int N =1 , K = 1;
    int w, s, c;
    ifstream infile;
    infile.open("input.txt");
    if (!infile)
    {
        cout << "打开文件失败" << endl;
        exit(0);
    }
    infile >> N >> K;
    key* keys  = new key[N];
    for (int i = 0; i < N; ++i)
    {
        keys[i].num = i+1;
        keys[i].flag = true;
    }
    event* events = new event[K * 2 + 1];
    for (int i = 0; i < K * 2; i += 2)
    {
        infile >> w >> s >> c ;
        events[i].num = w;
        events[i].time = s;
        events[i].flag = 0; //借
        events[i + 1].num = w;
        events[i + 1].time = c + s;
        events[i + 1].flag = 1; //还
    }
    infile.close();
    for (int i = 0; i < 2 * K; ++i)
    {
        event temp;
        int min = i;
        for (int j = i + 1; j < 2 * K; ++j)
        {
            if (events[j].time < events[min].time)
            {
                min = j;
            }
            else if (events[j].time == events[min].time)
            {
                if (events[j].flag == 1 && events[min].flag == 0)
                {
                    min = j;
                }
                if (events[j].flag == 1 && events[min].flag == 1)
                {
                    if (events[j].num < events[min].num)
                    {
                        min = j;
                    }
                }
            }
        }
        if (min != i)
        {
            temp = events[min];
            events[min] = events[i];
            events[i] = temp;
        }
    }
    for (int i = 0; i < 2 * K; ++i)
    {
        QueuePush(Q, events[i]);

    }
    event temp;
    while (!QueueEmpty(Q))
    {
        temp = QueueFront(Q);
        QueuePop(Q);
        if (temp.flag == 0)
        {
            for (int j = 0; j < N; ++j)
            {
                if (keys[j].num == temp.num)
                {
                    keys[j].num = -1;
                    keys[j].flag = false;
                    break;
                }

            }
        }
        else
        {
            for (int j = 0; j < N; ++j)
            {
                if (keys[j].flag == false)
                {
                    keys[j].num = temp.num;
                    keys[j].flag = true;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < N; ++i)
    {
        cout << keys[i].num << " " ;
    }
    cout << endl;
    delete[]keys;
    delete[]events;
    return 0;
}