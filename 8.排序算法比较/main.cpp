#include<iostream>
#include<stdlib.h>
#include<ctime>
#include<queue>
#include<iomanip>
#include <fstream>
#define MAX_NUM 20001
#define RAND_LIMIT 65536 //随机数最大值
using namespace std;

typedef struct {
    int head;
    int tail;
    int *base ;
} Queue;

bool InitQueue(Queue& q)
{
    q.base = (int*)malloc(sizeof(int) * MAX_NUM);
    q.head = q.tail = 0;
    if (q.base == NULL)
    {
        cout << "malloc失败" << endl;
        return false;
    }
    return true;
}

void DestroyQueue(Queue& q)
{
    free(q.base);
    return;
}

void QueuePush(Queue& q, int e)
{
    q.base[q.tail] = e;
    q.tail++;
}
void QueuePop(Queue& q)
{
    q.head++;
}
int QueueFront(Queue& q)
{
    return q.base[q.head];
}
bool QueueEmpty(Queue q)
{
    return q.head == q.tail;
}

void CreateData() //创建十个样本
{
    fstream outfile;
    char filename[11];
    int temp = 0;
    for (int i = 1; i <= 10; ++i)
    {
        srand(time(0));
        sprintf_s(filename, "data%d.txt", i);
        if (i == 1)
        {
            outfile.open(filename, ios::out);
            for (int j = 0; j < 20000; ++j)
            {
                outfile << j << " ";
                if (j % 16 == 0)
                    outfile << endl;
            }
            outfile.close();
            continue;
        }
        if (i == 2)
        {
            outfile.open(filename, ios::out);
            for (int j = 0; j < 20000; ++j)
            {
                outfile << 20000 - j << " ";
                if (j % 16 == 0)
                    outfile << endl;
            }
            outfile.close();
            continue;
        }
        //cout << endl;
        outfile.open(filename,ios::out);
        if (!outfile)
        {
            cout << "创建数据文件失败" << endl;
            exit(0);
        }
        for (int j = 0; j < 20000; ++j) //20000个数据
        {
            temp = rand() % RAND_LIMIT;
            outfile << temp << " ";
            if (j % 16 == 0) 
                outfile << endl;

        }
        
        outfile.close();
    }
    cout << "...数据创建完成" << endl;
    
    
}
int readData(int * data,int index)
{
    char filename[11];
    sprintf_s(filename, "data%d.txt", index);
    fstream infile;
    infile.open(filename, ios::in);
    if (!infile)
    {
        cout << "打开文件失败" << endl;
        exit(0);
    }
    int ct = 0;
    while (!infile.eof())
    {
        infile >> data[ct];
        if (infile.fail())
            break;
        ct++;

    }
    infile.close();
    return ct; //数据数量
}

void BubbleSort(int * data,int n)
{
    for (int i = 0; i < n; i++)
    {
        bool complete = true;
        int temp;
        for (int j = 1; j < n - i; j++)
        {
            if (data[j] < data[j - 1])
            {
                temp = data[j];
                data[j] = data[j - 1];
                data[j - 1] = temp;
                complete = false;
            }
        }
        if (complete)  //如果一趟比较没有交换 说明数据已经有序
            break;
    }
    return;
}

void InsertSort(int* data, int n)
{
    int j;
    for (int i = 1; i < n; ++i)
    {
        if (data[i] < data[i - 1]) //需要插入
        {
            int temp = data[i];
            for (j = i - 1; data[j] > temp; --j)
            {
                data[j + 1] = data[j];

            }
            data[j + 1] = temp;
        }
    }
    return;
}

void SelectSort(int* data, int n)
{
    for (int i = 0; i < n; ++i)
    {
        int min = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (data[min] > data[j])
                min = j;
        }
        if (min != i)
        {
            int temp = data[min];
            data[min] = data[i];
            data[i] = temp;
        }
    }
    return;
}

void ShellInsert(int* data, int n,int dk)
{
    int j;
    for (int i = dk; i < n; ++i)
    {
        if (data[i] < data[i - dk])
        {
            int temp = data[i];
            for (j = i - dk; j >= 0 && data[j] > temp; j -= dk)
            {
                data[j + dk] = data[j];
            }
            data[j + dk] = temp;
        }
    }
    return;
}

void ShellSort(int* data, int n, int dlta[], int t)
{
    for (int k = 0; k < t; ++k)
    {
        ShellInsert(data,n, dlta[k]);
    }
    return;
}

int Partition(int* data, int low, int high)
{
    int temp = data[low];
    int pivotkey = data[low];
    while (low < high)
    {
        while (low < high && data[high] >= pivotkey)
            --high;
        data[low] = data[high];
        while (low < high && data[low] <= pivotkey)
            ++low;
        data[high] = data[low];
    }
    data[low] = temp;
    return low;
}

