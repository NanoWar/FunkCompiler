#ifndef AST_h
#define AST_h

class Node
{
public:
	// Value
	bool own_string;
	char const *type;

	// Navigation
	Node *next;
	Node *prev;
	Node *parent;

	// Real value
	bool is_proxy;
	const char *proxy;
	const char *format;
	bool has_value;
	int value;

	// Children
	int n_elems;
	Node *elems[];
};

#include "StringHelper.h"
#include <typeinfo>
#include <map>

class NodeBase;
class ProgramNode;

extern map<NodeBase*, string> NodeIdentifiers;
extern map<string, NodeBase*> IdentifiableNodes;

class NodeBase
{
public:
	string Name;
	int SourceLine;
	NodeBase *Parent;

	NodeBase() : Parent(NULL) { Name = ""; }

	string GetIdentifier();
};

template<typename OwnType, typename ItemType, typename BaseType = NodeBase>
class ContainerNode : public BaseType
{
public:
	vector<ItemType*> Children;

	ContainerNode() {}
	ContainerNode(ItemType *item) { Children.push_back(item); }

	OwnType *Extend(ItemType *node)
	{
		Children.push_back(node);
		((NodeBase *) node)->Parent = this;
		return(OwnType *) this;
	}

	OwnType *Extend(int n, ...)
	{
		va_list params;
		va_start(params, n);
		for(int i = 0; i < n; i++) {
			auto node = va_arg(params, ItemType *);
			Children.push_back(node);
			((NodeBase *) node)->Parent = this;
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

class NameNode : public NodeBase
{
public:
	NameNode(string name) { Name = name; }
};

class FieldsNode : public ContainerNode<FieldsNode, NameNode>
{
public:
	FieldsNode(NameNode *nameNode) : ContainerNode(nameNode) {}
};

class IdentNode : public NodeBase
{
public:
	FieldsNode *Fields;
	IdentNode(NameNode *nameNode, FieldsNode *fieldsNode) : Fields(fieldsNode) { Name = nameNode->Name; }
};

class ItemNode : public NodeBase
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

class ParameterNode : public NodeBase
{
public:
	ParameterNode(NameNode *nameNode) { Name = nameNode->Name; }
};

class ParametersNode : public ContainerNode<ParametersNode, ParameterNode>
{
public:
};

class OutputNode : public NodeBase
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