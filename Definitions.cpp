#include "Global.h"

#include "Definitions.h"
#include "Console.h"
#include <fstream>
#include <sstream>

#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

unordered_map<string, string> Definitions = unordered_map<string, string>();

void LoadDefinitionsFile(string filePath)
{
	ifstream file(filePath);
	if(!file.is_open()) {
		error("Cannot open file <%s>\n", filePath.c_str());
		return;
	}
	if (!file.good()) {
		error("Cannot read file <%s>\n", filePath.c_str());
		return;
	}
	
	string line, key, value, tmp;
	while(getline(file, line)) {
		// Remove comments (assume no escaping)
		auto comment = line.find(';');
		if(comment != string::npos) {
			line.erase(comment, string::npos);
		}
		istringstream lss(line);
		lss >> key >> tmp >> value;
		if(key.empty()) continue;
		if(key[0] == '#') continue;
		if(key[0] == '.') continue;
		if (strcmp(tmp.c_str(), "=") == 0
		|| stricmp(tmp.c_str(), "equ") == 0) {
			// Override if necessary
			Definitions[key] = value;
		}
	}
}
