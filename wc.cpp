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
	char filename[] = "C:\\Users\\Joo\\Desktop\\�����Ŀ\\in.txt";
	cout << "�ַ�����"<< count_ch(filename) << endl;
	cout << "��������" << count_word(filename) << endl;
	cout << "������" << count_line(filename) << endl;
	cout << "�����У�" << count_line(filename) - count_noteline(filename) - count_blank(filename) << endl;
	cout << "ע��������" << count_noteline(filename) << endl;
	cout << "��������" << count_blank(filename) << endl;

}

int count_ch(char file[])//��ѯ�ַ���
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
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

int count_word(char file[])//��ѯ������,����ĸ֮ǰ��֮�����Ϊ��ĸΪһ������
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
	{
		return -1;
	}
	int word_num = 0;
	char ch;
	int flag = 0;//flag==0����ǰһ��Ϊ���ʣ�����Ϊ����
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

int count_line(char file[])//��ѯ����
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
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

int count_noteline(char file[])//��ע����
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
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
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
		if (thisline == 1)//˵�������Ѿ���ע�ͣ�������һ��
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

int count_blank(char file[])//��ѯ������
{
	FILE* fp1;
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
	{
		return -1;
	}
	int blank_row_num = 0;
	int row_char_num = 0;//�����ַ���
	char ch;
	while ((ch = fgetc(fp1)) != EOF)
	{
		if (ch == '\n')
		{
			if (row_char_num <= 1)//����ʾ�ַ�С�ڵ���1Ϊ����
			{
				blank_row_num++;
			}
			row_char_num = 0;
		}
		else
		{
			if (ch != ' '&&ch != '\t')//����ʾ�ַ�
			{
				row_char_num++;
			}
		}
	}
	if (row_char_num <= 1)//�������һ��
	{
		blank_row_num++;
	}

	return blank_row_num;
}