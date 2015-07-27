#ifndef Writer_h
#define Writer_h

#include "Global.h"
#include "Register.h"

extern char *S_LOAD;
extern char *S_ADD_A;
extern char *S_DEFINE;

extern FILE *output_file;
void write(const char* format, ...);

void WriteLoad(ERegister target, ERegister source);
void WriteLoad(ERegister target, string value);

void WriteDefine(string target, string value);
void WriteDefine(string target, ERegister source);

#endif Writer_h