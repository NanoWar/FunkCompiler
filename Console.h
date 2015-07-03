#ifndef Console_h
#define Console_h

#include "AST.h"

void info (const char* format, ...);
void print (const char* format, ...);
void print_node(Node *n, int depth);


void AddConsoleCtrlHandler ();

#endif Console_h