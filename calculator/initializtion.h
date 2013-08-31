//初始化语法
#pragma once
using namespace std;

const int kKeyNum = 2;//关键字个数
const int kBorderNum = 4;//符号个数
const int kArithNum = 4;//算术运算符个数
const int kRelNum = 7;//关系运算符个数
const int kNameNum = 8;//各种类型名字个数


string border[kBorderNum] = {".","(",")",";"};
string arithmetic[kArithNum] = {"+","-","*","/"};
int intc_num=0,//整型常量个数
	id_num = 0,//标识符个数
	charc_num =0;//字符常量个数

string key[kKeyNum]=//关键字表
{
	"int","return"
};

string relation[kRelNum] = {"<",">","<=",">=","==","!=","="};//关系运算符表

string name[kNameNum] = {"VN","KEY","BORDER","ARITHMETIC","RELATION","INTC","CHARC","ID"};