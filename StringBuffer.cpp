#include "Global.h"
#include "Writer.h"

#include "StringBuffer.h"

map<string, string> CurrentStrings = map<string, string>();
vector<map<string, string>> PagedStrings = vector<map<string, string>>();
int CurrentPage = 0;
int StringCount = 0;

void Check()
{
	if (PagedStrings.size() <= CurrentPage) {
		PagedStrings.push_back(CurrentStrings);
		// TODO
	}
}

string AddString(string str)
{
	auto ident = CurrentStrings[str];
	if (!ident.empty()) return ident;

	// Add to list
	stringstream ss;
	ss << "_str_" << ++StringCount;
	CurrentStrings[str] = ss.str();
	Check();
	return ss.str();
}

void CompileCurrentStrings()
{
	if (CurrentStrings.size() == 0) {
		return;
	}
	WriteLn("\n;==============================\n; Strings for page %d\n;==============================", CurrentPage);
	for (auto it = CurrentStrings.begin(); it != CurrentStrings.end(); ++it) {
		WriteLn("%s: .db \"%s\", 0", it->second.c_str(), it->first.c_str());
	}
}
