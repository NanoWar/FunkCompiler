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
