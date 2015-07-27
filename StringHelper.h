#ifndef StringHelper_h
#define StringHelper_h

#include "Global.h"

string join(vector<string> strings, string delimiter);
string join(string delimiter, int n, ...);
const char *join (vector<const char *> strings, const char delimiter);
const char *join (vector<const char *> strings, const char *delimiter);

#endif StringHelper_h