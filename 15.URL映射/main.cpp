//样例输入
//5 4
// articles / 2003 / special_case_2003
// articles / <int> / year_archive
// articles / <int> / <int> / month_archive
// articles / <int> / <int> / <str> / article_detail
// static / <path> static_serve
// articles / 2004/
//articles / 1985 / 09 / aloha/
//articles / hello/
//static / js / jquery.js
//样例输出
//year_archive 2004
//article_detail 1985 9 aloha
//404
//static_serve js / jquery.js
#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;

typedef struct
{
	int type; //0:/ 1:<str> 2:<int> 3:<path> 4:字符串 如"articles"
	char str[101];
}Rule;

typedef struct
{
    int type; // 0:/ 1:字符串
    char str[101];
}URL;

bool isNum(char* s)
{
    for (int i = 0; s[i] != '\0'; ++i)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}
int main(void)
{
    int n, m;
    cin >> n >> m;
    int RuleCount[101];
    string out[101];
    memset(RuleCount, 0, sizeof(int) * 101);
    Rule rules[101][51];
    char names[101][51];
    URL url[51];
    int urlCount;
    for (int i = 0; i < n; ++i)
    {
        char r[101];
        cin >> r >> names[i];
        int& cnt = RuleCount[i];
        for (int j = 0; r[j] != '\0';)
        {
            if (r[j] == '/')
            {
                rules[i][cnt].type = 0;
                cnt++;
                j++;
            }
            else if (r[j] == '<')
            {
                if (r[j + 1] == 's')
                {
                    rules[i][cnt].type = 1;
                    cnt++;
                    j += 5;
                }
                else if (r[j + 1] == 'i')
                {
                    rules[i][cnt].type = 2;
                    cnt++;
                    j += 5;

                }
                else 
                {
                    rules[i][cnt].type = 3;
                    cnt++;
                    j += 6;
                }

            }
            else 
            {
                rules[i][cnt].type = 4;
                
                int k = 0;
                for (; r[j] != '/' && r[j] != '\0'; j++)
                {
                    rules[i][cnt].str[k++] = r[j];
                }
                rules[i][cnt].str[k] = '\0';
                cnt++;
            }
        }
    }

    for (int i = 0; i < m; ++i)
    {
        char u[101];
        memset(u, 0, sizeof(char) * 101);
        cin >> u;
        urlCount = 0;
        int& cnt = urlCount;
        for (int j = 0; u[j] != '\0';)
        {
            if (u[j] == '/')
            {
                url[cnt].type = 0;
                cnt++;
                j++;
            }
            else
            {
                url[cnt].type = 1;
                
                int k = 0;
                for (; u[j] != '/' && u[j] != '\0'; j++)
                {
                    url[cnt].str[k++] = u[j];
                }
                url[cnt].str[k] = '\0';
                cnt++;
            }
        }

        bool fin_flag = false;
        for (int j = 0; j < n; ++j) //rule[j]
        {
            bool flag = true;
            int ansPath = 0;
            if (RuleCount[j] > urlCount)
                flag = false;
            if(RuleCount[j] < urlCount && rules[j][RuleCount[j]-1].type != 3)
                flag = false;

            for (int k = 0; k < RuleCount[j] && flag; ++k)
            {
                if (rules[j][k].type == 0)
                {
                    if (url[k].type != 0)
                        flag = false;
                }
                else if (rules[j][k].type == 1)
                {
                    if (url[k].type != 1)
                        flag = false;

                }
                else if (rules[j][k].type == 2)
                {
                    if (url[k].type != 1 || !isNum(url[k].str))
                        flag = false;
                }
                else if (rules[j][k].type == 3)
                {
                    ansPath = k;
                }
                else 
                {
                    if (strcmp(rules[j][k].str, url[k].str) != 0)
                    {
                        flag = false;
                    }
                }

            }
            if (flag)
            {
                fin_flag = true;
                out[i] = names[j];
                out[i] += " ";
                for (int k = 0; k < RuleCount[j]; ++k)
                {
                    if (rules[j][k].type == 1)
                    {
                        out[i] += url[k].str;
                    }
                    else if (rules[j][k].type == 2)
                    {
                        int w = 0;
                        for (; url[k].str[w] == '0'; ++w); //前导0
                        if (url[k].str[w] == '\0')
                        {
                            out[i] += "0 ";

                        }
                        else
                        {
                            out[i] += (url[k].str + w);
                            out[i] += " ";
                        }

                    }
                    else if (rules[j][k].type == 3)
                    {
                        for (int w = ansPath; w < urlCount; ++w)
                        {
                            if (url[w].type == 0)
                            {
                                out[i] += "/";

                            }
                            else if (url[w].type == 1)
                            {
                                out[i] += url[w].str;
                            }
                        }
                    }
                }

                
                break;
            }

        }
        if (!fin_flag) 
            out[i] = "404";
    }



    for (int i = 0; i < m; ++i)
    {
        cout << out[i] << endl;

    }

    return 0;
}