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

	char AlphaProcess(char buffer);  //�ؼ��ֺͱ�ʶ�������Ӻ���
	char DigitProcess(char buffer);   //���ִ�����
	char OtherProcess(char buffer);    //�����ַ�������

	list<Token>::iterator m_itr;

	Grammar *m_grammar;
};