#ifndef AST_h
#define AST_h

#include "Global.h"
#include "StringHelper.h"
#include "Register.h"
#include "Parser.h"
#include "Console.h"
#include <typeinfo>

class Node;

extern map<Node*, string> NodeToString;
extern map<string, Node*> StringToNode;

class Node
{
public:
	string Name;
	int SourceLine;
	Node *Parent;

	Node() : Parent(NULL) { Name = ""; }

	string GetIdentifier();
	virtual void Compile() { }
};

template<typename ItemType, typename BaseType = Node>
class ListNode : public BaseType
{
public:
	vector<ItemType *> Children;

	ListNode() {}
	ListNode(ItemType *item) { Extend(item); }

	ItemType *operator[](const int index)
	{
		return Children[index];
	}

	virtual ListNode *Extend(ItemType *node)
	{
		Children.push_back(node);
		return this;
	}

	~ListNode()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}

};

template<typename OwnType, typename ItemType, typename BaseType = Node>
class ContainerNode : public BaseType
{
public:
	vector<ItemType *> Children;

	ContainerNode() {}
	ContainerNode(ItemType *item) { Extend(item); }

	bool HasChildren() { return Children.size() > 0; }

	ItemType *operator[](const int index)
	{
		return Children[index];
	}

	OwnType *Extend(ItemType *node)
	{
		Children.push_back(node);
		((Node *) node)->Parent = this;
		return(OwnType *) this;
	}

	OwnType *Extend(int n, ...)
	{
		va_list params;
		va_start(params, n);
		Children.reserve(Children.size() + n);
		for(int i = 0; i < n; i++) {
			auto node = va_arg(params, ItemType *);
			Extend(node);
		}
		va_end(params);
		return(OwnType *) this;
	}

	OwnType *Extend(vector<ItemType*> nodes)
	{
		Children.reserve(Children.size() + nodes.size());
		for(int i = 0; i < nodes.size(); i++) {
			auto node = nodes[i];
			Extend(node);
		}
		return(OwnType *) this;
	}

	void ContainerNode::Compile()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			(*child)->Compile();
		}
	}

	~ContainerNode()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}

};

class StatementNode : public Node
{
public:
	StatementNode()
	{
		SourceLine = yylineno;
	}
};

class StatementsNode : public ContainerNode<StatementsNode, StatementNode>
{
public:
};

class ExpressionNode : public Node
{
public:
	string Target;

	bool HasTargetRegister;
	ERegister TargetRegister;

	bool HasStaticValue;
	int Value;

	int Size;
	
	ExpressionNode()
	{
		HasTargetRegister = false;
		HasStaticValue = false;
	}
	
	bool IsValid()
	{
		if(TargetRegister < REGISTER_BIG && Size == 1) return true;
		if(TargetRegister >= REGISTER_BIG && Size == 2) return true;
		return false;
	}

	virtual void Compile() { }
};

class IdentExpr : public ExpressionNode
{
public:
	string Ident;
	IdentExpr(string ident)
	{
		Ident = ident;
		Size = 2;
	}
	void Compile();
};

class NumberExpr : public ExpressionNode
{
public:
	NumberExpr(string str)
	{
		Target = str;
		HasStaticValue = true;
		int number = stoi(str);
		Value = number;
		if(number > 256 || number < -127)
			Size = 2;
		else
			Size = 1;
	}
};

class CharsExpr : public ExpressionNode
{
public:
	CharsExpr(string chars)
	{
		Size = chars.length();
		Target = chars;
		HasStaticValue = true;

		if(Size == 1)
			Value = chars[0];
		else
			Value = chars[1] * 256 + chars[0];
	}
};

class StringExpr : public IdentExpr
{
public:
	StringExpr(string str) : IdentExpr("str_xxx")
	{
		// TODO: Register string
		Size = 2;
	}
};

class PlusExpr : public ExpressionNode
{
public:
	ExpressionNode *Lhs;
	ExpressionNode *Rhs;

	PlusExpr(ExpressionNode *lhs, ExpressionNode *rhs)
		: Lhs(lhs), Rhs(rhs)
	{
		Size = max(lhs->Size, rhs->Size);
		lhs->Parent = this;
		lhs->Parent = this;
	}

	~PlusExpr()
	{
		delete Lhs;
		delete Rhs;
	}

	void Compile();
};

class IndirectionExpr : public ExpressionNode
{
public:
	ExpressionNode *Expr;

	IndirectionExpr(ExpressionNode *expr) : Expr(expr)
	{
		expr->Parent = this;
	}

	~IndirectionExpr()
	{
		delete Expr;
	}

	void Compile();
};


class RegisterExpr : public ExpressionNode
{
public:
	RegisterExpr(ERegister reg)
	{
		HasTargetRegister = true;
		TargetRegister = reg;
		Target = RegisterStringMap[reg];
	}
};

class AssignStmt : public StatementNode
{
public:
	string Target;
	ERegister TargetRegister;
	ExpressionNode *Expr;

	AssignStmt(ERegister target, ExpressionNode *expr) : Expr(expr)
	{
		expr->Parent = this;
		TargetRegister = target;
		Target = RegisterStringMap[target];
	}

	AssignStmt(string *target, ExpressionNode *expr) : Expr(expr)
	{
		expr->Parent = this;
		// TODO: Resolve register
		TargetRegister = ERegister::HL;
		Target = *target;
	}

	~AssignStmt()
	{
		delete Expr;
	}

	void Compile();
};


class ModuleNode : public StatementNode
{
public:
	StatementsNode *Statements;

	ModuleNode(string *name, StatementsNode *statements) : Statements(statements), StatementNode()
	{
		Name = *name;
		statements->Parent = this;
	}

	~ModuleNode()
	{
		delete Statements;
	}
	
	void Compile();
};

class ParameterNode : public Node
{
public:
	ParameterNode(string *name) { Name = *name; }
	void Compile();
};

class ParametersNode : public ContainerNode<ParametersNode, ParameterNode>
{
public:
};

class OutputNode : public Node
{
public:
};

class OutputsNode : public ContainerNode<OutputsNode, OutputNode>
{
public:
};

class FunctionDeclNode : public StatementNode
{
public:
	ParametersNode *Parameters;
	StatementsNode *Statements;

	FunctionDeclNode(string *name, ParametersNode *parameters, StatementsNode *statements)
		: Parameters(parameters), Statements(statements), StatementNode()
	{
		Name = *name;
		parameters->Parent = this;
		statements->Parent = this;
	}

	void Compile();

	~FunctionDeclNode()
	{
		delete Parameters;
		delete Statements;
	}
};


#endif