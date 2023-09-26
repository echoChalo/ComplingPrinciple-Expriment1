#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_Experiment1.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <set>
#include <Windows.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextcodec.h>

using namespace std;

enum Token {
	KEYWORD,// 关键字
	VAR, // 标识符
	NUMBER, CH, STRING, // 常量
	ARITHMETIC, RELATION, LOGIC, BIT, ASSIGNMENT, OTHERS, //运算符
	SPECIAL, // 其它符号，如分界符
	ANNOTATION, // 注释
	ENDINPUT // 结束符号
};

struct TokenStruct
{
	Token ID;
	string num; // 数字常量
	string ch;  // 字符常量
	string str; // 字符串常量
	string op; // 运算符
	string word; // 标识符
	string key; // 关键字
	string special; // 其他符号
	string annotation; // 注释
};

class Experiment1 : public QMainWindow
{
    Q_OBJECT

public:
    Experiment1(QWidget *parent = nullptr);
    ~Experiment1();
    void GetToken(); // 处理单词
	bool Multi_row(); // 多行注释处理
	void init(); // 将主要关键字存入集合中
	void init_token(); // 初始化token内单词
	void Write_result(); // 将结果写入文件中
	

private slots:
	void Write_test(); // 将需要分析的源文件打开
	void Show_Result(); // 将结果打印出ui界面上
	void main_process(); // 处理单词的主要进程
	void Exit(); // 结束程序

private:
    Ui::Experiment1Class ui;
	set<string> keyword; // 存放关键字的集合
	string buffer; // 单词
	string to_write; // 处理的文件名
	TokenStruct *T;// 存放单词的结构体数组
	TokenStruct token;// 单词结构
	int pos; // 单词字符串中的字符下标
	int lenth; // 存放单词个数
	int now_line; // 存放代码行数
	bool is_an; // 判断注释
	bool multi_row; // 判断多行注释
};
