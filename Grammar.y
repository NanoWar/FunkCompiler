%{
extern "C" int yylex();
extern "C" int yyparse();
extern char *yytext;

void yyerror(const char *s);

#include "Global.h"
#include "AST.h"
#include "Register.h"

#define YYERROR_VERBOSE

extern StatementsNode *Program;

%}

%union {
 void *unknown;
 int token;
 string *name;
 StatementsNode *stmts;
 StatementNode *stmt;
 ParametersNode *params;
 ParameterNode *param;
 FunctionDeclNode *fn;
 ModuleNode *mod;
 ERegister8 reg8;
 ERegister16 reg16;
}

%debug

%type <stmts> stmts program
%type <stmt> stmt fn mod assign
%type <params> params
%type <param> param
%type <name> name
%type <reg8> reg8
%type <reg16> reg16


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

%token <reg8> REG_A REG_F REG_B REG_C REG_D REG_E REG_H REG_L REG_IXH REG_IXL REG_I REG_R REG_IYH REG_IYL
%token <reg16> REG_AF REG_BC REG_DE REG_HL REG_IX REG_IY

//%right '='
%left  '+' '-'
%left  '*' '/'

// Start
%start program

%%

////////////////////////////////////////////////////////////////////////
// ENTRY
////////////////////////////////////////////////////////////////////////

program
: stmts				{ Program = $<stmts>1; }
;

////////////////////////////////////////////////////////////////////////
// STATEMENTS
////////////////////////////////////////////////////////////////////////

stmts
: /* empty */		{ $$ = new StatementsNode(); }
| stmt				{ $$ = new StatementsNode(); $$->Extend($<stmt>1); }
| stmts stmt		{ $1->Extend($<stmt>2); }
;

stmt
: fn				/*{ $$ = $<stmt>1; }*/
| mod				/*{ $$ = $<stmt>1; }*/
| assign
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

assign
: reg8 '=' reg8				{ $$ = new AssignNode($<reg8>1, $<reg8>3); }
| reg16 '=' reg16			{ $$ = new AssignNode($<reg16>1, $<reg16>3); }
;




////////////////////////////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////
// IDENTIFIERS
////////////////////////////////////////////////////////////////////////

name
: NAME					{ $$ = new string(yytext); }
;

reg8
: REG_A { $$ = ERegister8::A; }
| REG_F { $$ = ERegister8::F; }
| REG_B { $$ = ERegister8::B; }
| REG_C { $$ = ERegister8::C; }
| REG_D { $$ = ERegister8::D; }
| REG_E { $$ = ERegister8::E; }
| REG_H { $$ = ERegister8::H; }
| REG_L { $$ = ERegister8::L; }
| REG_IXH { $$ = ERegister8::IXH; }
| REG_IXL { $$ = ERegister8::IXL; }
| REG_IYH { $$ = ERegister8::IYH; }
| REG_IYL { $$ = ERegister8::IYL; }
| REG_I { $$ = ERegister8::I; }
| REG_R { $$ = ERegister8::R; }
;
reg16
: REG_AF { $$ = ERegister16::AF; }
| REG_BC { $$ = ERegister16::BC; }
| REG_DE { $$ = ERegister16::DE; }
| REG_HL { $$ = ERegister16::HL; }
| REG_IX { $$ = ERegister16::IX; }
| REG_IY { $$ = ERegister16::IY; }
;

%%
