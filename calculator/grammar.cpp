#include "grammar.h"
#include "initializtion.h"

Grammar::Grammar(void)
{
	printf("Initializing the grammer...");
	int i(0);
	//把关键字，符号都加入到终极符映射中，并建立它们的内部表示symbol
	for(i=0; i<kKeyNum;i++)
		terminals_[key[i]] = make_pair(KEY,i);//填入关键字
	for(i=0; i<kBorderNum; i++)
		terminals_[border[i]]=make_pair(BORDER,i); //填入特殊符号
	for(i=0; i<kArithNum; i++)
		terminals_[arithmetic[i]]=make_pair(ARITHMETIC,i); //填入数学符号
	for(i=0; i<kRelNum; i++)
		terminals_[relation[i]]=make_pair(RELATION,i);//填入联系符号
	terminals_["INTC"] = make_pair(INTC,-1);//为常数建立内部表示
	//terminals_["CHAR"] = make_pair(CHARC,-1);//为字符建立内部表示
	terminals_["ID"] = make_pair(ID,-1);//为标识符建立内部表示
	//terminals_["$"] = make_pair($,-1);//"$" means null，为NULL建立内部表示
	//terminals_["EOF"] = make_pair(IEOF,-1);//为文件结束符建立内部表示

	//初始化 非终极符
	start_symbol_ = make_pair(VN,0);
	//初始化 动作符

}

Symbol Grammar::Search(const string &search, SymbolTypes word_type)
{
	Symbols::iterator pos;
	int i(0);
	switch(word_type)
	{
	case KEY:
	case RELATION:
	case BORDER:
	case ARITHMETIC:
		{
			pos = terminals_.find(search);
			if(pos!=terminals_.end())
				return pos->second;
			return make_pair(DEFAULT,0);
		}
	case INTC:
		{
			pos = terminals_.find(search);
			if ( pos != terminals_.end() ) 
				return pos->second;
			else
			{
				terminals_[search] = make_pair(INTC,intc_num);
				intliterals_.push_back(search);
			}
			return make_pair(INTC,intc_num++);
		}
	case ID:
		{
			pos = terminals_.find(search);
			if ( pos != terminals_.end() ) 
				return pos->second;
			else
			{
				terminals_[search]=make_pair(ID,id_num);
				id_.push_back(search);
				return make_pair(ID,id_num++);
			}
		}
	default:
		return make_pair(DEFAULT,0);
	}
}

string Grammar::GetStr(const Symbol &sym)
{
	if (sym.second == -1) return name[(int)sym.first];
	else 
	switch(sym.first)
	{
	case KEY:
		return key[sym.second];
	case BORDER:
		return border[sym.second];
	case ARITHMETIC:
		return arithmetic[sym.second];
	case RELATION:
		return relation[sym.second];
	case INTC:
		return intliterals_[sym.second];
	case ID:
		return id_[sym.second];
	case IEOF:
		return "EOF";
	case DEFAULT:
		return "Sorry,it's Fail!";
	default:
		return NULL;
	}
}
