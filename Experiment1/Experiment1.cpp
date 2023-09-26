#include "Experiment1.h"


Experiment1::Experiment1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	setWindowIcon(QIcon("./picture.ico"));
	pos = 0; lenth = 0; now_line = 0;
	is_an = false; multi_row = false;
	to_write = "";
	T = new TokenStruct[255];
}

Experiment1::~Experiment1()
{
	delete[]T;
}

void Experiment1::GetToken()
{
	if (multi_row)return;
	// 空格或者Tab跳过
	while (buffer[pos] == 32 || buffer[pos] == 9 || buffer[pos] < 0)pos++;
	// 头文件判断
	if (lenth > 0 && T[lenth - 1].key == "include") {
		if (buffer[pos] == '<') {
			while (buffer[pos] != '>') {
				token.special += buffer[pos];
				pos++;
			}
			token.special += buffer[pos]; pos++;
			token.ID = SPECIAL;
		}
		else if (buffer[pos] == '\"') {
			token.special += buffer[pos]; pos++;
			while (buffer[pos] != '\"') {
				token.special += buffer[pos];
				pos++;
			}
			token.special += buffer[pos]; pos++;
			token.ID = SPECIAL;
		}
		else {
			cout << now_line << " 行 " << pos + 1 << "处代码错误！" << endl;
			exit(1);
		}
	}
	// 数字常量 NUMBER
	else if (((buffer[pos] >= '0') && (buffer[pos] <= '9')) || ((buffer[pos] == '-' || buffer[pos] == '+') && isdigit(buffer[pos + 1]))) {
		bool is_16 = false, is_8 = false, is_sign = false;
		token.num = ""; token.ID = NUMBER;
		if (buffer[pos] == '-' || buffer[pos] == '+') {
			token.num += buffer[pos]; pos++;
		}
		if (buffer[pos] == '0') {
			if (buffer[pos + 1] == 'x' || buffer[pos + 1] == 'X')// 16进制处理
			{
				token.num += buffer[pos]; token.num += buffer[pos + 1]; pos += 2;
				is_16 = true;
			}
			else if ((buffer[pos + 1] >= '0') && (buffer[pos + 1] <= '7')) // 8进制处理
			{
				token.num += buffer[pos]; pos++;
				is_8 = true;
			}
		}
		while ((buffer[pos] >= '0' && buffer[pos] <= '9') || (buffer[pos] >= 'a' && buffer[pos] <= 'f') || (buffer[pos] >= 'A' && buffer[pos] <= 'Z'))
		{
			if (is_16 && (buffer[pos] < 'A' || buffer[pos] > 'F') && (buffer[pos] < 'a' || buffer[pos] > 'f') && (buffer[pos] < '0' || buffer[pos] > '9')) {
				cout << now_line << " 行 " << pos + 1 << "处数字常量错误！" << endl; exit(1);
			}
			else if (is_8 && (buffer[pos] > '7' || buffer[pos] < '0')) {
				cout << now_line << " 行 " << pos + 1 << "处数字常量错误！" << endl; exit(1);
			}
			else if (buffer[pos] == 'u' || buffer[pos] == 'l')is_sign = true;
			else if (buffer[pos]) {
				token.num += buffer[pos];
				pos++;
			}
			else {
				cout << now_line << " 行 " << pos + 1 << " 处数字常量错误！" << endl; exit(1);
			}
		}
		if (is_sign) {
			if (token.num[token.num.length() - 1] != 'u' && token.num[token.num.length() - 1] != 'l') {
				cout << now_line << " 行 " << pos << "处数字常量错误！" << endl; exit(1);
			}
		}
		// 浮点数
		if (buffer[pos] == '.') {
			token.num += '.'; pos++;
			while ((buffer[pos] >= '0') && (buffer[pos] <= '9')) {
				token.num += buffer[pos];
				pos++;
			}
		}
		// 指数
		if (buffer[pos] == 'e' || buffer[pos] == 'E') {
			token.num += buffer[pos]; token.num += buffer[pos + 1]; pos += 2;
			while (isdigit(buffer[pos])) {
				token.num += buffer[pos];
				pos++;
			}
		}
	}
	// 字符常量
	else if (buffer[pos] == '\'') {
		token.ch += '\''; pos++;
		if (buffer[pos] == '\\') {
			token.ch += '\\'; pos++;
			token.ch += buffer[pos]; pos++;
		}
		else {
			token.ch += buffer[pos];
			pos++;
		}
		token.ID = CH; token.ch += '\'';
		pos++;
	}
	// 字符串常量
	else if (buffer[pos] == '"') {
		token.str += '"'; pos++;
		while (buffer[pos] != '"') {
			token.str += buffer[pos];
			if (buffer[pos] == '\\') {
				token.str += buffer[pos + 1];
				pos++;
			}
			pos++;

		}
		token.ID = STRING; token.str += '"';
		pos++;
	}
	// 单行注释
	else if (buffer[pos] == '/' && buffer[pos + 1] == '/') {
		token.ID = ANNOTATION; token.annotation = buffer.substr(pos);
		is_an = true;
		return;
	}
	// 多行注释
	else if (buffer[pos] == '/' && buffer[pos + 1] == '*') {
		token.ID = ANNOTATION;
		if (buffer.find("*/") == string::npos)multi_row = true;
		return;
	}
	// 标识符与关键字
	else if (isalpha(buffer[pos]) || buffer[pos] == '_') {
		token.word = "";
		while (isalpha(buffer[pos]) || isdigit(buffer[pos]) || buffer[pos] == '_')
		{
			token.word += buffer[pos]; pos++;
		}
		if (keyword.count(token.word)) {
			token.key = token.word;
			token.ID = KEYWORD;
		}
		else token.ID = VAR;
	}
	// 其它词义符号
	else if (buffer[pos] != '\0') {
		switch (buffer[pos])
		{
		case '+': {token.op += '+';
			if (buffer[pos + 1] == '+')
			{
				token.ID = ARITHMETIC; token.op += "+";
				pos++; break;
			}
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += "=";
				pos++; break;
			}
			token.ID = ARITHMETIC; break; }
		case '-': {token.op += '-';
			if (buffer[pos + 1] == '-')
			{
				token.ID = ARITHMETIC; token.op += '-';
				pos++; break;
			}
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			if (buffer[pos + 1] == '>')
			{
				token.ID = OTHERS; token.op += '>';
				pos++; break;
			}
			token.ID = ARITHMETIC; break; }
		case '*': {token.op += '*';
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			if (isalpha(buffer[pos + 1]) && T[lenth - 1].ID != VAR)
			{
				token.ID = OTHERS;
				break;
			}
			token.ID = ARITHMETIC; break; }
		case '/': {token.op += '/';
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			token.ID = ARITHMETIC; break; }
		case '%': {token.op += '%';
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			token.ID = ARITHMETIC; break; }
		case '>': {token.op += '>';
			if (buffer[pos + 1] == '=')
			{
				pos++; token.ID = RELATION; token.op += '=';
				break;
			}
			if (buffer[pos + 1] == '>') {
				pos++; token.ID = BIT; token.op += '>';
				if (buffer[pos + 1] == '=')
				{
					token.ID = ASSIGNMENT; token.op += '=';
					pos++; break;
				}
				break;
			}
			token.ID = RELATION;
			break; }
		case '<': {token.op += '<';
			if (buffer[pos + 1] == '=')
			{
				pos++; token.ID = RELATION; token.op += '=';
				break;
			}
			if (buffer[pos + 1] == '<') {
				pos++; token.ID = BIT; token.op += '<';
				if (buffer[pos + 1] == '=')
				{
					token.ID = ASSIGNMENT; token.op += "=";
					pos++; break;
				}
				break;
			}
			token.ID = RELATION;
			break; }
		case '(': {
			token.ID = SPECIAL; token.special += '('; break;
		}
		case ')': {
			token.ID = SPECIAL; token.special += ')'; break;
		}
		case '[': {
			token.ID = SPECIAL; token.special += '['; break;
		}
		case ']': {
			token.ID = SPECIAL; token.special += ']'; break;
		}
		case '=': {token.op += '=';
			if (buffer[pos + 1] == '=')
			{
				pos++; token.ID = RELATION; token.op += '='; break;
			}
			token.ID = ASSIGNMENT;
			break; }
		case '!': {token.op += '!';
			if (buffer[pos + 1] == '=')
			{
				pos++; token.ID = RELATION; token.op += '='; break;
			}
			token.ID = LOGIC;
			break; }
		case '&': {token.op += '&';
			if (buffer[pos + 1] == '&') {
				pos++; token.ID = LOGIC; token.op += '&'; break;
			}
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			if (isalpha(buffer[pos]))
			{
				token.ID = OTHERS; token.op += '\0';
				pos++; break;
			}
			token.ID = BIT;
			break; }
		case '|': {token.op += '|';
			if (buffer[pos + 1] == '|') {
				pos++; token.ID = LOGIC; token.op += '|'; break;
			}
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			token.ID = BIT;
			break; }
		case '^': {token.op += '^';
			if (buffer[pos + 1] == '=')
			{
				token.ID = ASSIGNMENT; token.op += '=';
				pos++; break;
			}
			token.ID = BIT;
			break; }
		case '~': {token.op += '~';
			token.ID = BIT;
			break; }
		case '.': {
			if (isalpha(buffer[pos + 1]))
			{
				token.ID = OTHERS; token.op += '.';
				break;
			}
		}
		case '?': {
			int now_pos = pos + 1, t = 0;
			while (buffer[now_pos] == 32 || buffer[now_pos] == 9) now_pos++;
			if (isalpha(buffer[now_pos]) || isdigit(buffer[now_pos])) {
				now_pos = now_pos + 1;
				while (buffer[now_pos] != ':' && (buffer[now_pos] == 32 || buffer[now_pos] == 9))
				{
					now_pos++;
				}
				if (buffer[now_pos] == ':') {
					token.op += '?'; token.op += ':';
					token.ID = OTHERS; buffer[now_pos] = ' ';
					break;
				}
				else {
					cout << now_line << " 行 " << pos + 1 << " 处代码错误！" << endl; exit(1);
				}
			}
		}
		case '#': {
			token.ID = SPECIAL; token.special += '#'; break;
		}
		case '{': {
			token.ID = SPECIAL; token.special += '{'; break;
		}
		case '}': {
			token.ID = SPECIAL; token.special += '}'; break;
		}
		case ';': {
			token.ID = SPECIAL; token.special += ';'; break;
		}
		case ',': {
			token.ID = SPECIAL; token.special += ','; break;
		}
		case ':': {
			token.ID = SPECIAL; token.special += ':'; break;
		}
		default: {cout << now_line << " 行 " << pos + 1 << " 处代码错误！" << endl; exit(1); }
		}
		pos++;
	}
	else if (buffer[pos] < 0) {
		cout << now_line << " 行 " << pos + 1 << " 处代码错误！" << endl; exit(1);
	}
	else token.ID = ENDINPUT;
}

