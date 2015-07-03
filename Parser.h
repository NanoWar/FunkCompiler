#ifndef Parser_h
#define Parser_h

#include <cstdio>

extern "C" int yylex();
extern "C" int yyparse();
extern FILE *yyin;
extern int yylineno;
extern int yydebug;

#endif Parser_h