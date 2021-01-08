#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<string.h>
#include<string>
#include<iomanip>
#include<ctype.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT	10
#define MAX_LEN 64

using namespace std;

typedef struct {
    double* base;
    double* top;
    int stacksize;
} DoubleSqStack;

typedef struct {
    char* base;
    char* top;
    int stacksize;
} CharSqStack;

void InitDoubleStack(DoubleSqStack& s)
{
    s.base = (double*)malloc(STACK_INIT_SIZE * sizeof(double));
    s.top = s.base;
    s.stacksize = STACK_INIT_SIZE;
    return;
}
void DestroyDoubleStack(DoubleSqStack& s)
{
    free(s.base);
    s.base = NULL;
    s.top = NULL;
    return;
}

void ClearDoubleStack(DoubleSqStack& s)
{
    s.top = s.base;
    s.stacksize = 0;
}
bool DoubleStackEmpty(DoubleSqStack s)
{
    if (s.top == s.base)
        return true;
    return false;
}
int DoubleStackLength(DoubleSqStack s)
{
    return s.top - s.base;
}
double GetTopDouble(DoubleSqStack s)
{
    if (s.top == s.base)
    {
        cout << "error";
        exit(0);
    }
    return *(s.top - 1);
}
void PushDouble(DoubleSqStack& s, double e)
{
    if (s.top - s.base == s.stacksize)
    {
        s.base = (double*)realloc(s.base, (s.stacksize + STACKINCREMENT) * sizeof(double));
        if (s.base == NULL)
        {
            exit(0);
        }
        s.top = s.base + s.stacksize;
        s.stacksize += STACKINCREMENT;
    }
    *s.top = e;
    s.top++;
    return;
}

bool PopDouble(DoubleSqStack& s, double& e)
{
    if (s.top == s.base)
        return false;
    --s.top;
    e = *s.top;
    return true;
}
void DoubleStackTraverse(DoubleSqStack s)
{
    cout << ' '; 
    double* p;
    int i = 0;
    for (p = s.base; p != s.top; p++)
    {
        cout << " " << *p;
        i++;
    }
        
    cout << setw(12-i) << " ";
    //cout << '\t';
    //cout << setw(16) ;
    //cout << endl;
}

void InitCharStack(CharSqStack& s)
{
    s.base = (char*)malloc(STACK_INIT_SIZE * sizeof(char));
    s.top = s.base;
    s.stacksize = STACK_INIT_SIZE;
}
void DestroyCharStack(CharSqStack& s)
{
    free(s.base);
    s.base = NULL;
    s.top = NULL;
}

void ClearCharStack(CharSqStack& s)
{
    s.top = s.base;
    s.stacksize = 0;
}
bool CharStackEmpty(CharSqStack s)
{
    if (s.top == s.base)
        return true;
    return false;
}
int CharStackLength(CharSqStack s)
{
    return s.top - s.base;
}
char GetTopChar(CharSqStack s)
{
    if (s.top == s.base)
    {
        cout << "error";
        exit(0);
    }
    return *(s.top - 1);
}
void PushChar(CharSqStack& s, char e)
{
    if (s.top - s.base == s.stacksize)
    {
        s.base = (char*)realloc(s.base, (s.stacksize + STACKINCREMENT) * sizeof(char));
        if (s.base == NULL)
        {
            exit(0);
        }
        s.top = s.base + s.stacksize;
        s.stacksize += STACKINCREMENT;
    }
    *s.top = e;
    s.top++;
    return;
}

bool PopChar(CharSqStack& s, char& e)
{
    if (s.top == s.base)
        return false;
    --s.top;
    e = *s.top;
    return true;
}

void CharStackTraverse(CharSqStack s)
{
    string str = "";
    for (char* p = s.base; p != s.top; p++)
        str += (*p);
    cout << setw(18) << str;
    //cout << endl;
}

bool isOPTR(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#')
        return true;
    else
        return false;
}

bool isCalc(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return true;
    return false;
}

char Precede(char a, char b)
{
    char pre[7][8] =
    {">><<<>>",
     ">><<<>>",
     ">>>><>>",
     ">>>><>>",
     "<<<<<= ",
     ">>>> >>",
     "<<<<< =" };
    char ch[7] = { '+','-','*','/','(',')','#' };
    unsigned int ch1, ch2;
    for (int i = 0; i < 7; ++i)
    {
        if (ch[i] == a)
        {
            ch1 = i;
        }
        if (ch[i] == b)
        {
            ch2 = i;
        }
    }
    return pre[ch1][ch2];
    

}

double Operate(double& a, char& theta, double& b)
{
    if (theta == '+')
        return a + b;
    else if (theta == '-')
        return a - b;
    else if (theta == '*')
        return a * b;
    else if (theta == '/')
    {
        if (b == 0)
        {
            cout << "����:����Ϊ0" << endl;
            return 0;
        }
        else
        {
            return a / b;
        }
    }

}
void PrintInput(char* s,int len)
{
    string str;
  
    str = "[";
    str += s[0];
    str += "]";
    for (int i = 1; i < len; ++i)
    {
        str += s[i];
    }
    cout.unsetf(ios::left);
    cout << setw(18) << str;
    return;
}

