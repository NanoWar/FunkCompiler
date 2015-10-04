#include "Global.h"

#include "AST.h"
#include "Console.h"
#include "Register.h"

FunctionDeclNode *GetParentFunctionNode(Node *start)
{
	while (start)
	{
		if (dynamic_cast<FunctionDeclNode *>(start))
		{
			return (FunctionDeclNode *)start;
		}
		start = start->Parent;
	}
	return NULL;
}

RegisterUsage *Node::GetRegisterUsage()
{
	return &(GetParentFunctionNode(this)->RegisterUsage);
}
