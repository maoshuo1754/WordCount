#include <sstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDir"
#include "QFileDialog"
using namespace std;

//
//五个基本功能
int count_ch(char file[]);
int count_word(char file[]);
int count_line(char file[]);
int count_noteline(char file[]);
int count_blank(char file[]);

string getstr(char filename[]){
        string result="";
        result+="\n";
        ostringstream oss;
        oss<<"  字符数="<<count_ch(filename)<<endl;
        oss<<"  单词数="<<count_word(filename)<<endl;
        oss<<"  行数="<<count_line(filename)<<endl;
        oss<<"  空行数="<<count_blank(filename)<<endl;
        oss<<"  注释行数="<<count_noteline(filename)<<endl;
        oss<<"  代码行数="<<(count_line(filename)-count_blank(filename)-count_noteline(filename))<<endl;
        result+=oss.str();
        return result;
}

int count_ch(char file[])//查询字符数
{
    FILE* fp1;
    char ch;
    int ch_num = 0;
    if (fopen_s(&fp1, file, "r") != 0)//失败
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


//

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//浏览按钮的实现代码
void MainWindow::on_toolButton_clicked()
{
    ui->textBrowser_2->setText("");
    ui->textBrowser->setText("");
    ui->comboBox->clear();
    QString path = QDir :: toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("view file"),QDir::currentPath()));
    ui->textBrowser_2->textCursor().insertText(path);
    QDir dir(path);

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList list=dir.entryInfoList();

    for(int i=0;i<list.length();i++){
        QString file=list.at(i).fileName();
        ui->comboBox->addItem(file);
    }

}

void MainWindow::on_pushButton_clicked()
{
   if(ui->textBrowser_2->document()->toPlainText()==NULL)
         ui->textBrowser->setText("请选择文件！");
   else{
        ui->textBrowser->setText("");
        ui->textBrowser->textCursor().insertText(QString::fromStdString("文件路径："));
        ui->textBrowser->textCursor().insertText(ui->textBrowser_2->document()->toPlainText());
        ui->textBrowser->textCursor().insertText(QString::fromStdString("\n文件:"));
        ui->textBrowser->textCursor().insertText(ui->comboBox->currentText());
        ui->textBrowser->textCursor().insertText(QString::fromStdString("\n"));
        string result="";
        string name=ui->textBrowser_2->document()->toPlainText().toStdString();
        name+="\\";
        name+=ui->comboBox->currentText().toStdString();
        char *buf=new char[name.length()];
        strcpy(buf,name.c_str());
        result=getstr(buf);
        ui->textBrowser->textCursor().insertText(QString::fromStdString(result));
       }
}

