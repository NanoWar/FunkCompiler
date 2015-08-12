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
		error("Id '%s' already registered.\n", id.c_str());
	}
	else {
		info("Registering id '%s'.\n", id.c_str());
		StringToNode[id] = node;
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