bool Experiment1::Multi_row()
{
	if (!multi_row)return false;
	token.annotation += ' ';
	if (buffer.find("*/") != string::npos) {
		multi_row = false;
		for (int i = 0; i < buffer.length(); i++) {
			if (buffer[i] == '*' && buffer[i + 1] == '/') {
				pos = i + 2;
				token.ID = ANNOTATION;
				token.annotation += buffer.substr(0, i + 2);
				return false;
			}
		}
	}
	else token.annotation += buffer.substr(pos);
	return true;
}

void Experiment1::init()
{
	ifstream file("Keyword.csv");
	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string word;
		while (getline(ss, word, ',')) {
			keyword.insert(word);
		}
	}
	ofstream init_result("result.txt", ios::out);
	init_result.close();
	pos = 0; lenth = 0; now_line = 0;
	is_an = false; multi_row = false;
}

void Experiment1::init_token()
{
	token.annotation = "";
	token.key = "";
	token.num = "";
	token.op = "";
	token.special = "";
	token.str = "";
	token.word = "";
	token.ch = "";
}

void Experiment1::main_process()
{
	init();
	
	fstream file(to_write);
	if (!file.is_open()) {
		QMessageBox::warning(NULL, QString("错误"), QString("文件读入错误！请输入根目录下的文件或在预览中选择文件！"));
		return;
	}
	while (getline(file, buffer)) {
		now_line++;
		token.ID = KEYWORD;
		while (token.ID != ENDINPUT) {
			if (is_an)break;
			GetToken();
			if (Multi_row()) break;
			T[lenth] = token;
			lenth++;
			init_token();
		}
		pos = 0; 
		is_an = false;
		Write_result();
		lenth = 0;
	}
}

