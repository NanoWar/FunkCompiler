#include "Global.h"

#include "AST.h"
#include "StringHelper.h"

map<Node*, string> NodeToString = map<Node*, string>();
map<string, Node*> StringToNode = map<string, Node*>();

string Node::GetIdentifier()
{
	auto id = NodeToString[this];
	if(!id.empty()) return id;

	vector<string> path;
	path.push_back(Name);
	Node *parent = Parent;
	while(parent) {
		// Jump over unnamed container Nodes
		if(!parent->Name.empty()) {
			path.push_back(parent->Name);
		}
		parent = parent->Parent;
	}
	reverse(path.begin(), path.end());
	auto result = join(path, ".");
	NodeToString[this] = result;
	return result;
}

// Helper
vector<string> GetNames(IdentExpr *node, bool includeOwn)
{
	vector<string> path;
	path.push_back(node->Name);
	for (int i = 0; i < (int) node->Children.size() - (includeOwn ? 0 : 1); i++)
	{
		path.push_back(*(node->Children.at(i)));
	}
	return path;
}

string IdentExpr::GetName(bool includeOwn)
{
	string name = join(GetNames(this, includeOwn), ".");
	return name;
}

Node *IdentExpr::GetReferenced(bool includeOwn)
{
	// Build own name and copy
	vector<string> names = GetNames(this, includeOwn);
	string name = join(names, ".");
	vector<string> path(names.begin(), names.end()); // copy

	// Search for matches up the tree
	reverse(path.begin(), path.end()); // make order upwards hierarchical
	Node *parent = Parent;
	Node *node = NULL;
	string result;
	while (parent)
	{
		// Search for complete path
		reverse(path.begin(), path.end()); // down
		result = join(path, ".");
		reverse(path.begin(), path.end()); // up
		node = StringToNode[result];
		if (node != NULL) break;

		// Jump over unnamed container Nodes
		if (!parent->Name.empty())
		{
			// Search for higher matches
			result = join(".", 2, parent->Name, name);
			node = StringToNode[result];
			if (node != NULL) break;

			if (dynamic_cast<FunctionDeclNode*>(parent)) {
				path.push_back(parent->Name);
			}
			if (dynamic_cast<ModuleNode*>(parent)) {
				path.push_back(parent->Name);
			}
		}

		// Next
		parent = parent->Parent;
	}

	// Try parallel paths
	if(node == NULL && path.size() > names.size())
	{
		reverse(path.begin(), path.end()); // down
		// For each path segment
		vector<string> progress;
		for(int n = 0; n < path.size() - names.size(); ++n)
		{
			progress.push_back(path.at(n));
			// Concat progress and initial name
			vector<string> copy(progress.begin(), progress.end());
			copy.insert(copy.end(), names.begin(), names.end());
			string result = join(copy, ".");
			node = StringToNode[result];
			if (node != NULL) break;
		}
	}

	return node;
}