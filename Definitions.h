#ifndef Definitions_h
#define Definitions_h

#include "Global.h"

#include "Hash.h"

struct char_ptr_hasher : public std::unary_function<const char *, size_t>
{
	size_t operator()(const char* _Keyval) const {
		uint32_t result[4];
		MurmurHash3_128(_Keyval, strlen(_Keyval), 0xB16B00B5 + 'F' + 'u' + 'n' + 'k', &result);
		return *result;
	}
};

struct char_ptr_equals : public std::binary_function<const char *, const char *, bool>
{
	bool operator()(const char * _Left, const char *_Right) const {
		return strcmp(_Left, _Right) == 0;
	}
};

extern unordered_map<string, string> Definitions;
extern unordered_map<const char *, char *, char_ptr_hasher, char_ptr_equals> DefinitionsHashed;

extern vector<char *>buffers;

char *LoadDefinitionsFile(string filePath);

int ParseNumber(char *ptr);

#endif Definitions_h