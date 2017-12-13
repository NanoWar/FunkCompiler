#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"
#include "Definitions.h"

bool Register(string str, Node *node)
{
	if (StringToNode[str]) {
		Error(node, "Id <%s> is already registered", str.c_str());
		return false;
	}
	else {
		Trace(node, "Registering id <%s>", str.c_str());
		StringToNode[str] = node;
		return true;
	}
}

bool RegisterId(Node *node)
{
	auto id = node->GetIdentifier();
	return Register(id, node);
}

void IdentRegExpr::Evaluate()
{
	RegisterId(this);
}

void IdentExpr::Evaluate()
{
}

void AssignStmt::Evaluate()
{
	Lhs->Evaluate();
	Rhs->Evaluate();

	if (!Lhs->HasTargetRegister)
	{
		// Its just a naming
		RegisterId(Lhs);
	}
	else
	{
		// Check if register is mutable
		auto reg = Lhs->TargetRegister;
		auto pool = GetRegisterPool();
		if (pool->GetUsageInfo(reg).IsMutable == false)
		{
			Error(this, "Register <%s> is immutable", RSMx(reg));
			return;
		}

		pool->SetUsage(this, reg, ERegisterUsage::USED);
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
	if (IsMutable == false)
	{
		// Set immutable registers as used
		auto pool = GetRegisterPool();
		pool->SetMutability(this, Register, IsMutable);
		pool->SetUsage(this, Register, ERegisterUsage::USED);
	}
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

void StructNode::Evaluate()
{
	RegisterId(this);
	Definitions->Evaluate();
}
