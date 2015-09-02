#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"

void RegisterId(Node *node)
{
	auto id = node->GetIdentifier();
	if (StringToNode[id]) {
		error("Id <%s> is already registered\n", id.c_str());
	}
	else {
		trace("Registering id <%s>\n", id.c_str());
		StringToNode[id] = node;
	}
}

void AssignStmt::Evaluate()
{
	if(!Lhs->HasTargetRegister) {
		RegisterId(Lhs);
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
