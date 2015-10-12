#include "Global.h"

#include "AST.h"
#include "Console.h"
#include "Register.h"

FunctionDeclNode *Node::GetFunctionScope()
{
	Node *node = this;
	while (node)
	{
		if (dynamic_cast<FunctionDeclNode *>(node))
		{
			return (FunctionDeclNode *)node;
		}
		node = node->Parent;
	}
	Error("Could not find function scope");
	return NULL;
}

RegisterUsage *Node::GetRegisterUsage()
{
	return &(GetFunctionScope()->RegisterUsage);
}
