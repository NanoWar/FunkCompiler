%{
extern "C" int yylex();
extern "C" int yyparse();
extern char *yytext;

void yyerror(const char *s);

#include "Global.h"
#include "AST.h"

#define YYERROR_VERBOSE

extern StatementsNode *Program;

%}

%union {
 int token;
 string *name;
 StatementsNode *stmts;
 StatementNode *stmt;
 ParametersNode *params;
 ParameterNode *param;
 FunctionDeclNode *fn;
 ModuleNode *mod;
}

%debug

%type <stmts> stmts program
%type <stmt> stmt fn mod
%type <params> params
%type <param> param
%type <name> name


%token <token> SHL SHR
%token <token> LE EQEQ NE GE
%token <token> ANDAND OROR
%token <token> SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ

%token <token> MINUSMINUS PLUSPLUS
%token <token> DOTDOT DOTDOTDOT MOD_SEP
%token <token> RARROW FAT_ARROW

%token <name> TINTEGER
%token <name> TCHARS TSTRING NAME

// Keywords
%token <token> LET IF ELIF ELSE MATCH LOOP END RETURN
%token <token> FN MOD ENUM STRUCT
%token <token> TRUE FALSE

// Types
%token <token> BYTE WORD BOOL REG8 REG16

//%right '='
%left  '+' '-'
%left  '*' '/'

// Start
%start program

%%

////////////////////////////////////////////////////////////////////////
// Part 1: Items and attributes
////////////////////////////////////////////////////////////////////////

program
: stmts				{ Program = $<stmts>1; }
;

stmts
: /* empty */		{ $$ = new StatementsNode(); }
| stmt				{ $$ = new StatementsNode(); $$->Extend($<stmt>1); }
| stmts stmt		{ $1->Extend($<stmt>2); }
;

stmt
: fn				/*{ $$ = $<stmt>1; }*/
| mod				/*{ $$ = $<stmt>1; }*/
;

fn
: FN name '(' params ')' '{' stmts '}' { $$ = new FunctionDeclNode($<name>2, $<params>4, $<stmts>7); delete $2;}
;
params
: /* empty */			{ $$ = new ParametersNode(); }
| param					{ $$ = new ParametersNode(); $$->Extend($<param>1); }
| params ',' param		{ $$ = $<params>1->Extend($<param>3); }
;
param
: name					{ $$ = new ParameterNode($<name>1); delete $1; }
;

mod
: MOD name '{' stmts '}'	{ $$ = new ModuleNode($<name>2, $<stmts>4); delete $2; }
;

name
: NAME					{ $$ = new string(yytext); }
;

%%
