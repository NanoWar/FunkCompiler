#include "Global.h"

#include "AST.h"
#include "StringHelper.h"

map<Node*, string> NodeToString = map<Node*, string>();
map<string, Node*> StringToNode = map<string, Node*>();

string Node::GetIdentifier()
{
	auto id = NodeToString[this];
	if(!id.empty()) return id;

	vector<string> names;
	names.push_back(Name);
	Node *parent = Parent;
	while(parent) {
		// Jump over unnamed container Nodes
		if(!parent->Name.empty()) names.push_back(parent->Name);
		parent = parent->Parent;
	}
	reverse(names.begin(), names.end());
	auto result = join(names, ".");
	NodeToString[this] = result;
	StringToNode[result] = this; // ?
	return result;
}
