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
