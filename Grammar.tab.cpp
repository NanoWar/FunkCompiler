/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "Grammar.y"

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


/* Line 371 of yacc.c  */
#line 85 "Grammar.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "Grammar.tab.h".  */
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
     MINUSMINUS = 269,
     ANDEQ = 270,
     OREQ = 271,
     PLUSEQ = 272,
     PLUSPLUS = 273,
     STAREQ = 274,
     SLASHEQ = 275,
     CARETEQ = 276,
     PERCENTEQ = 277,
     DOTDOT = 278,
     DOTDOTDOT = 279,
     MOD_SEP = 280,
     RARROW = 281,
     FAT_ARROW = 282,
     LIT_CHARS = 283,
     LIT_INTEGER = 284,
     LIT_STR = 285,
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
     REG16 = 305
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 201 "Grammar.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNRULES -- Number of states.  */
#define YYNSTATES  145

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    59,     2,
      55,    56,    53,    51,    57,    52,    63,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    58,     2,
      62,    60,    61,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     8,    10,    13,    15,    17,
      24,    28,    29,    31,    32,    34,    38,    43,    46,    47,
      49,    51,    52,    54,    57,    58,    60,    64,    66,    71,
      73,    75,    77,    79,    81,    83,    85,    87,    89,    90,
      92,    95,    97,    99,   101,   103,   105,   109,   113,   116,
     119,   124,   127,   128,   131,   132,   139,   141,   142,   144,
     147,   151,   153,   154,   157,   162,   167,   169,   170,   172,
     176,   179,   181,   182,   184,   186,   189,   193,   197,   201,
     205,   209,   213,   217,   221,   225,   227,   229,   231,   234,
     236,   238,   240,   242,   244,   247,   250,   251,   253,   257
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      65,     0,    -1,    66,    -1,    67,    -1,    -1,    68,    -1,
      67,    68,    -1,    69,    -1,    80,    -1,    40,   106,    70,
      77,    83,    38,    -1,    55,    71,    56,    -1,    -1,    72,
      -1,    -1,    73,    -1,    72,    57,    73,    -1,    81,    74,
      75,   106,    -1,    58,    76,    -1,    -1,    59,    -1,    53,
      -1,    -1,   106,    -1,    26,    78,    -1,    -1,    79,    -1,
      78,    57,    79,    -1,    81,    -1,    41,   106,    66,    38,
      -1,    46,    -1,    47,    -1,    48,    -1,    82,    -1,   106,
      -1,    49,    -1,    50,    -1,    84,    -1,    85,    -1,    -1,
      86,    -1,    85,    86,    -1,    87,    -1,    90,    -1,    96,
      -1,    97,    -1,   100,    -1,   103,    17,   103,    -1,   103,
      13,   103,    -1,   103,    18,    -1,   103,    14,    -1,    32,
     106,    88,    89,    -1,    58,    81,    -1,    -1,    60,   102,
      -1,    -1,    33,   102,    83,    91,    94,    38,    -1,    92,
      -1,    -1,    93,    -1,    92,    93,    -1,    34,   102,    83,
      -1,    95,    -1,    -1,    35,    83,    -1,    37,   106,    83,
      38,    -1,   106,    55,    98,    56,    -1,    99,    -1,    -1,
     106,    -1,    99,    57,   106,    -1,    39,   101,    -1,   102,
      -1,    -1,    97,    -1,   103,    -1,    53,   102,    -1,   102,
      51,   102,    -1,   102,    52,   102,    -1,   102,    53,   102,
      -1,   102,    54,   102,    -1,   102,     6,   102,    -1,   102,
       7,   102,    -1,   102,    61,   102,    -1,   102,    62,   102,
      -1,    55,   102,    56,    -1,   104,    -1,   106,    -1,    29,
      -1,    52,    29,    -1,    28,    -1,    44,    -1,    45,    -1,
     105,    -1,    30,    -1,   109,   107,    -1,    63,   108,    -1,
      -1,   109,    -1,   108,    63,   109,    -1,    31,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    90,    90,    94,    95,    99,   100,   104,   105,   109,
     113,   114,   117,   118,   121,   122,   125,   129,   130,   134,
     135,   136,   140,   144,   145,   148,   149,   152,   156,   169,
     170,   171,   172,   173,   177,   178,   186,   189,   190,   193,
     194,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     212,   215,   216,   219,   220,   224,   227,   228,   231,   232,
     235,   238,   239,   242,   246,   250,   253,   254,   257,   258,
     262,   270,   271,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   292,   293,   297,   298,   299,
     300,   301,   302,   306,   315,   319,   320,   324,   325,   329
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SHL", "SHR", "LE", "EQEQ", "NE", "GE",
  "ANDAND", "OROR", "SHLEQ", "SHREQ", "MINUSEQ", "MINUSMINUS", "ANDEQ",
  "OREQ", "PLUSEQ", "PLUSPLUS", "STAREQ", "SLASHEQ", "CARETEQ",
  "PERCENTEQ", "DOTDOT", "DOTDOTDOT", "MOD_SEP", "RARROW", "FAT_ARROW",
  "LIT_CHARS", "LIT_INTEGER", "LIT_STR", "NAME", "LET", "IF", "ELIF",
  "ELSE", "MATCH", "LOOP", "END", "RETURN", "FN", "MOD", "ENUM", "STRUCT",
  "TRUE", "FALSE", "BYTE", "WORD", "BOOL", "REG8", "REG16", "'+'", "'-'",
  "'*'", "'/'", "'('", "')'", "','", "':'", "'&'", "'='", "'>'", "'<'",
  "'.'", "$accept", "funk", "items_maybe", "items", "item", "item_fn",
  "fn_params_maybe", "param_defs_maybe", "param_defs", "param_def",
  "bind_maybe", "prefix_maybe", "register", "fn_outputs_maybe", "outputs",
  "output", "item_mod", "ty", "ty_reg", "block", "stmts_maybe", "stmts",
  "stmt", "let", "ty_ascription_maybe", "init_expr_maybe", "if",
  "elifs_maybe", "elifs", "elif", "else_maybe", "else", "loop", "call",
  "call_params_maybe", "call_params", "return", "expr_maybe", "expr",
  "val", "lit", "str", "ident", "fields_maybe", "fields", "name", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    43,    45,    42,    47,    40,    41,    44,    58,    38,
      61,    62,    60,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    66,    66,    67,    67,    68,    68,    69,
      70,    70,    71,    71,    72,    72,    73,    74,    74,    75,
      75,    75,    76,    77,    77,    78,    78,    79,    80,    81,
      81,    81,    81,    81,    82,    82,    83,    84,    84,    85,
      85,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      87,    88,    88,    89,    89,    90,    91,    91,    92,    92,
      93,    94,    94,    95,    96,    97,    98,    98,    99,    99,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   103,   103,   104,   104,   104,
     104,   104,   104,   105,   106,   107,   107,   108,   108,   109
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     1,     2,     1,     1,     6,
       3,     0,     1,     0,     1,     3,     4,     2,     0,     1,
       1,     0,     1,     2,     0,     1,     3,     1,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     1,     1,     1,     1,     1,     3,     3,     2,     2,
       4,     2,     0,     2,     0,     6,     1,     0,     1,     2,
       3,     1,     0,     2,     4,     4,     1,     0,     1,     3,
       2,     1,     0,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     2,     2,     0,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     0,     2,     3,     5,     7,     8,    99,
      11,    96,     4,     1,     6,    13,    24,     0,    94,     0,
      29,    30,    31,    34,    35,     0,    12,    14,    18,    32,
      33,     0,    38,    95,    97,    28,    10,     0,     0,    21,
      23,    25,    27,    89,    87,    93,     0,     0,     0,    72,
      90,    91,     0,     0,    36,    37,    39,    41,    42,    43,
      44,    45,     0,    85,    92,    86,     0,    15,    17,    22,
      20,    19,     0,     0,    52,     0,     0,    73,    38,    74,
      38,    70,    71,    88,     9,    40,     0,    49,     0,    48,
      67,    98,    16,    26,     0,    54,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,    47,
      86,    46,     0,    66,    68,    51,     0,    50,    84,    80,
      81,    76,    87,    77,    78,    79,    82,    83,     0,    62,
      56,    58,    64,    65,     0,    53,    38,    38,     0,    61,
      59,    69,    60,    63,    55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,    16,    25,    26,    27,
      39,    72,    68,    32,    40,    41,     8,    28,    29,    53,
      54,    55,    56,    57,    95,   117,    58,   129,   130,   131,
     138,   139,    59,    77,   112,   113,    61,    81,   123,    79,
      63,    64,    65,    18,    33,    11
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int16 yypact[] =
{
     -32,   -25,   -25,     4,   -64,   -32,   -64,   -64,   -64,   -64,
     -44,   -45,   -32,   -64,   -64,   -15,    -2,   -25,   -64,    -9,
     -64,   -64,   -64,   -64,   -64,   -16,   -13,   -64,   -17,   -64,
     -64,   -15,   173,   -21,   -64,   -64,   -64,   -15,   -25,   -40,
     -11,   -64,   -64,   -64,   -64,   -64,   -25,   133,   -25,   133,
     -64,   -64,    25,     5,   -64,   173,   -64,   -64,   -64,   -64,
     -64,   -64,     9,   -64,   -64,     0,   -25,   -64,   -64,   -64,
     -64,   -64,   -25,   -15,     1,   133,   133,   -64,    90,   -64,
     173,   -64,   106,   -64,   -64,   -64,   163,   -64,   163,   -64,
     -25,   -64,   -64,   -64,   -15,    -3,    14,    94,   133,   133,
     133,   145,   133,   133,   133,   133,    26,    31,   133,   -64,
     -64,   -64,     8,    23,   -64,   -64,   133,   -64,   -64,   106,
     106,    77,    64,    77,    14,    14,   106,   106,   133,    51,
      26,   -64,   -64,   -64,   -25,   106,    90,   173,    50,   -64,
     -64,   -64,   -64,   -64,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -64,   -64,    78,   -64,    87,   -64,   -64,   -64,   -64,    57,
     -64,   -64,   -64,   -64,   -64,    22,   -64,   -24,   -64,   -63,
     -64,   -64,    43,   -64,   -64,   -64,   -64,   -64,   -64,   -31,
     -64,   -64,   -64,   -27,   -64,   -64,   -64,   -64,   -37,   -30,
     -64,   -64,    -1,   -64,   -64,   -14
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -89
static const yytype_int16 yytable[] =
{
      10,    12,    62,    34,    13,    60,     9,    42,     1,     2,
      78,    15,    82,    70,    30,   106,     9,   107,    17,    71,
      98,    99,    86,    87,    31,    62,    88,    89,    60,    35,
      30,    20,    21,    22,    23,    24,    30,    69,    96,    97,
      36,    38,    66,    84,    37,    74,    73,    80,    62,    42,
      62,    60,    91,    60,    83,    90,   109,   116,   111,    94,
     128,   119,   120,   121,   133,   124,   125,   126,   127,   132,
     115,    92,    30,   142,   143,   104,   105,   -88,   -88,   135,
     134,   -88,   -88,    98,    99,   110,   137,   110,   144,   114,
      19,   136,    14,    30,    67,    93,    98,    99,    85,   140,
      98,    99,     0,     0,     0,     0,    62,    62,     0,    60,
      60,     0,    98,    99,     0,     0,     0,     0,    43,    44,
      45,     9,    46,    47,     0,     0,     0,    48,     0,    49,
     102,   103,     0,   141,    50,    51,     0,     0,   104,   105,
       0,   100,   101,   102,   103,   100,   108,   102,   103,     0,
     118,   104,   105,     0,     0,   104,   105,   100,   108,   102,
     103,    43,    44,    45,     9,     0,     0,   104,   105,     0,
       0,     0,     0,    43,   122,    45,     9,    50,    51,     0,
       0,     0,     0,     0,     0,    52,    75,     0,    76,    50,
      51,    43,    44,    45,     9,     0,     0,    52,    75,     0,
      76,    43,    44,    45,     9,    46,    47,    50,    51,     0,
      48,     0,    49,     0,     0,    52,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    52
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-64)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       1,     2,    32,    17,     0,    32,    31,    31,    40,    41,
      47,    55,    49,    53,    15,    78,    31,    80,    63,    59,
       6,     7,    13,    14,    26,    55,    17,    18,    55,    38,
      31,    46,    47,    48,    49,    50,    37,    38,    75,    76,
      56,    58,    63,    38,    57,    46,    57,    48,    78,    73,
      80,    78,    66,    80,    29,    55,    86,    60,    88,    58,
      34,    98,    99,   100,    56,   102,   103,   104,   105,    38,
      94,    72,    73,   136,   137,    61,    62,    13,    14,   116,
      57,    17,    18,     6,     7,    86,    35,    88,    38,    90,
      12,   128,     5,    94,    37,    73,     6,     7,    55,   130,
       6,     7,    -1,    -1,    -1,    -1,   136,   137,    -1,   136,
     137,    -1,     6,     7,    -1,    -1,    -1,    -1,    28,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      53,    54,    -1,   134,    44,    45,    -1,    -1,    61,    62,
      -1,    51,    52,    53,    54,    51,    52,    53,    54,    -1,
      56,    61,    62,    -1,    -1,    61,    62,    51,    52,    53,
      54,    28,    29,    30,    31,    -1,    -1,    61,    62,    -1,
      -1,    -1,    -1,    28,    29,    30,    31,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    55,    44,
      45,    28,    29,    30,    31,    -1,    -1,    52,    53,    -1,
      55,    28,    29,    30,    31,    32,    33,    44,    45,    -1,
      37,    -1,    39,    -1,    -1,    52,    -1,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    52
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    40,    41,    65,    66,    67,    68,    69,    80,    31,
     106,   109,   106,     0,    68,    55,    70,    63,   107,    66,
      46,    47,    48,    49,    50,    71,    72,    73,    81,    82,
     106,    26,    77,   108,   109,    38,    56,    57,    58,    74,
      78,    79,    81,    28,    29,    30,    32,    33,    37,    39,
      44,    45,    52,    83,    84,    85,    86,    87,    90,    96,
      97,   100,   103,   104,   105,   106,    63,    73,    76,   106,
      53,    59,    75,    57,   106,    53,    55,    97,   102,   103,
     106,   101,   102,    29,    38,    86,    13,    14,    17,    18,
      55,   109,   106,    79,    58,    88,   102,   102,     6,     7,
      51,    52,    53,    54,    61,    62,    83,    83,    52,   103,
     106,   103,    98,    99,   106,    81,    60,    89,    56,   102,
     102,   102,    29,   102,   102,   102,   102,   102,    34,    91,
      92,    93,    38,    56,    57,   102,   102,    35,    94,    95,
      93,   106,    83,    83,    38
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
/* Line 1792 of yacc.c  */
#line 95 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 99 "Grammar.y"
    { (yyval) = mk_node("Items", 1, (yyvsp[(1) - (1)])); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 100 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (2)]), 1, (yyvsp[(2) - (2)])); }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 109 "Grammar.y"
    { (yyval) = mk_node("Function", 4, (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 113 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 114 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 118 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 121 "Grammar.y"
    { (yyval) = mk_node("ParamDefs", 1, (yyvsp[(1) - (1)])); }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 122 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (3)]), 1, (yyvsp[(3) - (3)])); }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 125 "Grammar.y"
    { (yyval) = mk_node("ParamDef", 4, (yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 129 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 130 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 134 "Grammar.y"
    { (yyval) = mk_atom("Mutable"); }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 135 "Grammar.y"
    { (yyval) = mk_atom("Pointer"); }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 136 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 144 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 145 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 148 "Grammar.y"
    { (yyval) = mk_node("Outputs", 1, (yyvsp[(1) - (1)])); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 149 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (3)]), 1, (yyvsp[(3) - (3)])); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 152 "Grammar.y"
    { (yyval) = mk_node("Output", 1, (yyvsp[(1) - (1)])); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 156 "Grammar.y"
    { (yyval) = mk_node("Module", 2, (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 169 "Grammar.y"
    { (yyval) = mk_atom("TyByte"); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 170 "Grammar.y"
    { (yyval) = mk_atom("TyWord"); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 171 "Grammar.y"
    { (yyval) = mk_atom("TyBool"); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 177 "Grammar.y"
    { (yyval) = mk_atom("TyReg8"); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 178 "Grammar.y"
    { (yyval) = mk_atom("TyReg16"); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 186 "Grammar.y"
    { (yyval) = mk_node("Block", 1, (yyvsp[(1) - (1)])); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 190 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 193 "Grammar.y"
    { (yyval) = mk_node("Statements", 1, (yyvsp[(1) - (1)])); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 194 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (2)]), 1, (yyvsp[(2) - (2)])); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 205 "Grammar.y"
    { (yyval) = mk_node("PlusEquals", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 206 "Grammar.y"
    { (yyval) = mk_node("MinusEquals", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 207 "Grammar.y"
    { (yyval) = mk_node("PlusPlus", 1, (yyvsp[(1) - (2)])); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 208 "Grammar.y"
    { (yyval) = mk_node("MinusMinus", 1, (yyvsp[(1) - (2)])); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 212 "Grammar.y"
    { (yyval) = mk_node("DeclLocal", 3, (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 215 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 216 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 219 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 220 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 224 "Grammar.y"
    { (yyval) = mk_node("If", 4, (yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 228 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 231 "Grammar.y"
    { (yyval) = mk_node("Elifs", 1, (yyvsp[(1) - (1)])); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 232 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (2)]), 1, (yyvsp[(2) - (2)])); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 235 "Grammar.y"
    { (yyval) = mk_node("Elif", 2, (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 239 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 242 "Grammar.y"
    { (yyval) = mk_node("Else", 1, (yyvsp[(2) - (2)])); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 246 "Grammar.y"
    { (yyval) = mk_node("Loop", 2, (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 250 "Grammar.y"
    { (yyval) = mk_node("FunctionCall", 2, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 254 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 257 "Grammar.y"
    { (yyval) = mk_node("Params", 1, (yyvsp[(1) - (1)])); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 258 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (3)]), 1, (yyvsp[(3) - (3)])); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 262 "Grammar.y"
    { (yyval) = mk_node("Return", 1, (yyvsp[(2) - (2)])); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 271 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 277 "Grammar.y"
    { (yyval) = mk_node("Indirection", 1, (yyvsp[(2) - (2)])); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 278 "Grammar.y"
    { (yyval) = mk_node("Plus", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 279 "Grammar.y"
    { (yyval) = mk_node("Minus", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 280 "Grammar.y"
    { (yyval) = mk_node("Mul", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 281 "Grammar.y"
    { (yyval) = mk_node("Div", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 282 "Grammar.y"
    { (yyval) = mk_node("Equals", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 283 "Grammar.y"
    { (yyval) = mk_node("Unequals", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 284 "Grammar.y"
    { (yyval) = mk_node("Greater", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 285 "Grammar.y"
    { (yyval) = mk_node("Smaller", 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 286 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 297 "Grammar.y"
    { (yyval) = mk_node("Integer", 1, mk_atom(yytext)); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 298 "Grammar.y"
    { (yyval) = mk_node("NegInteger", 1, mk_atom(yytext)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 299 "Grammar.y"
    { (yyval) = mk_node("Char", 1, mk_atom(yytext)); }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 300 "Grammar.y"
    { (yyval) = mk_node("Bool", 1, mk_atom(yytext)); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 301 "Grammar.y"
    { (yyval) = mk_node("Bool", 1, mk_atom(yytext)); }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 306 "Grammar.y"
    { (yyval) = mk_node("String", 1, mk_atom(yytext)); }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 315 "Grammar.y"
    { (yyval) = mk_node("Ident", 2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 319 "Grammar.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 320 "Grammar.y"
    { (yyval) = mk_none(); }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 324 "Grammar.y"
    { (yyval) = mk_node("Fields", 1, (yyvsp[(1) - (1)])); }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 325 "Grammar.y"
    { (yyval) = ext_node((yyvsp[(1) - (3)]), 1, (yyvsp[(3) - (3)])); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 329 "Grammar.y"
    { (yyval) = mk_node("Name", 1, mk_atom(yytext)); }
    break;


/* Line 1792 of yacc.c  */
#line 2006 "Grammar.tab.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 332 "Grammar.y"

