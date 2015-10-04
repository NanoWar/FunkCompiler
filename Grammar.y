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
 ExpressionsNode *exprs;
 ExpressionNode *expr;
 FunctionCallExpr *fn_call;
 ERegister reg;
 IdentExpr *ident;
 IdentRegExpr *ident_reg;
 StatementsNode *stmts;
 StatementNode *stmt;
 ModuleNode *mod;
 FunctionDeclNode *fn_decl;
 ParametersNode *fn_decl_params;
 ParameterNode *fn_decl_param;
 ResultsNode *fn_decl_results;
 ResultNode *fn_decl_result;
 SaveListNode *save_list;
}

%debug
%locations

%type <str> name
%type <ident> ident
%type <reg> reg
%type <ident_reg> ident_reg
%type <stmts> stmts program else
%type <stmt> stmt asm mod fn_decl assign if save returns
%type <exprs> exprs
%type <expr> expr
%type <fn_call> fn_call
%type <fn_decl_params> fn_decl_params
%type <fn_decl_param> fn_decl_param
%type <fn_decl_results> fn_decl_results_maybe fn_decl_results
%type <fn_decl_result> fn_decl_result
%type <save_list> save_list


%token <token> SHL SHR
%token <token> LE EQEQ NE GE
%token <token> ANDAND OROR
%token <token> SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ

%token <token> MINUSMINUS PLUSPLUS
%token <token> DOTDOT DOTDOTDOT MOD_SEP
%token <token> RARROW FAT_ARROW

%token <str> tINTEGER tCHARS tSTRING NAME tASM

// Keywords
%token <token> LET IF ELIF ELSE MATCH LOOP END RETURN SAVE
%token <token> MOD FN ENUM STRUCT
%token <token> tTRUE tFALSE

// Types
%token <token> tBYTE tWORD tBOOL REG8 REG16

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
: asm
| mod
| fn_decl
| fn_call			{ $$ = new FunctionCallStmt($<fn_call>1); }
| assign
| if
| save
| returns
;

asm
: tASM						{ $$ = new AsmNode(string(yytext)); }
;

mod
: MOD name '{' stmts '}'	{ $$ = new ModuleNode($<str>2, $<stmts>4); delete $2; }
;

fn_decl
: FN name '(' fn_decl_params ')' fn_decl_results_maybe '{' stmts '}' { $$ = new FunctionDeclNode($<str>2, $<fn_decl_params>4, $<stmts>8, $<fn_decl_results>6); delete $2;}
;
fn_decl_params
: /* empty */				        { $$ = new ParametersNode(); }
| fn_decl_param						{ $$ = new ParametersNode(); $$->Extend($<fn_decl_param>1); }
| fn_decl_params ',' fn_decl_param	{ $$ = $<fn_decl_params>1->Extend($<fn_decl_param>3); }
;
fn_decl_param
: name ':' reg						{ $$ = new ParameterNode($<str>1, $<reg>3); delete $1; }
;
fn_decl_results_maybe
: /* empty */							{ $$ = new ResultsNode(); }
| RARROW fn_decl_results				{ $$ = $<fn_decl_results>2; }
;
fn_decl_results
: fn_decl_result						{ $$ = new ResultsNode(); $$->Extend($<fn_decl_result>1); }
| fn_decl_results ',' fn_decl_result	{ $$ = $<fn_decl_results>1->Extend($<fn_decl_result>3); }
;
fn_decl_result
: name ':' reg							{ $$ = new ResultNode($<str>1, $<reg>3); delete $1; }
| reg									{ $$ = new ResultNode($<reg>1); }
;

fn_call
: ident '(' exprs ')'		{ $$ = new FunctionCallExpr($<ident>1, $<exprs>3); }
;

assign
: reg '=' expr				{ $$ = new AssignStmt($<reg>1, $<expr>3); }
| ident '=' expr			{ $$ = new AssignStmt($<ident>1, $<expr>3); }
| ident_reg '=' expr		{ $$ = new AssignStmt($<ident_reg>1, $<expr>3); }
;

if
: IF expr '{' stmts '}'	else	{ $$ = new IfStmt($<expr>2, $<stmts>4, $<stmts>6); }
;

else 
: /* empty */					{ $$ = new StatementsNode(); }
| ELSE '{' stmts '}'			{ $$ = $<stmts>3; }
;

save
: SAVE save_list '{' stmts '}'	{ $$ = new SaveStmt($<save_list>2, $<stmts>4); }
;

save_list
: reg							{ $$ = new SaveListNode(); $$->Extend($<reg>1); }
| ident							{ $$ = new SaveListNode(); $$->Extend($<ident>1);}
| save_list ',' reg				{ $$ = $<save_list>1->Extend($<reg>3); }
| save_list ',' ident			{ $$ = $<save_list>1->Extend($<ident>3); }
;

returns
: RETURN exprs					{ $$ = new ReturnNode($<exprs>2); }
;


////////////////////////////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////////////////////////////

exprs
: /* empty */				{ $$ = new ExpressionsNode(); }
| expr						{ $$ = new ExpressionsNode(); $$->Extend($<expr>1); }
| exprs ',' expr			{ $$ = $<exprs>1->Extend($<expr>3); }

expr
: ident						{ $$ = $<ident>1; }
| fn_call					{ $$ = $<fn_call>1; }
| tINTEGER					{ $$ = new NumberExpr(string(yytext)); }
| tCHARS					{ $$ = new CharsExpr(string(yytext)); }
| tSTRING					{ $$ = new StringExpr(string(yytext)); }
| reg						{ $$ = new RegisterExpr($<reg>1); }
| expr '+' expr				{ $$ = new PlusExpr($<expr>1, $<expr>3); }
| expr EQEQ expr			{ $$ = new CompareExpr($<expr>1, $<expr>3); }
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
: name					{ $$ = new IdentExpr(*$<str>1); delete $1; }
| ident '.' name		{ $<ident>1->Extend($<str>3); }
;

ident_reg
: name ':' reg			{ $$ = new IdentRegExpr(*$<str>1, $<reg>3); delete $1; }
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
