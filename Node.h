#ifndef Node_h
#define Node_h

#include "Global.h"
#include "Parser.h"

class Node;
class FunctionDeclNode;
class RegisterPool;

class Node
{
public:
	string Name;
	Node *Parent;
	YYLTYPE SourceLoc;

	Node() // TODO: Remove to guarantee location
	{
		Name = "";
		Parent = NULL;
	}

	Node(YYLTYPE loc)
	{
		Node();
		SourceLoc = loc;
		SourceLoc.file_name = yylloc.file_name;
	}

	virtual string GetName() { return Name; }
	string GetIdentifier();
	virtual void Evaluate() { }
	virtual void Compile() { }

	FunctionDeclNode *GetFunctionScope();
	RegisterPool *GetRegisterPool();
};

#endif