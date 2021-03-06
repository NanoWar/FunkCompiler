#ifndef Parser_h
#define Parser_h

#include <cstdio>

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	string file_name;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 0
#endif

extern "C" int yylex();
extern "C" int yyparse(string file_name);
extern FILE *yyin;
extern int yylineno;
extern YYLTYPE yylloc;
extern char *yytext;
extern unsigned int yyleng;
extern int yydebug;

#endif Parser_h