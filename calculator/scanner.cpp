#include "scanner.h"
#include "process.h"
#include "ctype.h"
#include <iostream>

Scanner::Scanner(Grammar *grammar)
{
	m_grammar=grammar;
	m_line_num=1;
	if((m_fp=fopen("c:\\demo.txt","r+"))==NULL)
	{
		exit(1);
	}
	else
		Scan();
}

char Scanner::AlphaProcess(char buffer)
{
	Symbol atype;
	int i(-1);
	char alphatp[20];
	while((isalpha(buffer))||(isdigit(buffer)))
	{
		alphatp[++i] = buffer;
		buffer  = fgetc(m_fp);
	};
	alphatp[i+1]='\0';
	atype = m_grammar->Search(alphatp,KEY);
	if(atype.first != DEFAULT)
		token_list.push_back(Token(atype,m_line_num));
	else
	{
		atype = m_grammar->Search(alphatp,ID);
		token_list.push_back(Token(atype,m_line_num));
	};
	
	return buffer;
}

char Scanner::DigitProcess(char buffer)
{
	int i=0;
	char digit[20];
	Symbol type;
	while((isdigit(buffer)))
	{
		digit[i] = buffer;
		i++;
		buffer=fgetc(m_fp);
	}
	digit[i]='\0';
	type = m_grammar->Search(digit,INTC);
	token_list.push_back(Token(type,m_line_num));
	
	return buffer;
}

char Scanner::OtherProcess(char buffer)
{
	int i(0);
	char other[20];
	Symbol otype,ttype=make_pair(DEFAULT,0);
	if(buffer=='\n'||buffer==' '||buffer=='\t')
	{
		buffer = fgetc(m_fp);
		return buffer;
	}
	while((!isdigit(buffer))&&(!isalpha(buffer))&&(buffer!=' ')&&(buffer!='\n'))
	{
		other[i]=buffer;
		i++;
		other[i]='\0';

		otype=m_grammar->Search(other,ARITHMETIC);
		if (otype.first == DEFAULT)
			break;
		else 
		{ 
			ttype=otype; 
			buffer=fgetc(m_fp);
			continue; 
		}
		otype = m_grammar->Search(other,BORDER);
		if(otype.first==DEFAULT)
			break;
		else
		{
			ttype=otype;
			buffer = fgetc(m_fp);
			continue;
		}
	}
	if(ttype.first!= DEFAULT)
	{
		token_list.push_back(Token(ttype,m_line_num));
		return buffer;
	}
	else
	{
		printf("%c%d error,not a word\n",other,m_line_num);
		exit(1);
	}
}

void Scanner::Scan()
{
	printf("Scanning...");
	m_buffer = fgetc(m_fp);
	while(m_buffer!=EOF)
	{
		if(m_buffer=='\n') m_line_num++;//Ã¿ÐÐ¼Ó1
		if(isalpha(m_buffer))
			m_buffer = AlphaProcess(m_buffer);
		else if(isdigit(m_buffer))
			m_buffer = DigitProcess(m_buffer);
		else
			m_buffer = OtherProcess(m_buffer);
	}
	PrintToken();	
}

void Scanner::PrintToken()
{
	printf("Token list:\n");
	list<Token>::iterator it = token_list.begin();
	while(it!=token_list.end())
	{
		string a = m_grammar->GetStr(it->value);
		cout<< " " << a <<endl;
		it++;
	}
	m_itr = token_list.begin();
	//printf("result = %f\n",Calculator());
//	Compiler();
}

double Scanner::Calculator()
{
	double temp = Term();
	while(m_itr!=token_list.end()&&(m_itr->value.first==ARITHMETIC)&&(m_itr->value.second==0||m_itr->value.second==1))
	{
		switch(m_itr->value.second)
		{
		case 0:
			m_itr++;
			temp += Term();
			break;
		case 1:
			m_itr++;
			temp -= Term();
			break;
		default:
			break;
		}
	}
	return temp;
}

double Scanner::Term()
{
	double temp;
	temp = Factor();
	while(m_itr!=token_list.end()&&(m_itr->value.first==ARITHMETIC)&&(m_itr->value.second==2||m_itr->value.second==3))
	{
		switch(m_itr->value.second)
		{
		case 2:
			m_itr++;
			temp *= Factor();
			break;
		case 3:
			m_itr++;
			temp /= Factor();
			break;
		default:
			break;
		}
	}
	return temp;
}

double Scanner::Factor()
{
	double number(0);
	if(m_itr==token_list.end())
		return number;
	switch(m_itr->value.first)
	{
	case INTC:
		number = atoi(m_grammar->GetStr(m_itr->value).c_str());
		m_itr++;

		if(m_itr!=token_list.end()&&m_itr->value.first==BORDER&&m_itr->value.second==0)
		{
			number = GetFloat(number);
		}
		break;
	case BORDER:
		{
			if(m_itr->value.second==1)
			{
				m_itr++;
				number = Calculator();
				m_itr++;
			}
			break;
		}
	default:
		break;
	}
	return number;
}


double Scanner::GetFloat(double dbpre)
{
	double temp(0);
	
	m_itr++;
	temp = Factor();

	do
	{
		temp /= 10;
	}while(temp>1);

	temp += dbpre;
	return temp;
}
