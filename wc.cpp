#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include <fstream>
#include<string>
#include<string.h>
#include <io.h>
#include<tchar.h>
#include<stdafx.h>
#include<winnt.h>
using namespace std;
//�����������
int count_ch(char file[]);
int count_word(char file[]);
int count_line(char file[]);
int count_noteline(char file[]);
int count_blank(char file[]);
//�ݹ�����ļ�
void search_file(string path, int idx);
//��filenameִ��ĳ������
void command(char order[], char filename[]);
char para[100][100];

int main(int argc, char* argv[])
{

	if (strcmp(argv[1], "-s") == 0)// �ݹ鴦��Ŀ¼�·����������ļ�  
	{
		//��ȡ�ļ��ľ���·��
		TCHAR szPath[MAX_PATH] = { 0 };
		if (GetModuleFileName(NULL, szPath, MAX_PATH))
		{
			(_tcsrchr(szPath, _T('\\')))[1] = 0;//����ȡ����ִ�г���ľ���·���е�ִ�г��������ȥ��
		}
		for (int i = 0; i < argc; i++)
		{
			strcpy_s(para[i], argv[i]);
		}
		search_file((string)szPath, argc - 1);
	}
	else if (strcmp(argv[1], "-x") == 0)
	{
		//���
	}
	else
	{
		cout << argc << endl;
		for (int i = 1; i < argc; i++)
		{
			command(argv[i],argv[argc-1]);
		}
	}
}

int count_ch(char file[])//��ѯ�ַ���
{
	FILE* fp1;
	char ch;
	int ch_num = 0;
	if (fopen_s(&fp1, file, "r") != 0)//ʧ��
	{
		return -1;
	}
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

void search_file(string path, int idx)
{
	struct _finddata_t filefind;
	cout << path << endl;
	string cur = path + "*.*";//����ȫ���ļ�
	int done = 0;
	int handle;
	if ((handle = _findfirst(cur.c_str(), &filefind)) != -1)
	{
		while (!(done = _findnext(handle, &filefind))) 
		{
			if (strcmp(filefind.name, "..") == 0)
			{
				continue;
			}
			if ((_A_SUBDIR == filefind.attrib))//�жϵ�ǰ�ļ��Ƿ���һ���ļ���
			{ 
				cur = path + filefind.name + '\\';//�����ļ���
				search_file(cur, idx);    //�ݹ鴦��
			}
			else //���ҵ�ǰ�ļ��������еķ��Ͻ�β���ļ�
			{
				int len = strlen(filefind.name);
				for (int i = 0; i < len; i++) 
				{
					if (filefind.name[i] == '.') 
					{
						len = i;
						break;
					}
				}
				if (strcmp(filefind.name + len, para[idx] + 1) == 0) //�ļ�βһ���Ļ����Ƿ����ˣ����綼��.cpp��β
				{
					cur = path + filefind.name;
					printf("%s:\n", filefind.name);
					for (int i = 1; i < idx; i++)
					{
						command(para[i], &cur[0]);
					}
				}
			}
		}
		_findclose(handle);
	}
}

void command(char order[], char filename[])
{
	if (order[1] == 'c')
	{
		printf("�ַ���:%d\n", count_ch(filename));
	}
	else if (order[1] == 'w')
	{
		printf("������:%d\n", count_word(filename));
	}
	else if (order[1] == 'l')
	{
		printf("����:%d\n", count_line(filename));
	}
	else if (order[1] == 'a')
	{
		int note = count_noteline(filename);
		int blank = count_blank(filename);
		int line = count_line(filename);
		int codeline = line - blank - note;
		printf("��������:%d\n", codeline);
		printf("������:%d\n", blank);
		printf("ע������:%d\n", note);
	}
	cout << endl;
}
