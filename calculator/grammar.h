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

	Symbols terminals_;//存放终止符
	
protected:
	vector<string> intliterals_;  //常数表
	vector<string>	id_;			//标识符表

	NonTerminal start_symbol_;//开始符
};