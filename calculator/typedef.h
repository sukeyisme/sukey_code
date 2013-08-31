#pragma once
#include <utility>
#include <string>
#include <map>
#include <vector>


using namespace std;

//定义符号的类型
enum SymbolTypes
{
	VN,
	KEY,		//关键字
	BORDER,		//符号
	ARITHMETIC,	//算术运算
	RELATION,	
	INTC,		//常数
	CHARC,		//字符
	ID,			//标识符
	$,
	ACTION,
	DEFAULT,
	IEOF
};

//定义符号的结构，即类型和在表中的偏移
typedef pair<SymbolTypes,int> Symbol;

//定义字符串到内部符号的映射，即关联数组
typedef map<string,Symbol,less<string>>Symbols;

//定义终极符和非终极符
typedef Symbol NonTerminal;
typedef Symbol Terminal;

//定义产生式的结构，一个非终极符作为左部，一个符号数组作为右部
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

//标识符的类型
enum IdKinds
{
	TYPEKIND,
	VARKIND,
	PROCKIND
};

//变量的类别。dir表直接变量（值参），indir表示间接变量(变参)
enum AccessKinds
{
	DIR,
	INDIR
};

struct TypeIR;
//标识符的属性结构定义
typedef struct
{
	struct TypeIR *idtype;//指向标识符的类型内部表示
	IdKinds kind;			//标识符类型
	union
	{
		struct
		{
			AccessKinds access;//判断是变参还是值参
			int			level;
			int			off;
			bool		is_param;//判断是参数还是普通变量
		}VarAttr;//变量标示符的属性
		struct
		{
			int			level;//该过程的层数
		}ProcAttr;//过程名标识符的属性
	}More;//标识符的不同类型有不用的属性
}AttributeIR,*pAttributeIR;


////////////////////////类型内部标示/////////////////////////

//类型的枚举定义
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
//域类型单元结构定义
typedef struct Field
{
	char* id;//变量名
	int	  off;//所在记录中的偏移
	struct typeIR* unit_type;//域中成员的类型
	struct field*	next;//指向下一个域成员
}FieldTy,*pFiledTy;

//类型的内部结构定义
typedef struct TypeIR
{
	int			size;//类型所占空间大小
	TypeKinds	kind;
	union
	{
		struct
		{
			struct TypeIR *elem_ty;//记录数组元素的类型
			int		low;//记录数组类型的下界
			int		top;//记录数组类型的上界
		}ArrayAttr;
		pFiledTy body;//记录类型中的域
	}more;
}TypeIRTy,*pTypeIRTy;
