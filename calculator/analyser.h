#pragma once
#include "Grammar.h"
using namespace std;

class Analyser
{
public:
	Analyser(const Grammar &grammar);
	~Analyser(){};

	//map<pair<nonter>>
protected:
	Grammar m_grammar;

	void MarkLambda();//找出能推出空的非终极符
};