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

	void MarkLambda();//�ҳ����Ƴ��յķ��ռ���
};