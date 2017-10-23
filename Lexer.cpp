#include "Global.h"
#include <stack>

#include "Parser.h"
#include "Lexer.h"

void before_rule()
{
	yylloc.first_line = yylloc.last_line = yylineno;
    yylloc.first_column = yylloc.last_column;
	if (*yytext == '\n')
	{
		yylloc.last_line++;
        yylloc.last_column = 1;
	}
	else {
        yylloc.last_column += yyleng;
    }
}

void after_rule()
{
}

stack<YYLTYPE> location_stack = stack<YYLTYPE>();
stack<int> line_stack = stack<int>();

void yypush_location_state(const char *new_file_name)
{
	line_stack.push(yylineno);
	yylineno = 1;

	location_stack.push(yylloc);
	yylloc.first_line = yylloc.last_line = 1;
	yylloc.first_column = yylloc.last_column = 1;
	yylloc.file_name = new_file_name;
}

void yypop_location_state()
{
	yylineno = line_stack.top();
	line_stack.pop();

	yylloc = location_stack.top();
	location_stack.pop();
}