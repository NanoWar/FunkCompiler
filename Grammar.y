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
 string *str;
 IdentNode *ident;
 StatementsNode *stmts;
 StatementNode *stmt;
 ExpressionsNode *exprs;
 ExpressionNode *expr;
 ParametersNode *fn_params;
 ParameterNode *fn_param;
 FunctionDeclNode *fn;
 ModuleNode *mod;
 ERegister reg;
}

%debug

%type <str> name
%type <ident> ident
%type <stmts> stmts program
%type <stmt> stmt fn mod assign call
%type <exprs> exprs
%type <expr> expr
%type <fn_params> fn_params
%type <fn_param> fn_param
%type <reg> reg


%token <token> SHL SHR
%token <token> LE EQEQ NE GE
%token <token> ANDAND OROR
%token <token> SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ

%token <token> MINUSMINUS PLUSPLUS
%token <token> DOTDOT DOTDOTDOT MOD_SEP
%token <token> RARROW FAT_ARROW

%token <str> TINTEGER
%token <str> TCHARS TSTRING NAME

// Keywords
%token <token> LET IF ELIF ELSE MATCH LOOP END RETURN
%token <token> FN MOD ENUM STRUCT
%token <token> TRUE FALSE

// Types
%token <token> BYTE WORD BOOL REG8 REG16

%token <reg> REG_A REG_F REG_B REG_C REG_D REG_E REG_H REG_L REG_IXH REG_IXL REG_IYH REG_IYL REG_I REG_R
%token <reg> REG_AF REG_BC REG_DE REG_HL REG_IX REG_IY REG_AFS

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
| call
;

fn
: FN name '(' fn_params ')' '{' stmts '}' { $$ = new FunctionDeclNode($<str>2, $<fn_params>4, $<stmts>7); delete $2;}
;
fn_params
: /* empty */				{ $$ = new ParametersNode(); }
| fn_param					{ $$ = new ParametersNode(); $$->Extend($<fn_param>1); }
| fn_params ',' fn_param	{ $$ = $<fn_params>1->Extend($<fn_param>3); }
;
fn_param
: name						{ $$ = new ParameterNode($<str>1); delete $1; }
;

mod
: MOD name '{' stmts '}'	{ $$ = new ModuleNode($<str>2, $<stmts>4); delete $2; }
;

assign
: reg '=' expr				{ $$ = new AssignStmt($<reg>1, $<expr>3); }
| name '=' expr				{ $$ = new AssignStmt($<str>1, $<expr>3); }
;

call
: ident '(' exprs ')'		{ $$ = new FunctionCallStmt($<ident>1, $<exprs>3); delete $1; }
;



////////////////////////////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////////////////////////////

exprs
: /* empty */				{ $$ = new ExpressionsNode(); }
| expr						{ $$ = new ExpressionsNode(); $$->Extend($<expr>1); }
| exprs ',' expr			{ $$ = $<exprs>1->Extend($<expr>3); }

expr
: name						{ $$ = new IdentExpr(*$<str>1); delete $1; }
| TINTEGER					{ $$ = new NumberExpr(string(yytext)); }
| TCHARS					{ $$ = new CharsExpr(string(yytext)); }
| TSTRING					{ $$ = new StringExpr(string(yytext)); }
| reg						{ $$ = new RegisterExpr($<reg>1); }
| expr '+' expr				{ $$ = new PlusExpr($<expr>1, $<expr>3); }
| '*' expr					{ $$ = new IndirectionExpr($<expr>2); }
/*
| expr '-' expr				{ $$ = mk_node("Minus", 2, $1, $3); }
| expr '*' expr				{ $$ = mk_node("Mul", 2, $1, $3); }
| expr '/' expr				{ $$ = mk_node("Div", 2, $1, $3); }
| expr EQEQ expr			{ $$ = mk_node("Equals", 2, $1, $3); }
| expr NE expr				{ $$ = mk_node("Unequals", 2, $1, $3); }
| expr '>' expr				{ $$ = mk_node("Greater", 2, $1, $3); }
| expr '<' expr				{ $$ = mk_node("Smaller", 2, $1, $3); }
*/
| '(' expr ')'				{ $$ = $<expr>2; }
;

////////////////////////////////////////////////////////////////////////
// IDENTIFIERS
////////////////////////////////////////////////////////////////////////

name
: NAME					{ $$ = new string(yytext); }
;

ident
: name					{ $$ = new IdentNode($<str>1); delete $1; }
| ident '.' name		{ $<ident>1->Extend($<str>3); }
;

reg
: REG_A { $$ = ERegister::A; }
| REG_F { $$ = ERegister::F; }
| REG_B { $$ = ERegister::B; }
| REG_C { $$ = ERegister::C; }
| REG_D { $$ = ERegister::D; }
| REG_E { $$ = ERegister::E; }
| REG_H { $$ = ERegister::H; }
| REG_L { $$ = ERegister::L; }
| REG_IXH { $$ = ERegister::IXH; }
| REG_IXL { $$ = ERegister::IXL; }
| REG_IYH { $$ = ERegister::IYH; }
| REG_IYL { $$ = ERegister::IYL; }
| REG_I { $$ = ERegister::I; }
| REG_R { $$ = ERegister::R; }
| REG_AF { $$ = ERegister::AF; }
| REG_BC { $$ = ERegister::BC; }
| REG_DE { $$ = ERegister::DE; }
| REG_HL { $$ = ERegister::HL; }
| REG_IX { $$ = ERegister::IX; }
| REG_IY { $$ = ERegister::IY; }
;

%%
