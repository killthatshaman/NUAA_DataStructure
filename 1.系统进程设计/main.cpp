#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <processthreadsapi.h>
#include <timezoneapi.h>
#include <iomanip>

using namespace std;

DWORD aProcesses[1024];//aProcesses[]数组存储获取到的所有PID
DWORD cbNeeded;// EnumProcesses返回的字节总数,除以sizeof(PID)后即为进程数.
DWORD cProcesses; //总共进程数
PROCESS_MEMORY_COUNTERS pmc[1024]; //全部的进程内存信息
SYSTEMTIME BaseTime; //程序运行时的开始时间
SYSTEMTIME systime; //当前系统时间

typedef struct processinfo{
    DWORD pid;
    TCHAR name[MAX_PATH];
    PROCESS_MEMORY_COUNTERS pmc; //使用内存
    SYSTEMTIME time; //系统时间
    SYSTEMTIME endedtime; //结束时间
    bool visitable; // 标志该进程是否有权限访问
    bool flag; // 0:代表已经结束 1:代表正在运行
    processinfo() :pid(0), visitable(0), flag(0) {}
}PInfo;

typedef struct node{
    PInfo data;
    node* next;
    node* pre;
}PNode,* LinkList;

void getEnumsProcessIDS()
{

    unsigned int i;
    // 获取PID列表
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        cout << "内存不足,退出程序。" << endl;
        exit(0);
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    // 计算返回了多少个PID
    return ;
}

bool GetProcessInfo(PInfo & data)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, data.pid);

    if (hProcess == NULL)
    {
        data.visitable = false;
        return false;
    }
    HMODULE hmod = NULL;
    DWORD cb;
    TCHAR tempProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR unknown[MAX_PATH] = TEXT("<unknown>");
    GetModuleBaseName(hProcess, hmod, tempProcessName, (sizeof(tempProcessName) / sizeof(TCHAR)));
    if (!_tcscmp(tempProcessName, unknown)) //如果没有读取名称的权限就返回false;
    {
        data.visitable = false;
        CloseHandle(hProcess);
        return false;
    }
    else
    {
        _tcscpy_s(data.name, tempProcessName);
    }

    if (!GetProcessMemoryInfo(hProcess, &data.pmc, sizeof(data.pmc))) //如果没有获取内存使用的权限就返回false;
    {
        data.visitable = false;
        CloseHandle(hProcess);
        return false;
    }

    FILETIME ProcessTime[4];
    SYSTEMTIME SysTime[4];
    char buf[64] = { 0 };
    memset(ProcessTime, 0, 4 * sizeof(LPFILETIME));

    if (!GetProcessTimes(hProcess, &ProcessTime[0], &ProcessTime[1], &ProcessTime[2], &ProcessTime[3]))
    {
        data.visitable = false;
        return false;
    }

    for (int i = 0; i < 4; ++i)
    {
        FileTimeToSystemTime(&ProcessTime[i], &SysTime[i]);
    }

    data.time = SysTime[0]; //进程创建的时间
    CloseHandle(hProcess);
    return true;

}

void CalcTimeDiff(SYSTEMTIME A, SYSTEMTIME B, SYSTEMTIME& C)
{
    short h;
    short m;
    short s;

    s = B.wSecond - A.wSecond;
    if (s < 0)
    {
        s += 60;
        B.wMinute -= 1;
    }
    m = B.wMinute - A.wMinute;
    if (m < 0)
    {
        m += 60;
        B.wHour -= 1;
    }
    h = B.wHour - A.wHour;
    if (h < 0)
    {
        h += 24;
        B.wDay -= 1;
    }

    C.wHour = h;
    C.wMinute = m;
    C.wSecond = s;
    return;
}
void PrintDoingProcessInfo(PInfo& Process)
{
    SYSTEMTIME temp;
    CalcTimeDiff(Process.time, systime, temp);
    _tprintf(TEXT("%-32s\t%-6u\t%-02dhour %-02dmin %-2dsecond\t0x%-10pBytes\n"), Process.name, Process.pid,  
        temp.wHour, temp.wMinute, temp.wSecond,
        Process.pmc.PagefileUsage);
  //  _tprintf(TEXT("%s\t"), Process.name);
   // cout << Process.pid << endl;
    return;

}

