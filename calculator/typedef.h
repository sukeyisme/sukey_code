#pragma once
#include <utility>
#include <string>
#include <map>
#include <vector>


using namespace std;

//������ŵ�����
enum SymbolTypes
{
	VN,
	KEY,		//�ؼ���
	BORDER,		//����
	ARITHMETIC,	//��������
	RELATION,	
	INTC,		//����
	CHARC,		//�ַ�
	ID,			//��ʶ��
	$,
	ACTION,
	DEFAULT,
	IEOF
};

//������ŵĽṹ�������ͺ��ڱ��е�ƫ��
typedef pair<SymbolTypes,int> Symbol;

//�����ַ������ڲ����ŵ�ӳ�䣬����������
typedef map<string,Symbol,less<string>>Symbols;

//�����ռ����ͷ��ռ���
typedef Symbol NonTerminal;
typedef Symbol Terminal;

//�������ʽ�Ľṹ��һ�����ռ�����Ϊ�󲿣�һ������������Ϊ�Ҳ�
struct Production
{
	NonTerminal lhs;
	vector<Symbol> rhs;
};

class Token
{
public:
	Token(Symbol v,int l):value(v),line(l){};
	~Token(){};
	Symbol value;
	int line;
};

//��ʶ��������
enum IdKinds
{
	TYPEKIND,
	VARKIND,
	PROCKIND
};

//���������dir��ֱ�ӱ�����ֵ�Σ���indir��ʾ��ӱ���(���)
enum AccessKinds
{
	DIR,
	INDIR
};

struct TypeIR;
//��ʶ�������Խṹ����
typedef struct
{
	struct TypeIR *idtype;//ָ���ʶ���������ڲ���ʾ
	IdKinds kind;			//��ʶ������
	union
	{
		struct
		{
			AccessKinds access;//�ж��Ǳ�λ���ֵ��
			int			level;
			int			off;
			bool		is_param;//�ж��ǲ���������ͨ����
		}VarAttr;//������ʾ��������
		struct
		{
			int			level;//�ù��̵Ĳ���
		}ProcAttr;//��������ʶ��������
	}More;//��ʶ���Ĳ�ͬ�����в��õ�����
}AttributeIR,*pAttributeIR;


////////////////////////�����ڲ���ʾ/////////////////////////

//���͵�ö�ٶ���
enum TypeKinds
{
	INTTY,
	CHARTY,
	ARRAYTY,
	RECORDTY,
	BOOLTY
};

struct TypeIR;
struct Field;
//�����͵�Ԫ�ṹ����
typedef struct Field
{
	char* id;//������
	int	  off;//���ڼ�¼�е�ƫ��
	struct typeIR* unit_type;//���г�Ա������
	struct field*	next;//ָ����һ�����Ա
}FieldTy,*pFiledTy;

//���͵��ڲ��ṹ����
typedef struct TypeIR
{
	int			size;//������ռ�ռ��С
	TypeKinds	kind;
	union
	{
		struct
		{
			struct TypeIR *elem_ty;//��¼����Ԫ�ص�����
			int		low;//��¼�������͵��½�
			int		top;//��¼�������͵��Ͻ�
		}ArrayAttr;
		pFiledTy body;//��¼�����е���
	}more;
}TypeIRTy,*pTypeIRTy;
