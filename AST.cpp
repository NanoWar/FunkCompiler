#include "Global.h"

#include "AST.h"
#include "Console.h"
#include "Register.h"

RegisterUsage *Node::GetRegisterUsage()
{
	Node *node = this;
	while (node)
	{
		if (dynamic_cast<FunctionDeclNode *>(node))
		{
			return &(((FunctionDeclNode *)node)->RegisterUsage);
		}
		node = node->Parent;
	}
	Error("AST corrupt, no register scope found");
	return NULL;
}