void PrintEndedProcessInfo(PInfo& Process)
{
    SYSTEMTIME temp1, temp2;
    CalcTimeDiff(Process.time, Process.endedtime, temp1);
    CalcTimeDiff(Process.endedtime, systime, temp2);

    _tprintf(TEXT("%-32s\t%-6u\t%-2dhour %-02dmin %-02dsecond\t%-02dhour %-02dmin %-02dsecond\n"), Process.name,Process.pid,
        temp1.wHour, temp1.wMinute, temp1.wSecond,
        temp2.wHour, temp2.wMinute, temp2.wSecond);
    return;
}

bool MatchPinfo(PInfo& A, PInfo& B) //存在同名的不同PID进程 始做不同进程
{
    bool flag = true;
    if ( _tcscmp(A.name, B.name) || A.pid != B.pid ) 
    {
        flag = false;
    }
    //else if (A.time.wHour != B.time.wHour || A.time.wDay != B.time.wDay || A.time.wMinute != B.time.wMinute)
    //{
    //    flag = false;

    //}
    return flag;
}

void InitList(LinkList& L)
{

    L = (LinkList)malloc(sizeof(PNode));
    L->next = NULL;
    L->pre = NULL;
    L->data.visitable = false;
    return;

}

void DestoryList(LinkList& L)
{
    LinkList cur = L->next,ptr;
    L->next = NULL;
    ptr = cur;
    while (cur != NULL)
    {
        ptr = cur->next;
        free(cur);
        cur = ptr;
    }
    return;
}

void LinkListInsert(LinkList& L, LinkList& P) //按照内存消耗从小到大插入单链表
{

    LinkList ptr = L, pre;
    pre = ptr;
    ptr = L->next;
    while (ptr != NULL && ptr->data.pmc.PagefileUsage < P->data.pmc.PagefileUsage)
    {
        pre = ptr;
        ptr = ptr->next;
    }

    pre->next = P;
    P->next = ptr;
    return;

}

void DuLinkListInsert(LinkList& L, LinkList& P) //按照内存消耗从小到大插入双向链表
{
    LinkList ptr = L,pre;
    pre = ptr;
    ptr = L->next;
    if (ptr == NULL)
    {
        L->next = P;
        P->pre = L;
        P->next = NULL;
        return;
    }
    while (ptr->next != NULL && ptr->data.pmc.PagefileUsage < P->data.pmc.PagefileUsage ) //PagefileUsage是所使用的虚拟内存总量
    {
        pre = ptr;
        ptr = ptr->next;
    }
    if (ptr->data.pmc.PagefileUsage < P->data.pmc.PagefileUsage)
    {
        ptr->next = P;
        P->pre = ptr;
        P->next = NULL;
        return;
    }
    else
    {
        pre->next = P;
        P->next = ptr;
        ptr->pre = P;
        P->pre = pre;
    }

    return;
}

void InitDuListProcesses(LinkList &L)
{
    LinkList ptr;
    for (int i = 0; i < cProcesses; ++i)
    {
        ptr = (LinkList)malloc(sizeof(PNode));
        ptr->next = ptr->pre = NULL;
        ptr->data.pid = aProcesses[i];
        if (GetProcessInfo(ptr->data)) //如果该进程具有获取信息的权限
        {
            DuLinkListInsert(L, ptr);
        }
    }
    return;

}

void InitListProcesses(LinkList& L)
{
    LinkList ptr;
    for (int i = 0; i < cProcesses; ++i)
    {
        ptr = (LinkList)malloc(sizeof(PNode));
        ptr->next = ptr->pre = NULL;
        ptr->data.pid = aProcesses[i];
        if (GetProcessInfo(ptr->data)) //如果该进程具有获取信息的权限
        {
            LinkListInsert(L, ptr);
        }
    }
    return;
}

