#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"

void ParameterNode::Evaluate()
{
	GetIdentifier(); // Register
}

void ModuleNode::Evaluate()
{
	GetIdentifier(); // Register
	Statements->Evaluate();
}

void FunctionDeclNode::Evaluate()
{
	GetIdentifier(); // Register
	Parameters->Evaluate();
	Statements->Evaluate();
}
