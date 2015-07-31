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
		if(!parent->Name.empty()) {
			names.push_back(parent->Name);
		}
		parent = parent->Parent;
	}
	reverse(names.begin(), names.end());
	auto result = join(names, ".");
	NodeToString[this] = result;
	StringToNode[result] = this;
	return result;
}

string IdentNode::GetIdentifier()
{
	// Build own name
	vector<string> names;
	names.push_back(Name);
	for(auto it = Children.begin(); it != Children.end(); ++it) {
		names.push_back(**it);
	}
	string name = join(names, ".");

	// Search for matches up the tree
	reverse(names.begin(), names.end()); // make order upwards hierarchical
	Node *parent = Parent;
	Node *node;
	string result;
	while(parent) {
		reverse(names.begin(), names.end()); // down
		result = join(names, ".");
		reverse(names.begin(), names.end()); // up
		node = StringToNode[result];
		if(node != NULL) break;
		// Jump over unnamed container Nodes
		if(!parent->Name.empty()) {
			if(dynamic_cast<ModuleNode*>(parent)) {
				names.push_back(parent->Name);
			}
		}
		parent = parent->Parent;
	}
	if(node != NULL) {
		result = NodeToString[node];
		if(result.empty()) {
			// TODO: Log error
			result = name;
		}
	}
	else {
		// TODO: Log error
		result = name;
	}
	return result;
}
