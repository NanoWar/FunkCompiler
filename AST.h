#ifndef AST_h
#define AST_h

#include "Global.h"
#include "StringHelper.h"
#include <typeinfo>

class Node;

extern map<Node*, string> NodeToString;
extern map<string, Node*> StringToNode;

class Node
{
public:
	string Name;
	int SourceLine;
	Node *Parent;

	Node() : Parent(NULL) { Name = ""; }

	string GetIdentifier();
	virtual void Compile() { }
};

template<typename ItemType, typename BaseType = Node>
class ListNode : public BaseType
{
public:
	vector<ItemType *> Children;

	ListNode() {}
	ListNode(ItemType *item) { Extend(item); }

	ItemType *operator[](const int index)
	{
		return Children[index];
	}

	virtual ListNode *Extend(ItemType *node)
	{
		Children.push_back(node);
		return this;
	}

	~ListNode()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}

};

template<typename OwnType, typename ItemType, typename BaseType = Node>
class ContainerNode : public BaseType
{
public:
	vector<ItemType *> Children;

	ContainerNode() {}
	ContainerNode(ItemType *item) { Extend(item); }

	ItemType *operator[](const int index)
	{
		return Children[index];
	}

	OwnType *Extend(ItemType *node)
	{
		Children.push_back(node);
		((Node *) node)->Parent = this;
		return(OwnType *) this;
	}

	OwnType *Extend(int n, ...)
	{
		va_list params;
		va_start(params, n);
		Children.reserve(Children.size() + n);
		for(int i = 0; i < n; i++) {
			auto node = va_arg(params, ItemType *);
			Extend(node);
		}
		va_end(params);
		return(OwnType *) this;
	}

	OwnType *Extend(vector<ItemType*> nodes)
	{
		Children.reserve(Children.size() + nodes.size());
		for(int i = 0; i < nodes.size(); i++) {
			auto node = nodes[i];
			Extend(node);
		}
		return(OwnType *) this;
	}

	void ContainerNode::Compile()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			(*child)->Compile();
		}
	}

	~ContainerNode()
	{
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}

};

class IdentNode : Node
{
public:
	vector<string *> *Fields;
	IdentNode(string *name, vector<string *> *fields) : Fields(fields)
	{
		Name = *name;
	}

	~IdentNode()
	{
		delete Fields;
	}
};

class StatementNode : public Node
{
public:
};

class StatementsNode : public ContainerNode<StatementsNode, StatementNode>
{
public:
};

class ModuleNode : public StatementNode
{
public:
	StatementsNode *Statements;
	ModuleNode(string *name, StatementsNode *statements) : Statements(statements)
	{
		Name = *name;
		statements->Parent = this;
	}

	void Compile();

	~ModuleNode()
	{
		delete Statements;
	}
};

class ParameterNode : public Node
{
public:
	ParameterNode(string *name) { Name = *name; }
};

class ParametersNode : public ContainerNode<ParametersNode, ParameterNode>
{
public:
};

class OutputNode : public Node
{
public:
};

class OutputsNode : public ContainerNode<OutputsNode, OutputNode>
{
public:
};

class FunctionDeclNode : public StatementNode
{
public:
	ParametersNode *Parameters;
	StatementsNode *Statements;
	FunctionDeclNode(string *name, ParametersNode *parameters, StatementsNode *statements) : Parameters(parameters), Statements(statements)
	{
		Name = *name;
		parameters->Parent = this;
		statements->Parent = this;
	}

	void Compile();

	~FunctionDeclNode()
	{
		delete Parameters;
		delete Statements;
	}
};


#endif