bool CheckExpression(string& exp) //�����ʽ�Ƿ���ȷ
{
    int left = 0;
    int right = 0;
    if (exp[0] != '#' || exp[exp.length() - 1] != '#')
    {        
        cout << "����:��β��Ϊ���޷�#" << endl;
        return false;
    }
    else if (isCalc(exp[1]))
    {
        cout << "����:��������ܷ�����λ" << endl;        
        return false;
    }
    else if (isCalc(exp[exp.length() - 2]))
    {        
        cout << "����:��������ܷ���ĩλ" << endl;        
        return false;
    }
    else
    {
        for (int i = 1; i <= exp.length() - 2; i++)
        {
            if (exp[i] == '#')
            {                
                cout << "����:���ʽ�ڲ�����#" << endl;                
                return false;
            }
            if (exp[i] == '/' && exp[i+1] == '0')
            {
                cout << "����:����Ϊ0" <<endl;
                return false;
            }
            if (isCalc(exp[i]) && (isCalc(exp[i - 1]) || isCalc(exp[i - 1])))
            {                
                cout << "����:�����������������" << endl;                
                return false;
            }
            if (!isOPTR(exp[i]) && exp[i] != '.' && !isdigit(exp[i]))
            {                
                cout << "����:���ַǷ��ַ�" << endl;                
                return false;
            }

            if (exp[i] == '(')
            {
                left++;
                if (!isOPTR(exp[i - 1]) )
                {                    
                    cout << "����:'('ǰ��������" << endl;                    
                    return false;
                }
                if (isCalc(exp[i + 1]))
                {
                    
                    cout << "����:'('��ֱ�ӳ��������" << endl;                    
                    return false;
                }
                else if (exp[i + 1] == ')')
                {
                    
                    cout << "����:'( )'��Ϊ��" << endl;                    
                    return false;
                }

            }
            else if (exp[i] == ')')
            {
                right++;
                if (isCalc(exp[i - 1]))
                {
                    
                    cout << "����:')'ǰֱ�ӳ��������" << endl;                    
                    return false;
                }
                if (!isOPTR(exp[i + 1]))
                {                    
                    cout << "����:')'���������" << endl;
                    return false;
                }
            }
            if (exp[i] == '.' && (isOPTR(exp[i-1]) ||isOPTR(exp[i+1])) )
            {                
                cout << "����:С����ǰ����ַ���" << endl;                
                return false;
            }
        }
        if (left != right)
        {            
            cout << "����:�������Ų�ƥ��" << endl;            
            return false;
        }
    }
    return true;
}

bool EvaluateExpression(string & exp)
{
    cout.setf(ios::left);
    cout << setw(10) << "����" << setw(20) << "OPTRջ" << setw(20) << "OPNDջ" << setw(20) << "�����ַ�" << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    DoubleSqStack OPND;
    CharSqStack OPTR;
    InitDoubleStack(OPND);
    InitCharStack(OPTR);

    char theta; //�����
    double a;
    double b;
    PushChar(OPTR, '#');
    int i = 1;
    int step = 1; //����
    while ((exp[i] != '#' || GetTopChar(OPTR) != '#') && i <= exp.length() -1)
    {
        cout << setw(10) << step;
        step++;
        
        CharStackTraverse(OPTR);
        DoubleStackTraverse(OPND);
        PrintInput(&exp[i],exp.length() - i);
        cout.setf(ios::left);
        cout << endl;
        if (!isOPTR(exp[i])) //������������OPNDջ
        {
            int end = i;
            double num;
            char* temp;
            while (!isOPTR(exp[end]))
                end++;
            num = strtod(&exp[i], &temp);
            PushDouble(OPND, num);
            i = end;
        }
        else
        {
            switch (Precede(GetTopChar(OPTR), exp[i]))
            {
                case '<':
                {
                    PushChar(OPTR, exp[i]);
                    i++;
                    break;
                }
                case '=':
                {
                    PopChar(OPTR, theta);
                    i++;
                    break;
                }
                case '>':
                {
                    double result;
                    PopChar(OPTR, theta);
                    PopDouble(OPND, b);
                    PopDouble(OPND, a);
                    result = Operate(a, theta, b);
                    PushDouble(OPND,result);
                    break;

                }
            }
        }
    }
    double result = GetTopDouble(OPND);
    cout << "������:" << result << endl;
    return true;

}

int main(void)
{
    string Expression;
    cout << "������ʽ:";
    cin >> Expression;
    while (1)
    {
        if (!CheckExpression(Expression))
        {
            cout << "����������ʽ :";
            cin >> Expression;
        }
        else
        {
            break;
        }
    }
    EvaluateExpression(Expression);
    return 0;
}
