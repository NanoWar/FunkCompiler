#ifndef AST_h
#define AST_h

#include "StringHelper.h"
#include <typeinfo>
#include <map>

class Node;
class ProgramNode;

extern map<Node*, string> NodeIdentifiers;
extern map<string, Node*> IdentifiableNodes;

class Node
{
public:
	string Name;
	int SourceLine;
	Node *Parent;

	Node() : Parent(NULL) { Name = ""; }

	string GetIdentifier();
};

template<typename OwnType, typename ItemType, typename BaseType = Node>
class ContainerNode : public BaseType
{
public:
	vector<ItemType*> Children;

	ContainerNode() {}
	ContainerNode(ItemType *item) { Children.push_back(item); }

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
		for(int i = 0; i < n; i++) {
			auto node = va_arg(params, ItemType *);
			Children.push_back(node);
			((Node *) node)->Parent = this;
		}
		va_end(params);
		return(OwnType *) this;
	}

	~ContainerNode() {
		for(auto child = Children.begin(); child != Children.end(); ++child) {
			delete *child;
		}
	}

};

class NameNode : public Node
{
public:
	NameNode(string name) { Name = name; }
};

class FieldsNode : public ContainerNode<FieldsNode, NameNode>
{
public:
	FieldsNode(NameNode *nameNode) : ContainerNode(nameNode) {}
};

class IdentNode : public Node
{
public:
	FieldsNode *Fields;
	IdentNode(NameNode *nameNode, FieldsNode *fieldsNode) : Fields(fieldsNode) { Name = nameNode->Name; }
};

class ItemNode : public Node
{
public:
};

class ProgramNode : public ContainerNode<ProgramNode, ItemNode>
{
public:
};

class ModuleNode : public ContainerNode<ModuleNode, ItemNode, ItemNode>
{
public:
	ModuleNode(NameNode *nameNode) { Name = nameNode->Name; }
};

class ParameterNode : public Node
{
public:
	ParameterNode(NameNode *nameNode) { Name = nameNode->Name; }
};

class ParametersNode : public ContainerNode<ParametersNode, ParameterNode>
{
public:
};

class OutputNode : public Node
{
public:
};

class FunctionNode : public ContainerNode<ProgramNode, ItemNode, ItemNode>
{
public:
	ParametersNode *Parameters;

	FunctionNode(NameNode *nameNode, ParametersNode *parameters) : Parameters(parameters) {
		Name = nameNode->Name; 
		parameters->Parent = this;
	}
	~FunctionNode() {
		delete Parameters;
	}
};


#endif