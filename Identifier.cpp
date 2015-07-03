#include "Global.h"
#include "AST.h"

map<NodeBase*, string> NodeIdentifiers = map<NodeBase*, string>();
map<string, NodeBase*> IdentifiableNodes = map<string, NodeBase*>();

string NodeBase::GetIdentifier() {
	auto id = NodeIdentifiers[this];
	if(!id.empty()) return id;

	vector<string> names;
	names.push_back(Name);
	NodeBase *parent = Parent;
	while(parent && parent->Parent) {
		// Jump over unnamed container nodes
		if(!parent->Name.empty()) names.push_back(parent->Name);
		parent = parent->Parent;
	}
	reverse(names.begin(), names.end());
	auto result = join(names, ".");
	NodeIdentifiers[this] = result;
	return result;
}