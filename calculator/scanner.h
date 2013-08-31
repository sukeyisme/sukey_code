#pragma once
#include "grammar.h"
#include <stdio.h>
#include <list>

class Scanner
{
public:
	Scanner(Grammar *grammar);
	~Scanner(){};
	void Scan();

	list<Token> token_list;
	double Calculator();
	double Term();
	double Factor();
	double GetFloat(double dbpre);

protected:
	FILE *m_fp;
	int	m_line_num;
	char m_buffer;
	
	void PrintToken();

	char AlphaProcess(char buffer);  //关键字和标识符处理子函数
	char DigitProcess(char buffer);   //数字处理函数
	char OtherProcess(char buffer);    //其他字符处理函数

	list<Token>::iterator m_itr;

	Grammar *m_grammar;
};