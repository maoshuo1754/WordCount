#include <io.h>
#include <iostream>
#include <fstream>
using namespace std;

bool transfer(string fileName, int exeNum );
void dfsFolder(string folderPath, ofstream &fout);

int main()
{
    _finddata_t file;
    int k;
    long HANDLE;
    k = HANDLE = _findfirst("*.*", &file);
    while (k != -1)
    {
        cout << file.name << endl;
        k = _findnext(HANDLE, &file);
    }
    _findclose(HANDLE);

    transfer("C:\\Windows\\*.exe", 0);
    ofstream o_fstream;

    dfsFolder("E:\\\WHU\\Study", o_fstream);


    return 0;
}

//_findfirst �������ص���ƥ�䵽�ļ��ľ������������Ϊlong��
//�������̿���ָ���ļ����ͣ���ͨ��FileName�ĸ�ֵ��ʵ�֣�����Ҫ����C : \WINDOWS�µ�����.exe�ļ�

bool transfer(string fileName , int exeNum)
{
    _finddata_t fileInfo;
    long handle = _findfirst(fileName.c_str(), &fileInfo);

    if (handle == -1L)
    {
        cerr << "failed to transfer files" << endl;
        return false;
    }

    do
    {
        exeNum++;
        cout << fileInfo.name << endl;
    } while (_findnext(handle, &fileInfo) == 0);
    cout << " .exe files' number:  " << exeNum << endl;

    return true;
}

//�����ļ��м������ļ����������ļ�������ϵͳ���ļ���Ŀ¼����״�ṹ��ʹ������������Ա��������ļ����õ�_A_SUBDIR����


//���ж�������Ŀ¼��if��֧�У�����ϵͳ�ڽ���һ����Ŀ¼ʱ��ƥ�䵽��ͷ�����ļ�(��)��"."(��ǰĿ¼)��".."(��һ��Ŀ¼)��
//��Ҫ���Ե����������������Ҫ�Ա��������ļ�������ʱ����else��֧�������Ӧ�Ĵ���ͺ�

void dfsFolder(string folderPath, ofstream &fout)
{
    _finddata_t FileInfo;
    string strfind = folderPath + "\\*";
    long Handle = _findfirst(strfind.c_str(), &FileInfo);

    if (Handle == -1L)
    {
        cerr << "can not match the folder path" << endl;
        exit(-1);
    }
    do{
        //�ж��Ƿ�����Ŀ¼  
        if (FileInfo.attrib & _A_SUBDIR)
        {
            //���������Ҫ  
            if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
            {
                string newPath = folderPath + "\\" + FileInfo.name;
                dfsFolder(newPath, fout);
            }
        }
        else
        {
            fout<<folderPath.c_str() << "\\" << FileInfo.name << " ";
            cout << folderPath.c_str() << "\\" << FileInfo.name << endl;
        }
    } while (_findnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
    fout.close();
}

