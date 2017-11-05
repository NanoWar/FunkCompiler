#ifndef AST_h
#define AST_h

#include "Global.h"
#include "Node.h"
#include "Register.h"
#include "Parser.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include <typeinfo>

extern map<Node*, string> NodeToString;
extern map<string, Node*> StringToNode;

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
	StatementNode() : Node() {} // TODO: Remove to guarantee location
	StatementNode(YYLTYPE loc) : Node(loc) {}
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

	ExpressionNode(YYLTYPE loc) : Node(loc)
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

	ExpressionStmt(YYLTYPE loc, ExpressionNode *expr)
		: Expr(expr),
		StatementNode(loc)
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
	IdentExpr(YYLTYPE loc, string ident)
	{
		Name = ident;
		SourceLoc = loc;
		SourceLoc.file_name = yylloc.file_name;
		Size = 2; // TODO: check if 2 is good default
	}

	Node *GetReferenced();
	string GetName();

	void Evaluate();
	void Compile();
};

// LHS only!
class IdentRegExpr : public IdentExpr
{
public:
	IdentRegExpr(YYLTYPE loc, string name, ERegister reg)
		: IdentExpr(loc, name)
	{
		HasTargetRegister = true;
		TargetRegister = reg;
		Size = REG_SIZE(reg);
	}

	void Evaluate();
};

class NumberExpr : public ExpressionNode
{
public:
	NumberExpr(string str)
	{
		Target = str;
		HasStaticValue = true;
		Value = stoi(str);
		Size = Value > 255 || Value < -127 ? 2 : 1;
	}

	// Word
	NumberExpr(YYLTYPE loc, NumberExpr *hi, NumberExpr *lo)
		: ExpressionNode(loc)
	{
		if (hi->Size > 1)
		{
			Error(this, "Compound high number part (%d) is larger than one byte", hi->Value);
		}
		if (lo->Size > 1)
		{
			Error(this, "Compound low number part (%d) is larger than one byte", lo->Value);
		}
		Size = 2;
		HasStaticValue = true;
		Value = hi->Value * 256 + lo->Value;
		Target = to_string(Value);
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
	StringExpr(YYLTYPE loc, string str) : IdentExpr(loc, AddString(str))
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

class SetupNode
{
public:
	vector<Node *> Setups;

	void Compile_Setup()
	{
		for (auto setup = Setups.begin(); setup != Setups.end(); ++setup) {
			(*setup)->Compile();
		}
	}
};

class CompareExpr : public ExpressionNode, public SetupNode
{
public:
	ExpressionNode *Lhs;
	ExpressionNode *Rhs;

	CompareExpr(YYLTYPE loc, ExpressionNode *lhs, ExpressionNode *rhs)
		: ExpressionNode(loc), Lhs(lhs), Rhs(rhs)
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
	void Evaluate();
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

	AssignStmt(YYLTYPE loc, ERegister reg, ExpressionNode *rhs)
		: Rhs(rhs),
		StatementNode(loc)
	{
		Lhs = new RegisterExpr(reg);
		Lhs->Parent = this;
		Rhs->Parent = this;
	}

	AssignStmt(YYLTYPE loc, ERegister lreg, ERegister rreg)
		: StatementNode(loc)
	{
		Lhs = new RegisterExpr(lreg);
		Lhs->Parent = this;
		Rhs = new RegisterExpr(rreg);
		Rhs->Parent = this;
	}

	AssignStmt(YYLTYPE loc, IdentExpr *ident, ExpressionNode *rhs)
		: Lhs(ident), Rhs(rhs),
		StatementNode(loc)
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

	ModuleNode(YYLTYPE loc, string *name, StatementsNode *statements)
		: Statements(statements),
		StatementNode(loc)
	{
		Name = *name;
		statements->Parent = this;
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
	bool IsMutable;

	ParameterNode(YYLTYPE loc, string *name, ERegister reg, bool isMutable)
		: Register(reg), IsMutable(isMutable),
		Node(loc)
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

	ResultNode(YYLTYPE loc, ERegister reg)
		: Register(reg),
		Node(loc)
	{
	}

	ResultNode(YYLTYPE loc, string *name, ERegister reg)
		: Register(reg),
		Node(loc)
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

	FunctionCallExpr(YYLTYPE loc, IdentExpr *ident, ExpressionsNode *parameters)
		: Ident(ident), Parameters(parameters),
		ExpressionNode(loc)
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
	FunctionCallStmt(YYLTYPE loc, FunctionCallExpr *functionCall)
		: ExpressionStmt(loc, functionCall)
	{
	}
};

class Scope
{
public:
	RegisterPool *RegisterPool;
	int Counter; // for e.g. label names

	Scope()
	{
		Counter = 0;
		RegisterPool = new ::RegisterPool();
	}

	~Scope()
	{
		delete RegisterPool;
	}
};

class FunctionDeclNode : public StatementNode, public Scope
{
public:
	ParametersNode *Parameters;
	StatementsNode *Statements;
	ResultsNode *Results;

	FunctionDeclNode(YYLTYPE loc, string *name, ParametersNode *parameters, StatementsNode *statements, ResultsNode *results)
		: Parameters(parameters), Statements(statements), Results(results),
		StatementNode(loc)
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

	SaveStmt(YYLTYPE loc, SaveListNode *save_list, StatementsNode *stmts)
		: SaveList(save_list), Statements(stmts),
		StatementNode(loc)
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

	ReturnNode(YYLTYPE loc, ExpressionsNode *exprs) : StatementNode(loc),
		Exprs(exprs)
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