#ifndef AST_h
#define AST_h

#include "Global.h"
#include "Register.h"
#include "Parser.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include <typeinfo>

class Node;
class FunctionDeclNode;

extern map<Node*, string> NodeToString;
extern map<string, Node*> StringToNode;

class Node
{
public:
	string Name;
	int SourceLine;
	Node *Parent;

	Node()
	{
		Name = "";
		Parent = NULL;
		SourceLine = -1;
	}

	string GetIdentifier();
	virtual void Evaluate() { }
	virtual void Compile() { }

	FunctionDeclNode *GetFunctionScope();
	RegisterUsage *GetRegisterUsage();
};

template<typename OwnType, typename ItemType, typename BaseType = Node>
class VectorNode : public BaseType
{
public:
	vector<ItemType *> Children;

	VectorNode() { }
	VectorNode(ItemType *item) { Extend(item); }

	bool HasChildren() { return Children.size() > 0; }

	ItemType *operator[](const int index)
	{
		return Children[index];
	}

	virtual OwnType *Extend(ItemType *item)
	{
		Children.push_back(item);
		return(OwnType *) this;
	}

	OwnType *Extend(int n, ...)
	{
		va_list params;
		va_start(params, n);
		Children.reserve(Children.size() + n);
		for (int i = 0; i < n; i++) {
			auto item = va_arg(params, ItemType *);
			Extend(item);
		}
		va_end(params);
		return(OwnType *) this;
	}

	OwnType *Extend(vector<ItemType*> items)
	{
		Children.reserve(Children.size() + items.size());
		for (int i = 0; i < items.size(); i++) {
			auto item = items[i];
			Extend(item);
		}
		return(OwnType *) this;
	}
};

template<typename OwnType, typename ItemType, typename BaseType = Node>
class ContainerNode : public VectorNode < OwnType, ItemType, BaseType >
{
public:
	ContainerNode() { }
	ContainerNode(ItemType *item) : VectorNode(item) { }

	OwnType *Extend(ItemType *node)
	{
		Children.push_back(node);
		((Node *)node)->Parent = this;
		return(OwnType *) this;
	}

	void Evaluate()
	{
		for (auto child = Children.begin(); child != Children.end(); ++child) {
			(*child)->Evaluate();
		}
	}

	void Compile()
	{
		for (auto child = Children.begin(); child != Children.end(); ++child) {
			(*child)->Compile();
		}
	}

	~ContainerNode()
	{
		for (auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}
};

class StatementNode : public Node
{
public:
};

class AsmNode : public StatementNode
{
public:
	string Text;

	AsmNode(string text) : Text(text) { }

	void Compile();
};

class StatementsNode : public ContainerNode < StatementsNode, StatementNode >
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
		Size = 1;
	}

	bool IsValid()
	{
		if (TargetRegister < REGISTER_BIG && Size == 1) return true;
		if (TargetRegister >= REGISTER_BIG && Size == 2) return true;
		return false;
	}

	void SetTargetRegister(ERegister reg)
	{
		TargetRegister = reg;
		HasTargetRegister = true;
		Size = REG_SIZE(reg);
		//TODO: this should not be set:
		Target = RegisterStringMap[reg];
	}

	virtual void Compile() { }
};

class ExpressionsNode : public ContainerNode < ExpressionsNode, ExpressionNode >
{
public:
};

// Expression wrapper in statement
class ExpressionStmt : public StatementNode
{
public:
	ExpressionNode *Expr;

	ExpressionStmt(ExpressionNode *expr) : Expr(expr)
	{
		Expr->Parent = this;
	}

	~ExpressionStmt()
	{
		delete Expr;
	}

	void Compile() { Expr->Compile(); }
};

class IdentExpr : public VectorNode < IdentExpr, string, ExpressionNode >
{
public:
	IdentExpr(string ident)
	{
		Name = ident;
		Size = 2; // TODO: check if 2 is good default
	}
	Node *GetReferenced();
	string GetName();
	void Compile();
};

class IdentRegExpr : public IdentExpr
{
public:
	IdentRegExpr(string name, ERegister reg)
		: IdentExpr(name)
	{
		HasTargetRegister = true;
		TargetRegister = reg;
		Size = REG_SIZE(reg);
	}
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
		if (number > 256 || number < -127)
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
		HasStaticValue = true;
		if (Size == 1)
			Value = chars[0];
		else
			Value = chars[1] * 256 + chars[0];
		stringstream ss;
		ss << "'" << chars << "'";
		Target = ss.str();
	}
};

class StringExpr : public IdentExpr
{
public:
	StringExpr(string str) : IdentExpr(AddString(str))
	{
		Size = 2;
	}

	void Compile() { Target = Name; }
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
		rhs->Parent = this;
	}

	~PlusExpr()
	{
		delete Lhs;
		delete Rhs;
	}

	void Compile();
};

class CompareExpr : public ExpressionNode
{
public:
	ExpressionNode *Lhs;
	ExpressionNode *Rhs;

	CompareExpr(ExpressionNode *lhs, ExpressionNode *rhs)
		: Lhs(lhs), Rhs(rhs)
	{
		Size = 0;
		Lhs->Parent = this;
		Rhs->Parent = this;
	}

	~CompareExpr()
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

	IndirectionExpr(ExpressionNode *expr)
		: Expr(expr)
	{
		expr->Parent = this;
		Size = 1;
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
		Size = REG_SIZE(reg);
		//Trace("Register %s in line %d", Target.c_str(), SourceLine);
	}
};

