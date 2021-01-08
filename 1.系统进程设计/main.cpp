#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <processthreadsapi.h>
#include <timezoneapi.h>
#include <iomanip>

using namespace std;

DWORD aProcesses[1024];//aProcesses[]����洢��ȡ��������PID
DWORD cbNeeded;// EnumProcesses���ص��ֽ�����,����sizeof(PID)��Ϊ������.
DWORD cProcesses; //�ܹ�������
PROCESS_MEMORY_COUNTERS pmc[1024]; //ȫ���Ľ����ڴ���Ϣ
SYSTEMTIME BaseTime; //��������ʱ�Ŀ�ʼʱ��
SYSTEMTIME systime; //��ǰϵͳʱ��

typedef struct processinfo{
    DWORD pid;
    TCHAR name[MAX_PATH];
    PROCESS_MEMORY_COUNTERS pmc; //ʹ���ڴ�
    SYSTEMTIME time; //ϵͳʱ��
    SYSTEMTIME endedtime; //����ʱ��
    bool visitable; // ��־�ý����Ƿ���Ȩ�޷���
    bool flag; // 0:�����Ѿ����� 1:������������
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
    // ��ȡPID�б�
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        cout << "�ڴ治��,�˳�����" << endl;
        exit(0);
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    // ���㷵���˶��ٸ�PID
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
    if (!_tcscmp(tempProcessName, unknown)) //���û�ж�ȡ���Ƶ�Ȩ�޾ͷ���false;
    {
        data.visitable = false;
        CloseHandle(hProcess);
        return false;
    }
    else
    {
        _tcscpy_s(data.name, tempProcessName);
    }

    if (!GetProcessMemoryInfo(hProcess, &data.pmc, sizeof(data.pmc))) //���û�л�ȡ�ڴ�ʹ�õ�Ȩ�޾ͷ���false;
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

    data.time = SysTime[0]; //���̴�����ʱ��
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

bool MatchPinfo(PInfo& A, PInfo& B) //����ͬ���Ĳ�ͬPID���� ʼ����ͬ����
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

void LinkListInsert(LinkList& L, LinkList& P) //�����ڴ����Ĵ�С������뵥����
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

void DuLinkListInsert(LinkList& L, LinkList& P) //�����ڴ����Ĵ�С�������˫������
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
    while (ptr->next != NULL && ptr->data.pmc.PagefileUsage < P->data.pmc.PagefileUsage ) //PagefileUsage����ʹ�õ������ڴ�����
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
        if (GetProcessInfo(ptr->data)) //����ý��̾��л�ȡ��Ϣ��Ȩ��
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
        if (GetProcessInfo(ptr->data)) //����ý��̾��л�ȡ��Ϣ��Ȩ��
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

bool DuLinkListDelete(LinkList& L, PInfo& P) //˫�������ɾ���ڵ�
{
    LinkList ptr = L->next,pre = L;
    bool flag = false;
    while (ptr->next != NULL) //�������һ���ڵ�
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
    if (MatchPinfo(P, ptr->data)) //�����һ���ڵ�
    {
        ptr->pre->next = ptr->next;
        ptr->pre = NULL;
        
        flag = true;
    }

    return flag;
    
}

bool LinkListDelete(LinkList& L, PInfo& P) //������ɾ���ڵ�
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
    if (LinkListSearch(L, process)) //�ҵ�����0 û�ҵ�����1
        return false;
    return true;

}




int main(void)
{
    getEnumsProcessIDS();

    LinkList Initial; //��ʼ��ȫ������
    
    InitList(Initial);
    InitDuListProcesses(Initial);
    LinkList Current; //����ִ�еĽ���
    LinkList ended; //�Ѿ������Ľ���
    InitList(Current);
    InitList(ended);
    int count = 1;
    cout.setf(ios::left); //�����
    while (1)
    {
        getEnumsProcessIDS();
        
        cout <<"-----------------------------------------time = " << count <<"----------------------------------------"<< endl;
        cout << setw(40) << "������" << setw(8) << "PID" << setw(24) << "���г���ʱ��" << setw(17) << "ռ�������ڴ�" << endl;
        GetSystemTime(&systime);//��ȡ��ǰϵͳʱ��
        InitDuListProcesses(Current);
        LinkList p = Initial->next;
        LinkList temp;
        while (p != NULL)
        {
            temp = p->next;
            if (CheckEnded(Current, p->data)) //����ý����Ѿ�������ô�ʹ�current����ɾ�����ҷŵ�ended��
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
        cout << "--------------------------------------�Ѿ������Ľ���-------------------------------------" << endl;
        cout << setw(40) << "������" << setw(8) << "PID" << setw(24) << "���г���ʱ��" << setw(24) << "���̽���ʱ��" << endl;
        count++;
        EndedTraverse(ended);
        Sleep(1000);
        DestoryList(Current);
    }
    DestoryList(Initial);
    DestoryList(ended);
    return 0;
    


    
    

 
}