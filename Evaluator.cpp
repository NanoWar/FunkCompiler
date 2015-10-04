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
		Trace("Id <%s> is already registered", id.c_str());
		return false;
	}
	else {
		Trace("Registering id <%s>", id.c_str());
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
		GetRegisterUsage()->SetUsage(Lhs->TargetRegister, ERegisterUsage::USED);
	}
}

void IfStmt::Evaluate()
{
	TrueStmts->Evaluate();
	FalseStmts->Evaluate();
}

void ParameterNode::Evaluate()
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
	Statements->Evaluate();
}
