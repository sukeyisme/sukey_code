#pragma once
#include "typedef.h"
using namespace std;

class Grammar
{
public:
	Grammar(void);
	~Grammar(void){};

	Symbol Search(const string& search,SymbolTypes word_type);
	string GetStr(const Symbol &sym);

	Symbols terminals_;//�����ֹ��
	
protected:
	vector<string> intliterals_;  //������
	vector<string>	id_;			//��ʶ����

	NonTerminal start_symbol_;//��ʼ��
};