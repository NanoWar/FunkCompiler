#include "Global.h"
#include "Hash.h"

#include "Definitions.h"
#include "StringHelper.h"
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
unordered_map<const char *, const char *, char_ptr_hasher, char_ptr_equals> DefinitionsHashed = unordered_map<const char *, const char *, char_ptr_hasher, char_ptr_equals>();

int ParseBuffer(char *file_begin, char *file_end)
{
	int found = 0;
	char *ptr = file_begin,
		*line_start, *line_end,
		*key_start, *key_end,
		*equ_start, *equ_end,
		*value_start, *value_end;
	while(ptr < file_end)
	{
		line_start = ptr;
		line_end = SkipLine(line_start);
		ptr = line_end + 1;
		key_start = SkipWhiteSpace(line_start);
		key_end = SkipWord(key_start);
		if (key_end >= line_end) continue;
		equ_start = SkipWhiteSpace(key_end);
		equ_end = SkipEqu(equ_start);
		if (equ_end >= line_end) continue;
		if (equ_start == equ_end) continue;
		if (!(*equ_start == '=' || stricmp(equ_start, "equ"))) continue;
		value_start = SkipWhiteSpace(equ_end);
		value_end = SkipWord(value_start);
		if (value_end > line_end) continue;
		// Terminate strings
		*key_end = *value_end = 0;
		DefinitionsHashed[key_start] = value_start;
		found++;
	}
	Trace("Found %d definitions", found, DefinitionsHashed.size());
	return found;
}

char *LoadDefinitionsFile(string filePath)
{
	ifstream file(filePath);
	if(!file.is_open()) {
		Error("Cannot open file <%s>", filePath.c_str());
		return NULL;
	}
	if (!file.good()) {
		Error("Cannot read file <%s>", filePath.c_str());
		return NULL;
	}

	// Read file into buffer
	int length = file.seekg(0, file.end).tellg();
	char *buffer = new char[length+1];
	file.seekg(0, file.beg);
	file.read(buffer, length);
	if (!file.eof()) Error("File <%s> could not be read entirely", filePath.c_str());
	length = file.gcount();
	file.close();
	// Terminate buffer
	buffer[length] = 0;
	// Parse contents
	ParseBuffer(buffer, buffer + length -1);
	return buffer;
}
