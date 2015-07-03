%{
extern "C" int yylex();
extern "C" int yyparse();
extern char *yytext;

void yyerror(const char *s);

#define YYERROR_VERBOSE
#define YYSTYPE Node *
#include "AST.h"

%}

%debug

%token SHL
%token SHR
%token LE
%token EQEQ
%token NE
%token GE
%token ANDAND
%token OROR
%token SHLEQ
%token SHREQ
%token MINUSEQ
%token MINUSMINUS
%token ANDEQ
%token OREQ
%token PLUSEQ
%token PLUSPLUS
%token STAREQ
%token SLASHEQ
%token CARETEQ
%token PERCENTEQ
%token DOTDOT
%token DOTDOTDOT
%token MOD_SEP
%token RARROW
%token FAT_ARROW

%token LIT_CHARS
%token LIT_INTEGER
%token LIT_STR
%token NAME

// Keywords
%token LET
%token IF
%token ELIF
%token ELSE
%token MATCH
%token LOOP
%token END
%token RETURN

%token FN
%token MOD
%token ENUM
%token STRUCT

%token TRUE
%token FALSE

// Types
%token BYTE
%token WORD
%token BOOL
%token REG8
%token REG16

// Start
%start program

//%right '='
%left  '+' '-'
%left  '*' '/'

%%

////////////////////////////////////////////////////////////////////////
// Part 1: Items and attributes
////////////////////////////////////////////////////////////////////////

program
: items_maybe		{ $$ = new ProgramNode(); }
;

items_maybe
: items
| /* empty */
;

items
: item				{ $$ = $$.Extend($1); }
| items item		{ $$ = $1.Extend($2); }
;

item
: fn
| mod
;

fn
: FN name fn_params_maybe { $$ = new FunctionNode($2); delete $2;}
;
fn_params_maybe
: '(' fn_params ')'		{ $$ = $2 }
| /* empty */
;
fn_params
: name					{ $$ = new ParametersNode($1); delete $1; }
| fn_params ',' name	{ $$ = $$.Extend($3); delete $3; }
;

mod
: MOD name	{ $$ = new ModuleNode($2); delete $2; }
;

name
: NAME					{ $$ = new NameNode($1); }
;

%%
