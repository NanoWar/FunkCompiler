#ifndef Global_h
#define Global_h

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

extern int verbose;
extern int quiet;
extern int repl;
extern int no_colors;
extern string input_file_name;
extern string input_file_ext;
extern FILE *input_file;
extern FILE *output_file;
extern int errors;

#endif Global_h