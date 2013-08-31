#include "analyser.h"

Analyser::Analyser(const Grammar& grammar):m_grammar(grammar)
{
	MarkLambda();
	/*fill_first_set(); 
	fill_follow_set();
	fill_predict_set();
	fill_LL_Table();
	show_LL_Table();*/
}

void Analyser::MarkLambda()
{
	
}
