#include "Global.h"

#include "StringHelper.h"

string join(vector<string> strings, string delimiter)
{
	stringstream result;
	for (auto str = strings.begin(); str != strings.end(); ) {
		result << *str;
		if (++str != strings.end()) result << '.';
	}
	return result.str();
}

string join(string delimiter, int n, ...)
{
	va_list args;
	va_start(args, n);
	vector<string> s;
	for(int i = 0; i < n; i++)
	{
		s.push_back(va_arg(args, string));
	}
	va_end(args);
	return join(s, delimiter);
}

const char *join (vector<const char *> strings, const char delimiter)
{
	int len = 0;
	for (auto str = strings.begin(); str != strings.end(); ++str) {
		len += strlen (*str) + 1;
	}
	if (!len) return NULL;

	auto result = new char[len];
	auto ptr = result;
	for (auto str = strings.begin(); str != strings.end(); ++str) {
		strcpy (ptr, *str);
		ptr += strlen (*str);
		*ptr++ = delimiter;
	}
	*--ptr = '\0';
	return result;
}

const char *join (vector<const char *> strings, const char *delimiter)
{
	int len = 0;
	int delimiter_len = strlen (delimiter);
	for (auto str = strings.begin(); str != strings.end(); ++str) {
		len += strlen (*str) + delimiter_len;
	}
	if (!len) return NULL;
	len = len - delimiter_len + 1; 

	auto result = new char[len];
	auto ptr = result;
	for (auto str = strings.begin(); str != strings.end(); ++str) {
		strcpy (ptr, *str);
		ptr += strlen (*str);
		strcpy (ptr, delimiter);
		ptr += delimiter_len;
	}
	*(ptr-delimiter_len) = '\0';
	return result;
}

bool endsWith(string const &fullString, string const &ending)
{
    if (fullString.length() < ending.length()) return false;
	return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
}


// Returns pointer after last slash
char *SkipFolders(char *ptr)
{
	char *name_start = ptr;
	while(*ptr) {
		if(*ptr == '\\' || *ptr == '/') {
			name_start = ptr + 1;
		}
		ptr++;
	}
	return name_start;
}

// Returns pointer after last dot
char *SkipFileName(char *ptr)
{
	char *ext_start = ptr;
	while(*ptr) {
		if(*ptr == '.') {
			ext_start = ptr + 1;
		}
		ptr++;
	}
	return ext_start;
}


char *SkipWhiteSpace(char *ptr)
{
	while(*ptr == ' ' || *ptr == '\t') ptr++;
	return ptr;
}

char *SkipLine(char *ptr)
{
	while(*ptr != '\0' && *ptr != '\n') ptr++;
	return ptr;
}

char *SkipWord(char *ptr)
{
	while(*ptr != '\0' && (*ptr == '_' || isalnum(*ptr))) ptr++;
	return ptr;
}

char *SkipLineTillComment(char *ptr)
{
	while(*ptr != '\0' && *ptr != '\n' && *ptr != ';') ptr++;
	return ptr;
}

char *SkipEqu(char *ptr)
{
	while(*ptr != '\0' && (*ptr == '=' || isalnum(*ptr))) ptr++;
	return ptr;
}