void LinkListTraverse(LinkList& L)
{
    LinkList ptr = L->next;
    while (ptr != NULL)
    {
        PrintDoingProcessInfo(ptr->data);
        ptr = ptr->next;
    }
    return;

}
void EndedTraverse(LinkList& L)
{
    LinkList ptr = L->next;
    while (ptr != NULL)
    {
        PrintEndedProcessInfo(ptr->data);
        ptr = ptr->next;
    }
    return;
}

bool LinkListSearch(LinkList& L, PInfo& P) 
{
    LinkList ptr = L->next;
    bool flag = false;
    while (ptr != NULL && !flag)
    {
        if (MatchPinfo(P, ptr->data))
        {
            flag = true;
            break;
        }
        ptr = ptr->next;
    }

    return flag;

}

bool DuLinkListDelete(LinkList& L, PInfo& P) //双向链表的删除节点
{
    LinkList ptr = L->next,pre = L;
    bool flag = false;
    while (ptr->next != NULL) //不是最后一个节点
    {
        if (MatchPinfo(P, ptr->data))
        {
            flag = true;
            ptr->pre->next = ptr->next;
            ptr->next->pre = ptr->pre;
            break;
        }
        pre = ptr;
        ptr = ptr->next;
    }
    if (MatchPinfo(P, ptr->data)) //是最后一个节点
    {
        ptr->pre->next = ptr->next;
        ptr->pre = NULL;
        
        flag = true;
    }

    return flag;
    
}

bool LinkListDelete(LinkList& L, PInfo& P) //单链表删除节点
{
    LinkList ptr = L->next, pre = L;
    bool flag = false;
    while (ptr != NULL)
    {
        if (MatchPinfo(P, ptr->data))
        {
            flag = true;
            pre->next = ptr->next;
            
            break;
        }

    }
    return flag;
}

bool CheckEnded(LinkList& L,PInfo& process)
{
    if (LinkListSearch(L, process)) //找到返回0 没找到返回1
        return false;
    return true;

}




int main(void)
{
    getEnumsProcessIDS();

    LinkList Initial; //初始的全部进程
    
    InitList(Initial);
    InitDuListProcesses(Initial);
    LinkList Current; //正在执行的进程
    LinkList ended; //已经结束的进程
    InitList(Current);
    InitList(ended);
    int count = 1;
    cout.setf(ios::left); //左对齐
    while (1)
    {
        getEnumsProcessIDS();
        
        cout <<"-----------------------------------------time = " << count <<"----------------------------------------"<< endl;
        cout << setw(40) << "进程名" << setw(8) << "PID" << setw(24) << "运行持续时间" << setw(17) << "占用虚拟内存" << endl;
        GetSystemTime(&systime);//获取当前系统时间
        InitDuListProcesses(Current);
        LinkList p = Initial->next;
        LinkList temp;
        while (p != NULL)
        {
            temp = p->next;
            if (CheckEnded(Current, p->data)) //如果该进程已经结束那么就从current里面删除并且放到ended里
            {
                p->data.endedtime = systime;
                DuLinkListDelete(Initial, p->data);
                LinkListInsert(ended, p);
            }
            p = temp;
            
        }
        LinkList e = ended->next;
        while (e != NULL)
        {
            temp = e->next;
            if (LinkListSearch(Current,e->data))
            {
                LinkListDelete(ended, e->data);
                DuLinkListInsert(Initial, e);
            }
            e = temp;
        }
        LinkListTraverse(Current);
        cout << "--------------------------------------已经结束的进程-------------------------------------" << endl;
        cout << setw(40) << "进程名" << setw(8) << "PID" << setw(24) << "运行持续时间" << setw(24) << "进程结束时间" << endl;
        count++;
        EndedTraverse(ended);
        Sleep(1000);
        DestoryList(Current);
    }
    DestoryList(Initial);
    DestoryList(ended);
    return 0;
    


    
    

 
}