%{
extern "C" int yylex();
extern "C" int yyparse();
extern char *yytext;

void yyerror(const char *s);

#define YYERROR_VERBOSE
#define YYSTYPE Node *
class Node;
extern Node *mk_node(char const *name, int n, ...);
extern Node *mk_atom(char *text);
extern Node *mk_none();
extern Node *ext_node(struct Node *nd, int n, ...);

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
%start funk

//%right '='
%left  '+' '-'
%left  '*' '/'

%%

////////////////////////////////////////////////////////////////////////
// Part 1: Items and attributes
////////////////////////////////////////////////////////////////////////

funk
: items_maybe		//{ $$ = new ProgramNode().Extend($1); }
;

items_maybe
: items
| /* empty */		{ $$ = mk_none(); }
;

items
: item				{ $$ = mk_node("Items", 1, $1); }
| items item		{ $$ = ext_node($1, 1, $2); }
;

item
: item_fn
| item_mod
;

item_fn
: FN name fn_params_maybe fn_outputs_maybe block END { $$ = mk_node("Function", 4, $2, $3, $4, $5); }
;

fn_params_maybe
: '(' param_defs_maybe ')'	{ $$ = $2; }
| /* empty */				{ $$ = mk_none(); }
;
param_defs_maybe
: param_defs
| /* empty */				{ $$ = mk_none(); }
;
param_defs
: param_def					{ $$ = mk_node("ParamDefs", 1, $1); }
| param_defs ',' param_def	{ $$ = ext_node($1, 1, $3); }
;
param_def
: ty bind_maybe prefix_maybe name	{ $$ = mk_node("ParamDef", 4, $1, $2, $3, $4); }
;

bind_maybe
: ':' register				{ $$ = $2; }
| /* empty */				{ $$ = mk_none(); }
;

prefix_maybe
: '&'						{ $$ = mk_atom("Mutable"); }
| '*'						{ $$ = mk_atom("Pointer"); }
| /* empty */				{ $$ = mk_none(); }
;

register
: ident
;

fn_outputs_maybe
: RARROW outputs			{ $$ = $2; }
| /* empty */				{ $$ = mk_none(); }
;
outputs
: output					{ $$ = mk_node("Outputs", 1, $1); }
| outputs ',' output		{ $$ = ext_node($1, 1, $3); }
;
output
: ty						{ $$ = mk_node("Output", 1, $1); }
;

item_mod
: MOD ident items_maybe END	{ $$ = mk_node("Module", 2, $2, $3); }
;

////////////////////////////////////////////////////////////////////////
// Part 2: Patterns
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Part 3: Types
////////////////////////////////////////////////////////////////////////

ty
: BYTE	{ $$ = mk_atom("TyByte"); }
| WORD	{ $$ = mk_atom("TyWord"); }
| BOOL	{ $$ = mk_atom("TyBool"); }
| ty_reg
| ident
;

ty_reg
: REG8	{ $$ = mk_atom("TyReg8"); }
| REG16	{ $$ = mk_atom("TyReg16"); }
;

////////////////////////////////////////////////////////////////////////
// Part 4: Blocks, statements, and expressions
////////////////////////////////////////////////////////////////////////

block
: stmts_maybe	{ $$ = mk_node("Block", 1, $1); }
;
stmts_maybe
: stmts
| /* empty */	{ $$ = mk_none(); }
;
stmts
: stmt           { $$ = mk_node("Statements", 1, $1); }
| stmts stmt     { $$ = ext_node($1, 1, $2); }
;

// Statements

stmt
: let
| if
| loop
| call
| return
| val PLUSEQ val			{ $$ = mk_node("PlusEquals", 2, $1, $3); }
| val MINUSEQ val			{ $$ = mk_node("MinusEquals", 2, $1, $3); }
| val PLUSPLUS				{ $$ = mk_node("PlusPlus", 1, $1); }
| val MINUSMINUS			{ $$ = mk_node("MinusMinus", 1, $1); }
;

let
: LET ident ty_ascription_maybe init_expr_maybe { $$ = mk_node("DeclLocal", 3, $2, $3, $4); }
;
ty_ascription_maybe
: ':' ty		{ $$ = $2; }
| /* empty */	{ $$ = mk_none(); }
;
init_expr_maybe
: '=' expr		{ $$ = $2; }
| /* empty */	{ $$ = mk_none(); }
;

if
: IF expr block	elifs_maybe else_maybe END	{ $$ = mk_node("If", 4, $2, $3, $4, $5); }
;
elifs_maybe
: elifs
| /* empty */				{ $$ = mk_none(); }
;
elifs
: elif						{ $$ = mk_node("Elifs", 1, $1); }
| elifs elif				{ $$ = ext_node($1, 1, $2); }
;
elif
: ELIF expr block			{ $$ = mk_node("Elif", 2, $2, $3); }
;
else_maybe
: else
| /* empty */				{ $$ = mk_none(); }
;
else
: ELSE block				{ $$ = mk_node("Else", 1, $2); }
;

loop
: LOOP ident block END		{ $$ = mk_node("Loop", 2, $2, $3); }
;

call
: ident '(' call_params_maybe ')'	{ $$ = mk_node("FunctionCall", 2, $1, $3); }
;
call_params_maybe
: call_params
| /* empty */				{ $$ = mk_none(); }
;
call_params
: ident						{ $$ = mk_node("Params", 1, $1); }
| call_params ',' ident		{ $$ = ext_node($1, 1, $3); }
;

return
: RETURN expr_maybe			{ $$ = mk_node("Return", 1, $2); }
;



// Expressions

expr_maybe
: expr
| /* empty */	{ $$ = mk_none(); }
;

expr
: call
| val
| '*' expr					{ $$ = mk_node("Indirection", 1, $2); }
| expr '+' expr				{ $$ = mk_node("Plus", 2, $1, $3); }
| expr '-' expr				{ $$ = mk_node("Minus", 2, $1, $3); }
| expr '*' expr				{ $$ = mk_node("Mul", 2, $1, $3); }
| expr '/' expr				{ $$ = mk_node("Div", 2, $1, $3); }
| expr EQEQ expr			{ $$ = mk_node("Equals", 2, $1, $3); }
| expr NE expr				{ $$ = mk_node("Unequals", 2, $1, $3); }
| expr '>' expr				{ $$ = mk_node("Greater", 2, $1, $3); }
| expr '<' expr				{ $$ = mk_node("Smaller", 2, $1, $3); }
| '(' expr ')'				{ $$ = $2; }
;

////////////////////////////////////////////////////////////////////////

val
: lit
| ident
;

lit
: LIT_INTEGER				{ $$ = mk_node("Integer", 1, mk_atom(yytext)); }
| '-' LIT_INTEGER			{ $$ = mk_node("NegInteger", 1, mk_atom(yytext)); }
| LIT_CHARS					{ $$ = mk_node("Char", 1, mk_atom(yytext)); }
| TRUE						{ $$ = mk_node("Bool", 1, mk_atom(yytext)); }
| FALSE						{ $$ = mk_node("Bool", 1, mk_atom(yytext)); }
| str
;

str
: LIT_STR					{ $$ = mk_node("String", 1, mk_atom(yytext)); }
;

ident_maybe
: ident
| /* empty */				{ $$ = mk_none(); }
;

ident
: name fields_maybe			{ $$ = mk_node("Ident", 2, $1, $2); }
;

fields_maybe
: '.' fields				{ $$ = $2; }
| /* empty */				{ $$ = mk_none(); }
;

fields
: name						{ $$ = mk_node("Fields", 1, $1); }
| fields '.' name			{ $$ = ext_node($1, 1, $3); }
;

name
: NAME						{ $$ = mk_node("Name", 1, mk_atom(yytext)); }
;

%%
