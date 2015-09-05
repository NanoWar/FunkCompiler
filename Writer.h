#ifndef Writer_h
#define Writer_h

#include "Global.h"
#include "Register.h"

extern char *S_LOAD;
extern char *S_ADD_A;
extern char *S_DEFINE;

extern FILE *output_file;

void Write(const char* format, ...);
void WriteLn(const char* format, ...);

void WriteLoad(ERegister target, ERegister source);
void WriteLoad(ERegister target, string value);
void WriteLoad(ERegister target, int value);

void WriteDefine(string target, string value);
void WriteDefine(string target, ERegister source);

void WriteProgStart();

#endif Writer_h