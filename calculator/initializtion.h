//��ʼ���﷨
#pragma once
using namespace std;

const int kKeyNum = 2;//�ؼ��ָ���
const int kBorderNum = 4;//���Ÿ���
const int kArithNum = 4;//�������������
const int kRelNum = 7;//��ϵ���������
const int kNameNum = 8;//�����������ָ���


string border[kBorderNum] = {".","(",")",";"};
string arithmetic[kArithNum] = {"+","-","*","/"};
int intc_num=0,//���ͳ�������
	id_num = 0,//��ʶ������
	charc_num =0;//�ַ���������

string key[kKeyNum]=//�ؼ��ֱ�
{
	"int","return"
};

string relation[kRelNum] = {"<",">","<=",">=","==","!=","="};//��ϵ�������

string name[kNameNum] = {"VN","KEY","BORDER","ARITHMETIC","RELATION","INTC","CHARC","ID"};