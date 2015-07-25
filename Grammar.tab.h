/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SHL = 258,
     SHR = 259,
     LE = 260,
     EQEQ = 261,
     NE = 262,
     GE = 263,
     ANDAND = 264,
     OROR = 265,
     SHLEQ = 266,
     SHREQ = 267,
     MINUSEQ = 268,
     ANDEQ = 269,
     OREQ = 270,
     PLUSEQ = 271,
     STAREQ = 272,
     SLASHEQ = 273,
     CARETEQ = 274,
     PERCENTEQ = 275,
     MINUSMINUS = 276,
     PLUSPLUS = 277,
     DOTDOT = 278,
     DOTDOTDOT = 279,
     MOD_SEP = 280,
     RARROW = 281,
     FAT_ARROW = 282,
     TINTEGER = 283,
     TCHARS = 284,
     TSTRING = 285,
     NAME = 286,
     LET = 287,
     IF = 288,
     ELIF = 289,
     ELSE = 290,
     MATCH = 291,
     LOOP = 292,
     END = 293,
     RETURN = 294,
     FN = 295,
     MOD = 296,
     ENUM = 297,
     STRUCT = 298,
     TRUE = 299,
     FALSE = 300,
     BYTE = 301,
     WORD = 302,
     BOOL = 303,
     REG8 = 304,
     REG16 = 305,
     REG_A = 306,
     REG_F = 307,
     REG_B = 308,
     REG_C = 309,
     REG_D = 310,
     REG_E = 311,
     REG_H = 312,
     REG_L = 313,
     REG_IXH = 314,
     REG_IXL = 315,
     REG_I = 316,
     REG_R = 317,
     REG_IYH = 318,
     REG_IYL = 319,
     REG_AF = 320,
     REG_BC = 321,
     REG_DE = 322,
     REG_HL = 323,
     REG_IX = 324,
     REG_IY = 325
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 18 "Grammar.y"

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


/* Line 2058 of yacc.c  */
#line 142 "Grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
