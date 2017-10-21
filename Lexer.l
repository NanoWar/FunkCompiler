%{

#define YY_DECL extern "C" int yylex()
#include "Global.h"
#include "AST.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "Definitions.h"
#include "Lexer.h"

#include "Grammar.tab.h"

#define YY_USER_ACTION before_rule();
#define YY_BREAK after_rule(); break;

%}

%option noyywrap
%option stack
%option yylineno

%x str
%x chars
%x linecomment
%x blockcomment
%x include
%x suffix
%x asm

name [a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]*[\!\?]?

%%

asm[ \t\n\r]*\{	{ BEGIN(asm); }
<asm>[^\}]*		{ yymore(); }
<asm>\}			{ BEGIN(INITIAL); *(yytext+yyleng-1) = 0; return tASM; }

<suffix>{name}	{ BEGIN(INITIAL); }
<suffix>(.|\n)	{ yyless(0); BEGIN(INITIAL); }

[ \t\r] { }
[\n] { }

\/\* 					{ yy_push_state(blockcomment); }
<blockcomment>\*\/		{ yy_pop_state(); }
<blockcomment>(.|\n)	{ }

\/\/					{ BEGIN(linecomment); }
<linecomment>\n			{ BEGIN(INITIAL); }
<linecomment>[^\n]*		{ }

inc						{ BEGIN(include); }
<include>[ \t\x22]*		{ yytext += yyleng; }
<include>[^ \t\n\r\x22][^\t\n\r]*\x22? {
	if(*(yytext+yyleng-1) == '\x22') *(yytext+yyleng-1) = '\0';
	Trace("Including file <%s>", yytext);
	string file_path(input_file_folder);
	file_path.append(yytext);
	string file_name(yytext);
	if(endsWith(file_name, ".z80")) {
		LoadDefinitionsFile(file_path);
	} else {
		FILE *includeFile = fopen(file_path.c_str(), "r");
		if (!includeFile) {
			file_name.append(".funk");
			file_path.append(".funk");
			Trace("Including file <%s> instead", file_name.c_str());
			includeFile = fopen(file_path.c_str(), "r");
		}
		if (!includeFile) {
			Error("Cannot open file <%s>", yytext);
		} else {
			yyin = includeFile;
			yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
			yypush_location_state(file_name);
		}
	}
	BEGIN(INITIAL);
}

 /* Keywords */
if 			{ return IF; }
elif		{ return ELIF; }
else 		{ return ELSE; }
loop 		{ return LOOP; }
save		{ return SAVE; }
return 		{ return RETURN; }

fn 			{ return FN; }
mod 		{ return MOD; }
enum 		{ return ENUM; }
struct 		{ return STRUCT; }

true 		{ return tTRUE; }
false 		{ return tFALSE; }

 /* Types */
byte		{ return tBYTE; }
word		{ return tWORD; }
bool		{ return tBOOL; }

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

{name}			{ return NAME; }

0x[0-9a-fA-F_]+	{ BEGIN(suffix); return tINTEGER; }
0o[0-8_]+		{ BEGIN(suffix); return tINTEGER; }
0b[01_]+		{ BEGIN(suffix); return tINTEGER; }
[0-9][0-9_]*	{ BEGIN(suffix); return tINTEGER; }

\x27							{ BEGIN(chars); yymore(); yytext++; }
<chars>\x27						{ BEGIN(suffix); *(yytext+yyleng-1) = 0; return tCHARS; }
<chars><<EOF>>					{ return -1; }
<chars>\\[n\nrt\\\x27\x220]		{ yymore(); }
<chars>\\x[0-9a-fA-F]{2}		{ yymore(); }
<chars>\\u\{[0-9a-fA-F]?{6}\}	{ yymore(); }
<chars>\\[^n\nrt\\\x27\x220]	{ return -1; }
<chars>(.|\n)					{ yymore(); }

\x22							{ BEGIN(str); yymore(); yytext++; }
<str>\x22						{ BEGIN(suffix); *(yytext+yyleng-1) = 0; return tSTRING; }
<str><<EOF>>					{ return -1; }
<str>\\[n\nrt\\\x27\x220]		{ yymore(); }
<str>\\x[0-9a-fA-F]{2}			{ yymore(); }
<str>\\u\{[0-9a-fA-F]?{6}\}		{ yymore(); }
<str>\\[^n\nrt\\\x27\x220]		{ return -1; }
<str>(.|\n)						{ yymore(); }

;		{ return ';'; }
,		{ return ','; }
\.		{ return '.'; }
\(		{ return '('; }
\)		{ return ')'; }
\{		{ return '{'; }
\}		{ return '}'; }
\[		{ return '['; }
\]		{ return ']'; }
\@		{ return '@'; }
\#		{ return '#'; }

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

<<EOF>> {
	yypop_buffer_state();
	if (!YY_CURRENT_BUFFER) {
		yyterminate();
		return 0;
	}
	else {
		yypop_location_state();
	}
}

%%