void Experiment1::Write_result()
{
	ofstream out;
	out.open("result.txt", ios::app);
	for (int i = 0; i < lenth; i++) {
		if (T[i].ID == NUMBER)out << "数字\t\t" << T[i].num << endl;
		if (T[i].ID == STRING)out << "字符串  \t\t" << T[i].str << endl;
		if (T[i].ID == VAR)out << "标识符  \t\t" << T[i].word << endl;
		if (T[i].ID == ARITHMETIC)out << "算术运算符\t" << T[i].op << endl;
		if (T[i].ID == RELATION)out << "关系运算符\t" << T[i].op << endl;
		if (T[i].ID == LOGIC)out << "逻辑运算符\t" << T[i].op << endl;
		if (T[i].ID == BIT)out << "位运算符 \t" << T[i].op << endl;
		if (T[i].ID == ASSIGNMENT)out << "赋值运算符\t" << T[i].op << endl;
		if (T[i].ID == OTHERS)out << "杂项运算符\t" << T[i].op << endl;
		if (T[i].ID == KEYWORD)out << "关键字  \t\t" << T[i].key << endl;
		if (T[i].ID == SPECIAL)out << "特殊字符 \t" << T[i].special << endl;
		if (T[i].ID == ANNOTATION)out << "注释\t\t" << T[i].annotation << endl;
		if (T[i].ID == CH)out << "字符常量\t\t" << T[i].ch << endl;
	}
	out.close();
}

void Experiment1::Write_test() 
{
	auto File_Path = QFileDialog::getOpenFileName(this, tr("预览"), QDir::currentPath(), tr("C++源文件(*.cpp);;所有类型(*.*)"));
	to_write = string(File_Path.toLocal8Bit());
	//ShellExecuteA(NULL, "open", to_write.c_str(), NULL, NULL, SW_SHOW);
}

void Experiment1::Show_Result()
{
	ShellExecuteA(NULL, "open", "result.txt", NULL, NULL, SW_SHOW);
}

void Experiment1::Exit() {
	if (!(QMessageBox::information(this, tr("提示"), tr("您要退出此程序吗?"), tr("是"), tr("否"))))
	{
		QApplication* app;
		app->exit(0);
	}
}