class AssignStmt : public StatementNode
{
public:
	ExpressionNode *Lhs;
	ExpressionNode *Rhs;

	AssignStmt(ERegister reg, ExpressionNode *rhs)
		: Rhs(rhs)
	{
		Lhs = new RegisterExpr(reg);
		Lhs->Parent = this;
		Rhs->Parent = this;
	}

	AssignStmt(IdentExpr *ident, ExpressionNode *rhs)
		: Lhs(ident), Rhs(rhs)
	{
		Lhs->Parent = this;
		Rhs->Parent = this;
	}

	~AssignStmt()
	{
		delete Lhs;
		delete Rhs;
	}

	void Compile();
	void Evaluate();
};


class ModuleNode : public StatementNode
{
public:
	StatementsNode *Statements;

	ModuleNode(string *name, StatementsNode *statements)
		: Statements(statements),
		StatementNode()
	{
		Name = *name;
		statements->Parent = this;
		Trace("Creating module <%s> in line %d", Name.c_str(), SourceLine);
	}

	~ModuleNode()
	{
		delete Statements;
	}

	void Evaluate();
	void Compile();
};

class ParameterNode : public Node
{
public:
	ERegister Register;

	ParameterNode(string *name, ERegister reg)
		: Register(reg)
	{
		Name = *name;
	}

	void Evaluate();
	void Compile();
};

class ParametersNode : public ContainerNode < ParametersNode, ParameterNode >
{
public:
};

class ResultNode : public Node
{
public:
	ERegister Register;

	ResultNode(ERegister reg)
		: Register(reg)
	{
	}

	ResultNode(string *name, ERegister reg)
		: Register(reg)
	{
		Name = *name;
	}

	void Evaluate();
	void Compile();
};

class ResultsNode : public ContainerNode < ResultsNode, ResultNode >
{
public:
};

class OutputNode : public Node
{
public:
};

class OutputsNode : public ContainerNode < OutputsNode, OutputNode >
{
public:
};

class FunctionCallExpr : public ExpressionNode
{
public:
	IdentExpr *Ident;
	ExpressionsNode *Parameters;

	FunctionCallExpr(IdentExpr *ident, ExpressionsNode *parameters)
		: Ident(ident), Parameters(parameters),
		ExpressionNode()
	{
		Ident->Parent = this;
		Parameters->Parent = this;
	}

	~FunctionCallExpr()
	{
		delete Ident;
		delete Parameters;
	}

	void Compile();
};

// Function call without return parameters
class FunctionCallStmt : public ExpressionStmt
{
public:
	FunctionCallStmt(FunctionCallExpr *functionCall)
		: ExpressionStmt(functionCall)
	{
	}
};

class Scope
{
public:
	RegisterUsage RegisterUsage;
	int Counter;
	Scope() : Counter(0) { }
};

class FunctionDeclNode : public StatementNode, public Scope
{
public:
	ParametersNode *Parameters;
	StatementsNode *Statements;
	ResultsNode *Results;

	FunctionDeclNode(string *name, ParametersNode *parameters, StatementsNode *statements, ResultsNode *results)
		: Parameters(parameters), Statements(statements), Results(results),
		StatementNode()
	{
		Name = *name;
		parameters->Parent = this;
		statements->Parent = this;
		results->Parent = this;
	}

	~FunctionDeclNode()
	{
		delete Parameters;
		delete Statements;
		delete Results;
	}

	void Evaluate();
	void Compile();
};

class IfStmt : public StatementNode
{
public:
	ExpressionNode *Expr;
	StatementsNode *TrueStmts;
	StatementsNode *FalseStmts;

	IfStmt(ExpressionNode *expr, StatementsNode *ifStmts, StatementsNode *elseStmts)
		: Expr(expr), TrueStmts(ifStmts), FalseStmts(elseStmts),
		StatementNode()
	{
		Expr->Parent = this;
		TrueStmts->Parent = this;
		FalseStmts->Parent = this;
	}

	~IfStmt()
	{
		delete TrueStmts;
		delete FalseStmts;
	}

	void Evaluate();
	void Compile();
};


class SaveListNode : public StatementNode
{
public:
	vector<ERegister> Registers;
	vector<IdentExpr *> Idents;

	SaveListNode() : StatementNode() { }

	SaveListNode *Extend(ERegister reg) { Registers.push_back(reg); return this; }
	SaveListNode *Extend(IdentExpr *ident) { Idents.push_back(ident); ident->Parent = this; return this; }

	~SaveListNode()
	{
		for (auto ident = Idents.begin(); ident != Idents.end(); ++ident) {
			delete *ident;
		}
	}
};

class SaveStmt : public StatementNode
{
public:
	SaveListNode *SaveList;
	StatementsNode *Statements;

	SaveStmt(SaveListNode *save_list, StatementsNode *stmts)
		: SaveList(save_list), Statements(stmts)
	{
		SaveList->Parent = this;
		Statements->Parent = this;
	}

	~SaveStmt()
	{
		delete SaveList;
		delete Statements;
	}

	void Compile();
};

class ReturnNode : public StatementNode
{
public:
	ExpressionsNode *Exprs;

	ReturnNode(ExpressionsNode *exprs)
		: Exprs(exprs), StatementNode()
	{
		Exprs->Parent = this;
	}

	~ReturnNode()
	{
		delete Exprs;
	}

	void Compile();
};

#endif