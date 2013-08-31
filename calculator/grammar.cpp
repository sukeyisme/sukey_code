#include "grammar.h"
#include "initializtion.h"

Grammar::Grammar(void)
{
	printf("Initializing the grammer...");
	int i(0);
	//�ѹؼ��֣����Ŷ����뵽�ռ���ӳ���У����������ǵ��ڲ���ʾsymbol
	for(i=0; i<kKeyNum;i++)
		terminals_[key[i]] = make_pair(KEY,i);//����ؼ���
	for(i=0; i<kBorderNum; i++)
		terminals_[border[i]]=make_pair(BORDER,i); //�����������
	for(i=0; i<kArithNum; i++)
		terminals_[arithmetic[i]]=make_pair(ARITHMETIC,i); //������ѧ����
	for(i=0; i<kRelNum; i++)
		terminals_[relation[i]]=make_pair(RELATION,i);//������ϵ����
	terminals_["INTC"] = make_pair(INTC,-1);//Ϊ���������ڲ���ʾ
	//terminals_["CHAR"] = make_pair(CHARC,-1);//Ϊ�ַ������ڲ���ʾ
	terminals_["ID"] = make_pair(ID,-1);//Ϊ��ʶ�������ڲ���ʾ
	//terminals_["$"] = make_pair($,-1);//"$" means null��ΪNULL�����ڲ���ʾ
	//terminals_["EOF"] = make_pair(IEOF,-1);//Ϊ�ļ������������ڲ���ʾ

	//��ʼ�� ���ռ���
	start_symbol_ = make_pair(VN,0);
	//��ʼ�� ������

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
