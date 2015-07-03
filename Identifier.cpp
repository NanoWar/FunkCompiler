#include "Global.h"
#include "AST.h"

map<Node*, string> NodeIdentifiers = map<Node*, string>();
map<string, Node*> IdentifiableNodes = map<string, Node*>();

string Node::GetIdentifier() {
	auto id = NodeIdentifiers[this];
	if(!id.empty()) return id;

	vector<string> names;
	names.push_back(Name);
	Node *parent = Parent;
	while(parent && parent->Parent) {
		// Jump over unnamed container Nodes
		if(!parent->Name.empty()) names.push_back(parent->Name);
		parent = parent->Parent;
	}
	reverse(names.begin(), names.end());
	auto result = join(names, ".");
	NodeIdentifiers[this] = result;
	return result;
}