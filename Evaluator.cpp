#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"

bool RegisterId(Node *node)
{
	auto id = node->GetIdentifier();
	if (StringToNode[id]) {
		Trace(node, "Id <%s> is already registered", id.c_str());
		return false;
	}
	else {
		Trace(node, "Registering id <%s>", id.c_str());
		StringToNode[id] = node;
		return true;
	}
}

void AssignStmt::Evaluate()
{
	if(dynamic_cast<IdentRegExpr *>(Lhs))
	{
		RegisterId(Lhs);
	}

	if (!Lhs->HasTargetRegister)
	{
		RegisterId(Lhs);
	}
	else
	{
		GetRegisterUsage()->SetUsage(this, Lhs->TargetRegister, ERegisterUsage::USED);
	}
}

void CompareExpr::Evaluate()
{
	Lhs->Evaluate();
	Rhs->Evaluate();

	if (Lhs->HasStaticValue && Rhs->HasStaticValue)
	{
		if (Lhs->Value == Rhs->Value)
		{
			Warn(this, "Comparison is always true");
			Value = 1;
		}
		else
		{
			Warn(this, "Comparison is always false");
			Value = 0;
		}
		HasStaticValue = true;
		return;
	}
	
	if(Lhs->HasTargetRegister)
	{
		AssignStmt *assign;
		if(IS_BIG(Lhs->TargetRegister))
		{
			assign = new AssignStmt(SourceLoc, ERegister::HL, Lhs->TargetRegister);
			Lhs->TargetRegister = ERegister::HL;
		}
		else
		{
			assign = new AssignStmt(SourceLoc, ERegister::A, Lhs->TargetRegister);
			Lhs->TargetRegister = ERegister::A;
		}
		Setups.push_back(assign);
		assign->Parent = this;
		assign->Evaluate();
	}
}

void IfStmt::Evaluate()
{
	Expr->Evaluate();
	TrueStmts->Evaluate();
	FalseStmts->Evaluate();
}

void ParameterNode::Evaluate()
{
	RegisterId(this);
}

void ResultNode::Evaluate()
{
	RegisterId(this);
}

void ModuleNode::Evaluate()
{
	Statements->Evaluate();
}

void FunctionDeclNode::Evaluate()
{
	RegisterId(this);
	Parameters->Evaluate();
	Results->Evaluate();
	Statements->Evaluate();
}
