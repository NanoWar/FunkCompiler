%{
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()
#include "AST.h"
#include "Register.h"
#include "Grammar.tab.h"
%}

%option noyywrap
%option stack
%option yylineno

%x str
%x rawstr
%x rawstr_esc_begin
%x rawstr_esc_body
%x rawstr_esc_end
%x byte
%x bytestr
%x rawbytestr
%x rawbytestr_nohash
%x pound
%x shebang_or_attr
%x ltorchar
%x linecomment
%x doc_line
%x blockcomment
%x doc_block
%x suffix

name [a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]*[\!\?]?

%%

<suffix>{name}	{ BEGIN(INITIAL); }
<suffix>(.|\n)	{ yyless(0); BEGIN(INITIAL); }

[ \t\n\r] { }

\/\* 					{ yy_push_state(blockcomment); }
<blockcomment>\*\/		{ yy_pop_state(); }
<blockcomment>(.|\n)	{ }

\/\/					{ BEGIN(linecomment); }
<linecomment>\n			{ BEGIN(INITIAL); }
<linecomment>[^\n]*		{ }

 /* Keywords */
let 		{ return LET; }
if 			{ return IF; }
elif		{ return ELIF; }
else 		{ return ELSE; }
match 		{ return MATCH; }
loop 		{ return LOOP; }
end			{ return END; }
return 		{ return RETURN; }

fn 			{ return FN; }
mod 		{ return MOD; }
enum 		{ return ENUM; }
struct 		{ return STRUCT; }

true 		{ return TRUE; }
false 		{ return FALSE; }

 /* Types */
byte		{ return BYTE; }
word		{ return WORD; }
bool		{ return BOOL; }
reg			{ return REG8; }
reg8		{ return REG8; }
reg16		{ return REG16; }

 /* Registers */
a			{ return REG_A; }
f			{ return REG_F; }
b			{ return REG_B; }
c			{ return REG_C; }
d			{ return REG_D; }
e			{ return REG_E; }
h			{ return REG_H; }
l			{ return REG_L; }
ixh			{ return REG_IXH; }
ixl			{ return REG_IXL; }
i			{ return REG_I; }
r			{ return REG_R; }
iyh			{ return REG_IYH; }
iyl			{ return REG_IYL; }
af			{ return REG_AF; }
bc			{ return REG_BC; }
de			{ return REG_DE; }
hl			{ return REG_HL; }
ix			{ return REG_IX; }
iy			{ return REG_IY; }
af'			{ return REG_AFS; }

{name}		{ return NAME; }

0x[0-9a-fA-F_]+	{ BEGIN(suffix); return TINTEGER; }
0o[0-8_]+		{ BEGIN(suffix); return TINTEGER; }
0b[01_]+		{ BEGIN(suffix); return TINTEGER; }
[0-9][0-9_]*	{ BEGIN(suffix); return TINTEGER; }

;		{ return ';'; }
,		{ return ','; }
\.\.\.	{ return DOTDOTDOT; }
\.\.	{ return DOTDOT; }
\.		{ return '.'; }
\(		{ return '('; }
\)		{ return ')'; }
\{		{ return '{'; }
\}		{ return '}'; }
\[		{ return '['; }
\]		{ return ']'; }
@		{ return '@'; }
#		{ return '#'; }

\~ 		{ return '~'; }
:: 		{ return MOD_SEP; }
: 		{ return ':'; }
\$ 		{ return '$'; }
\? 		{ return '?'; }
== 		{ return EQEQ; }
=> 		{ return FAT_ARROW; }
= 		{ return '='; }
\!= 	{ return NE; }
\! 		{ return '!'; }
\<= 	{ return LE; }
\<\< 	{ return SHL; }
\<\<= 	{ return SHLEQ; }
\< 		{ return '<'; }
\>= 	{ return GE; }
\>\> 	{ return SHR; }
\>\>= 	{ return SHREQ; }
\> 		{ return '>'; }

 /* CHARS */
\x27								{ BEGIN(ltorchar); yymore(); }
<ltorchar>\x27						{ BEGIN(suffix); return TCHARS; }
<ltorchar><<EOF>>					{ return -1; }
<ltorchar>\\[n\nrt\\\x27\x220]		{ yymore(); }
<ltorchar>\\x[0-9a-fA-F]{2}			{ yymore(); }
<ltorchar>\\u\{[0-9a-fA-F]?{6}\}	{ yymore(); }
<ltorchar>\\[^n\nrt\\\x27\x220]		{ return -1; }
<ltorchar>(.|\n)					{ yymore(); }

 /* STRING */
\x22							{ BEGIN(str); yymore(); }
<str>\x22						{ BEGIN(suffix); return TSTRING; }
<str><<EOF>>					{ return -1; }
<str>\\[n\nrt\\\x27\x220]		{ yymore(); }
<str>\\x[0-9a-fA-F]{2}			{ yymore(); }
<str>\\u\{[0-9a-fA-F]?{6}\}		{ yymore(); }
<str>\\[^n\nrt\\\x27\x220]		{ return -1; }
<str>(.|\n)						{ yymore(); }

-\>		{ return RARROW; }
-		{ return '-'; }
--		{ return MINUSMINUS; }
-=		{ return MINUSEQ; }
&&		{ return ANDAND; }
&		{ return '&'; }
&=		{ return ANDEQ; }
\|\|	{ return OROR; }
\|		{ return '|'; }
\|=		{ return OREQ; }
\+		{ return '+'; }
\+\+	{ return PLUSPLUS; }
\+=		{ return PLUSEQ; }
\*		{ return '*'; }
\*=		{ return STAREQ; }
\/		{ return '/'; }
\/=		{ return SLASHEQ; }
\^		{ return '^'; }
\^=		{ return CARETEQ; }
%		{ return '%'; }
%=		{ return PERCENTEQ; }
<<EOF>> { return 0; }

%%