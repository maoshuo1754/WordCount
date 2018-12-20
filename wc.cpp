#include<iostream>
#include<cstdio>
#include<stdlib.h>
//#include "io.h"
using namespace std;


int count_ch(char file[]);
int count_word(char file[]);
int count_line(char file[]);
int count_noteline(char file[]);
int count_blank(char file[]);

int main()
{
	char filename[] = "C:\\Users\\Joo\\Desktop\\结对项目\\in.txt";
	cout << "字符数："<< count_ch(filename) << endl;
	cout << "单词数：" << count_word(filename) << endl;
	cout << "行数：" << count_line(filename) << endl;
	cout << "代码行：" << count_line(filename) - count_noteline(filename) - count_blank(filename) << endl;
	cout << "注释行数：" << count_noteline(filename) << endl;
	cout << "空行数：" << count_blank(filename) << endl;

}

int count_ch(char file[])//查询字符数
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//失败
	{
		return -1;
	}
	char ch;
	int ch_num = 0;
	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch != ' '&&ch != '\t'&&ch != '\n')
		{
			ch_num++;
		}
	}
	return ch_num;
}

int count_word(char file[])//查询单词数,以字母之前和之后均不为字母为一个单词
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//失败
	{
		return -1;
	}
	int word_num = 0;
	char ch;
	int flag = 0;//flag==0代表前一个为单词，否则不为单词
	while ((ch = fgetc(fp1)) != EOF)
	{
		if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		{
			if (flag == 0)
			{
				word_num++;
				flag = 1;
			}
		}
		else
		{
			flag = 0;
		}
	}
	return word_num;
}

int count_line(char file[])//查询行数
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//失败
	{
		return -1;
	}
	int row_num = 1;
	char ch;
	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch == '\n')
		{
			row_num++;
		}
	}
	return row_num;
}

int count_noteline(char file[])//查注释行
{
	int n_num = 0;
	int line = 0;
	int ch_num = 0;
	int flag_1 = 0;
	int flag_2 = 0;
	int flag_3 = 0;
	int thisline = 0;
	char ch;
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//失败
	{
		return -1;
	}
	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch == '\n')
		{
			if (line && ch_num > 0)
			{
				line++;
			}
			ch_num = 0;
			flag_1 = thisline = 0;
		}
		if (thisline == 1)//说明本行已经是注释，跳入下一行
		{
			continue;
		}
		if (ch != ' '&&ch != '\t'&&ch != '\n')
		{
			ch_num++;
		}
		if (flag_2)
		{
			if (ch != ' '&&ch != '\t'&&ch != '\n')
			{
				ch_num++;
			}
			if (ch == '*')
			{
				flag_3 = 1;
			}
			else if (ch == '/'&&flag_3)
			{
				n_num += line; line = 0;
				flag_2 = flag_3 = 0;
				thisline = 1;
			}
			else flag_3 = 0;
		}
		else if (ch == '/')
		{
			if (flag_1 == 0) flag_1 = 1;
			else if (flag_1 == 1 && ch_num <= 3)
			{
				n_num++; 
				thisline = 1;
			}
		}
		else if (ch == '*')
		{
			if (flag_1 == 1)
			{
				flag_2 = 1;
				line = 1;
			}
		}
		else
		{
			flag_1 = 0;
		}
	}
	fclose(stdin);
	return n_num;
}

int count_blank(char file[])//查询空行数
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//失败
	{
		return -1;
	}
	int blank_row_num = 0;
	int row_char_num = 0;//本行字符数
	char ch;
	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch == '\n')
		{
			if (row_char_num <= 1)//可显示字符小于等于1为空行
			{
				blank_row_num++;
			}
			row_char_num = 0;
		}
		else
		{
			if (ch != ' '&&ch != '\t')//可显示字符
			{
				row_char_num++;
			}
		}
	}
	if (row_char_num <= 1)//处理最后一行
	{
		blank_row_num++;
	}

	return blank_row_num;
}