void QuickSort(int* data, int low,int high)
{
    if (low >= high)
        return;
    int pivotloc = Partition(data, low, high);
    QuickSort(data, low, pivotloc-1);
    QuickSort(data, pivotloc+1, high);

        
}

void HeapAdjust(int* data, int low, int high)
{
    int index = low;
    for (int j = 2 * low + 1; j <= high; j = j*2 + 1)
    {
        if (j < high && data[j] < data[j + 1])
            j++;
        if (data[index] > data[j])
            return ;
        int temp = data[index];
        data[index] = data[j];
        data[j] = temp;
        index = j;
    }
    return;
}
void HeapSort(int* data, int n)
{
    for (int i = (n-1) / 2; i >= 0; --i)
    {
        HeapAdjust(data, i, n);
    }
    for (int i = n - 1; i > 0; --i)
    {
        int temp = data[i];
        data[i] = data[0];
        data[0] = temp;
        HeapAdjust(data, 0, i - 1);
    }
    return;
}

void Merge(int* data, int low, int mid, int high)
{
    int* temp = new int[high - low + 1];
    int count = 0;
    int i = low, j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (data[i] < data[j])
        {
            temp[count] = data[i];
            count++;
            i++;
        }
        else
        {
            temp[count] = data[j];
            count++;
            j++;
        }
            
    }
    while (i <= mid)
    {
        temp[count] = data[i];
        count++;
        i++;
    }
    while (j <= high)
    {
        temp[count] = data[j];
        count++;
        j++;
    }
    for (int k = low; k < low + count; ++k)
    {
        data[k] = temp[k - low];
    }
    delete[]temp;
    return;
}

void MergeSort(int* data, int low, int high)
{
    if (low >= high)
        return;
    int mid = (high + low) / 2;
    MergeSort(data, low, mid);
    MergeSort(data, mid + 1, high);
    Merge(data, low, mid, high);
}

void RadixSort(int* data, int n)
{
    Queue radix[10]; //基数的队列数组
    for (int i = 0; i < 10; ++i)
    {
        InitQueue(radix[i]);
    }
    for (int i = 1; i < RAND_LIMIT; i *= 10) //数值最多五位
    {
        int j;
        for (j = 0; j < n; ++j)
        {
            QueuePush(radix[(data[j] / i) % 10], data[j]); //取第i位作为基数
        }
        j = 0;
        for (int k = 0; k < 10; ++k)
        {
            while (!QueueEmpty(radix[k]))
            {
                data[j] = QueueFront(radix[k]);
                QueuePop(radix[k]);
                j++;
            }
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        DestroyQueue(radix[i]);
    }
    return;

}

void Traverse(int* data, int n)
{
    cout << "排序后的数组内容为:";
    for (int i = 0; i < n; ++i)
    {
        cout << data[i] << " ";
        if (i % 16 == 0)
            cout << endl;
    }

    cout << endl;
    return;
}

clock_t Start, End; //排序开始时间,排序结束时间
double totTime; //使用总时间

int Data[MAX_NUM];
int Sort_data[MAX_NUM];

int main(void)
{
    CreateData();
    int count = 0;
    for (int i = 1; i <= 10; ++i)
    {
        cout << "-------------------------------------"; 
        cout << "data" << i << ".txt";
        count = readData(Data, i);
        cout << "-------------------------------------" << endl;
        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "1.直接插入排序:" << endl;
        cout << "排序开始"<< endl;
        Start = clock();
        InsertSort(Sort_data, count);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;

        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "2.希尔排序:" << endl; //、、、，、基数排序"
        cout << "排序开始" << endl;
        int dlta[5] = { 9,5,3,2,1 };
        Start = clock();
        ShellSort(Sort_data, count, dlta, 5);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;


        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "3.冒泡排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        BubbleSort(Sort_data, count);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "4.快速排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        QuickSort(Sort_data,0, count -1);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "5.选择排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        SelectSort(Sort_data, count);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "6.堆排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        HeapSort(Sort_data, count);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "7.归并排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        MergeSort(Sort_data, 0,count-1);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

        for (int j = 0; j < count; ++j)
        {
            Sort_data[j] = Data[j];
        }
        cout << "8.基数排序:" << endl;
        cout << "排序开始" << endl;
        Start = clock();
        RadixSort(Sort_data, count);
        End = clock();
        cout << "排序结束" << endl;
        totTime = (double)(End - Start) / CLOCKS_PER_SEC;
        cout << "排序时间使用为" << totTime << "S" << endl;

    }
    system("pause");

    return 0;
    
}