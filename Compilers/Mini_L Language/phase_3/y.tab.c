/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM = 258,
     BEGIN_PROGRAM = 259,
     END_PROGRAM = 260,
     INTEGER = 261,
     ARRAY = 262,
     OF = 263,
     IF = 264,
     THEN = 265,
     ENDIF = 266,
     ELSE = 267,
     ELSEIF = 268,
     WHILE = 269,
     DO = 270,
     BEGINLOOP = 271,
     ENDLOOP = 272,
     BREAK = 273,
     CONTINUE = 274,
     EXIT = 275,
     READ = 276,
     WRITE = 277,
     AND = 278,
     OR = 279,
     NOT = 280,
     TRUE = 281,
     FALSE = 282,
     SUB = 283,
     ADD = 284,
     MULT = 285,
     DIV = 286,
     MOD = 287,
     EQ = 288,
     NEQ = 289,
     LT = 290,
     GT = 291,
     LTE = 292,
     GTE = 293,
     PLUS = 294,
     MINUS = 295,
     SEMICOLON = 296,
     COLON = 297,
     COMMA = 298,
     QUESTION = 299,
     L_BRACKET = 300,
     R_BRACKET = 301,
     L_PAREN = 302,
     R_PAREN = 303,
     ASSIGN = 304,
     NUMBER = 305,
     IDENT = 306
   };
#endif
/* Tokens.  */
#define PROGRAM 258
#define BEGIN_PROGRAM 259
#define END_PROGRAM 260
#define INTEGER 261
#define ARRAY 262
#define OF 263
#define IF 264
#define THEN 265
#define ENDIF 266
#define ELSE 267
#define ELSEIF 268
#define WHILE 269
#define DO 270
#define BEGINLOOP 271
#define ENDLOOP 272
#define BREAK 273
#define CONTINUE 274
#define EXIT 275
#define READ 276
#define WRITE 277
#define AND 278
#define OR 279
#define NOT 280
#define TRUE 281
#define FALSE 282
#define SUB 283
#define ADD 284
#define MULT 285
#define DIV 286
#define MOD 287
#define EQ 288
#define NEQ 289
#define LT 290
#define GT 291
#define LTE 292
#define GTE 293
#define PLUS 294
#define MINUS 295
#define SEMICOLON 296
#define COLON 297
#define COMMA 298
#define QUESTION 299
#define L_BRACKET 300
#define R_BRACKET 301
#define L_PAREN 302
#define R_PAREN 303
#define ASSIGN 304
#define NUMBER 305
#define IDENT 306




/* Copy the first part of user declarations.  */
#line 1 "mini_l.y"

 #include <stdio.h>
 #include <stdlib.h>
 #include <sstream>
 #include <fstream>
 #include <string>
 #include <map>
 #include <stack>
 using namespace std;

 void yyerror(const char *msg);
 int yylex(void);
 extern int currLine;
 extern int currPos;

 stringstream declare;
 stringstream body;
 //ofstream declare("declare.txt");
 //ofstream body("body.txt");

 ofstream output;

 map<string, int> identifiers;
 stack<string> id_stack;
 stack<string> index_stack;
 stack<string> logic_stack;
 stack<string> pred_stack;
 stack<string> label_stack;
 stack<string> cont_stack;
 stack<string> brk_stack;
 stack<string> io_stack;

 string program_name;

 int temp_num = 0;
 int pred_num = 0;
 int label_num = 0;
 
 bool declare_mode = 1;
 bool error_flag = 0;
 string from_array (string save) {
	if (index_stack.top()!="-1") {
		stringstream conv;
		conv << temp_num;
		declare << ". t" << temp_num << endl;
		body << "=[] t" << temp_num << ", " << save
		<< ", " << index_stack.top() << endl;
		temp_num++;
		return "t"+conv.str();
	}
	return save;
 }



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 56 "mini_l.y"
{
  int ival;
  char s[20];
}
/* Line 193 of yacc.c.  */
#line 258 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 271 "y.tab.c"

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
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   141

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  82
/* YYNRULES -- Number of states.  */
#define YYNSTATES  155

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     9,    11,    17,    19,    23,    24,    29,
      33,    34,    36,    38,    45,    53,    55,    57,    59,    62,
      65,    67,    74,    80,    84,    91,    93,    99,   102,   103,
     105,   107,   108,   110,   112,   114,   116,   118,   124,   126,
     128,   131,   135,   136,   139,   143,   144,   147,   149,   153,
     155,   157,   161,   163,   165,   167,   169,   171,   173,   176,
     180,   181,   183,   185,   188,   192,   193,   195,   197,   199,
     202,   204,   206,   208,   212,   214,   219,   221,   225,   229,
     230,   233,   237
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,     3,    54,    41,    55,     5,    -1,    51,
      -1,    58,    41,    57,    56,    93,    -1,     4,    -1,    58,
      41,    57,    -1,    -1,    60,    59,    42,    61,    -1,    43,
      60,    59,    -1,    -1,    51,    -1,     6,    -1,     7,    45,
      50,    46,     8,     6,    -1,     7,    45,    28,    50,    46,
       8,     6,    -1,    20,    -1,    19,    -1,    18,    -1,    22,
      95,    -1,    21,    95,    -1,    63,    -1,    15,    69,    93,
      17,    14,    76,    -1,    70,    76,    71,    93,    72,    -1,
      91,    49,    73,    -1,     9,    76,    64,    93,    65,    11,
      -1,    10,    -1,    67,    76,    68,    93,    65,    -1,    66,
      93,    -1,    -1,    12,    -1,    13,    -1,    -1,    16,    -1,
      14,    -1,    16,    -1,    17,    -1,    83,    -1,    76,    75,
      83,    74,    83,    -1,    42,    -1,    44,    -1,    78,    77,
      -1,    24,    78,    77,    -1,    -1,    80,    79,    -1,    23,
      80,    79,    -1,    -1,    25,    81,    -1,    81,    -1,    83,
      82,    83,    -1,    26,    -1,    27,    -1,    47,    76,    48,
      -1,    33,    -1,    34,    -1,    35,    -1,    36,    -1,    37,
      -1,    38,    -1,    86,    84,    -1,    85,    86,    84,    -1,
      -1,    29,    -1,    28,    -1,    89,    87,    -1,    88,    89,
      87,    -1,    -1,    30,    -1,    31,    -1,    32,    -1,    28,
      90,    -1,    90,    -1,    91,    -1,    50,    -1,    47,    83,
      48,    -1,    60,    -1,    60,    92,    83,    46,    -1,    45,
      -1,    62,    41,    94,    -1,    62,    41,    94,    -1,    -1,
      91,    96,    -1,    43,    91,    96,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    81,    81,    91,    99,   102,   107,   108,   111,   114,
     115,   118,   169,   176,   187,   193,   194,   202,   210,   232,
     255,   256,   265,   266,   282,   291,   315,   319,   322,   325,
     334,   342,   360,   377,   387,   406,   417,   418,   433,   460,
     478,   481,   493,   496,   499,   511,   514,   524,   527,   543,
     544,   545,   548,   549,   550,   551,   552,   553,   556,   559,
     583,   586,   587,   590,   593,   620,   623,   624,   625,   628,
     640,   643,   644,   650,   653,   663,   670,   680,   683,   684,
     687,   690,   691
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "BEGIN_PROGRAM",
  "END_PROGRAM", "INTEGER", "ARRAY", "OF", "IF", "THEN", "ENDIF", "ELSE",
  "ELSEIF", "WHILE", "DO", "BEGINLOOP", "ENDLOOP", "BREAK", "CONTINUE",
  "EXIT", "READ", "WRITE", "AND", "OR", "NOT", "TRUE", "FALSE", "SUB",
  "ADD", "MULT", "DIV", "MOD", "EQ", "NEQ", "LT", "GT", "LTE", "GTE",
  "PLUS", "MINUS", "SEMICOLON", "COLON", "COMMA", "QUESTION", "L_BRACKET",
  "R_BRACKET", "L_PAREN", "R_PAREN", "ASSIGN", "NUMBER", "IDENT",
  "$accept", "prog", "p_ident", "block", "begin_program", "dec_loop",
  "dec", "ident_loop", "ident", "dec2", "stat", "if_stat", "then",
  "else_loop", "else", "elseif", "e_cond", "d_beginloop", "w_while",
  "w_beginloop", "w_endloop", "stat2", "colon", "question", "bool_exp",
  "bool_exp2", "rel_and_exp", "rel_and_exp2", "rel_exp", "rel_exp2",
  "comp", "exp", "exp2", "exp_op", "m_exp", "m_exp2", "mult_op", "term",
  "term2", "var", "l_bracket", "stat_loop", "stat_loop2", "var_loop",
  "var_loop2", 0
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
     305,   306
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    54,    55,    56,    57,    57,    58,    59,
      59,    60,    61,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    64,    65,    65,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    73,    74,    75,
      76,    77,    77,    78,    79,    79,    80,    80,    81,    81,
      81,    81,    82,    82,    82,    82,    82,    82,    83,    84,
      84,    85,    85,    86,    87,    87,    88,    88,    88,    89,
      89,    90,    90,    90,    91,    91,    92,    93,    94,    94,
      95,    96,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     1,     5,     1,     3,     0,     4,     3,
       0,     1,     1,     6,     7,     1,     1,     1,     2,     2,
       1,     6,     5,     3,     6,     1,     5,     2,     0,     1,
       1,     0,     1,     1,     1,     1,     1,     5,     1,     1,
       2,     3,     0,     2,     3,     0,     2,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     3,
       0,     1,     1,     2,     3,     0,     1,     1,     1,     2,
       1,     1,     1,     3,     1,     4,     1,     3,     3,     0,
       2,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     3,     0,     1,     0,    11,     0,     0,
      10,     2,     7,     0,     0,     0,     0,    10,     0,     5,
       0,     7,     9,    12,     0,     8,     0,    33,     0,    17,
      16,    15,     0,     0,    74,     0,    20,     0,     0,     4,
       6,     0,     0,    49,    50,     0,     0,    72,     0,    42,
      45,    47,     0,    60,    65,    70,    71,    32,     0,    82,
      19,    18,    76,     0,    79,     0,     0,     0,     0,    46,
       0,    69,     0,     0,    25,     0,     0,    40,     0,    43,
      52,    53,    54,    55,    56,    57,     0,    62,    61,    58,
       0,    66,    67,    68,    63,     0,     0,     0,    80,     0,
       0,    77,    34,     0,    23,     0,    36,     0,     0,     0,
      51,    73,    28,    42,    45,    48,    60,    65,     0,    82,
      75,    79,     0,    39,     0,     0,     0,    29,    30,     0,
       0,     0,    41,    44,    59,    64,     0,    81,    78,    35,
      22,     0,     0,    13,    24,    27,    31,    21,    38,     0,
      14,     0,    37,    28,    26
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     4,     8,    20,    15,    16,    14,    34,    25,
      35,    36,    75,   129,   130,   131,   151,    58,    37,   103,
     140,   104,   149,   124,    48,    77,    49,    79,    50,    51,
      86,    52,    89,    90,    53,    94,    95,    54,    55,    56,
      63,    39,   101,    60,    98
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -62
static const yytype_int8 yypact[] =
{
       2,   -35,    24,   -62,    -4,   -62,   -11,   -62,    49,    17,
      27,   -62,   -11,   -11,    29,    74,    50,    27,    39,   -62,
       8,   -11,   -62,   -62,    35,   -62,     6,   -62,    76,   -62,
     -62,   -62,   -11,   -11,    48,    53,   -62,     6,    47,   -62,
     -62,   -14,    22,   -62,   -62,   -43,     6,   -62,    87,    75,
      79,   -62,    51,    23,   -21,   -62,   -62,   -62,     8,    60,
     -62,   -62,   -62,    -8,     8,    82,     6,    56,    61,   -62,
      -8,   -62,    63,    28,   -62,     8,     6,   -62,     6,   -62,
     -62,   -62,   -62,   -62,   -62,   -62,    -8,   -62,   -62,   -62,
      -8,   -62,   -62,   -62,   -62,    -8,    91,   -11,   -62,    66,
      68,   -62,   -62,     8,   -62,    70,    51,    67,   107,    71,
     -62,   -62,    55,    75,    79,   -62,    23,   -21,   102,    60,
     -62,     8,   100,   -62,    -8,   110,   114,   -62,   -62,   111,
       8,     6,   -62,   -62,   -62,   -62,     6,   -62,   -62,   -62,
     -62,    81,   115,   -62,   -62,   -62,   -62,   -62,   -62,    -8,
     -62,     8,   -62,    55,   -62
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -62,   -62,   -62,   -62,   -62,   103,   119,   109,    69,   -62,
     -61,   -62,   -62,   -26,   -62,   -62,   -62,   -62,   -62,   -62,
     -62,   -62,   -62,   -62,   -31,    15,    54,    18,    57,    92,
     -62,   -45,    13,   -62,    43,    19,   -62,    42,    93,   -20,
     -62,   -56,    20,   106,    21
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      38,    73,    96,   100,    70,     1,    65,    47,     7,    91,
      92,    93,    59,    59,    67,    72,     3,    26,    99,   112,
      45,   106,    27,    28,     5,   109,    29,    30,    31,    32,
      33,    42,    43,    44,    45,   105,    68,     6,    38,    70,
       7,   115,    47,     7,    38,    23,    24,   122,    43,    44,
      45,    87,    88,    46,    11,    38,    47,     7,    12,     7,
     100,    80,    81,    82,    83,    84,    85,   127,   128,    46,
      13,    18,    47,     7,   145,    10,   111,   119,    19,   141,
      41,    10,    17,    38,    80,    81,    82,    83,    84,    85,
      10,    21,    57,    62,    64,   153,    66,    74,   102,    76,
     146,    38,    78,    97,   152,   147,   107,   108,   118,   121,
      38,   110,   120,   125,   123,   126,   136,   139,   142,   111,
     143,   150,   144,   148,    40,     9,    22,   154,   132,   134,
     113,    38,   133,   116,    69,   114,   135,   117,    71,    61,
     137,   138
};

static const yytype_uint8 yycheck[] =
{
      20,    46,    58,    64,    47,     3,    37,    50,    51,    30,
      31,    32,    32,    33,    28,    46,    51,     9,    63,    75,
      28,    66,    14,    15,     0,    70,    18,    19,    20,    21,
      22,    25,    26,    27,    28,    66,    50,    41,    58,    47,
      51,    86,    50,    51,    64,     6,     7,   103,    26,    27,
      28,    28,    29,    47,     5,    75,    50,    51,    41,    51,
     121,    33,    34,    35,    36,    37,    38,    12,    13,    47,
      43,    42,    50,    51,   130,     6,    48,    97,     4,   124,
      45,    12,    13,   103,    33,    34,    35,    36,    37,    38,
      21,    41,    16,    45,    41,   151,    49,    10,    16,    24,
     131,   121,    23,    43,   149,   136,    50,    46,    17,    41,
     130,    48,    46,    46,    44,     8,    14,    17,     8,    48,
       6,     6,    11,    42,    21,     6,    17,   153,   113,   116,
      76,   151,   114,    90,    42,    78,   117,    95,    45,    33,
     119,   121
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    53,    51,    54,     0,    41,    51,    55,    58,
      60,     5,    41,    43,    59,    57,    58,    60,    42,     4,
      56,    41,    59,     6,     7,    61,     9,    14,    15,    18,
      19,    20,    21,    22,    60,    62,    63,    70,    91,    93,
      57,    45,    25,    26,    27,    28,    47,    50,    76,    78,
      80,    81,    83,    86,    89,    90,    91,    16,    69,    91,
      95,    95,    45,    92,    41,    76,    49,    28,    50,    81,
      47,    90,    76,    83,    10,    64,    24,    77,    23,    79,
      33,    34,    35,    36,    37,    38,    82,    28,    29,    84,
      85,    30,    31,    32,    87,    88,    93,    43,    96,    83,
      62,    94,    16,    71,    73,    76,    83,    50,    46,    83,
      48,    48,    93,    78,    80,    83,    86,    89,    17,    91,
      46,    41,    93,    44,    75,    46,     8,    12,    13,    65,
      66,    67,    77,    79,    84,    87,    14,    96,    94,    17,
      72,    83,     8,     6,    11,    93,    76,    76,    42,    74,
       6,    68,    83,    93,    65
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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


/* Prevent warnings from -Wmissing-prototypes.  */

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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
        case 2:
#line 81 "mini_l.y"
    {
				body << ": EndLabel" << endl;
				if (!error_flag) {
					output.open (program_name.c_str(), std::ofstream::out);
					output << declare.rdbuf();
					output << body.rdbuf();
				}
			;}
    break;

  case 3:
#line 91 "mini_l.y"
    {
				string aug_name = "_"+string((yyvsp[(1) - (1)].s));
				pair<string, int> id(string(aug_name),-1);
				identifiers.insert(id);
				program_name = string((yyvsp[(1) - (1)].s))+".mil";
			;}
    break;

  case 5:
#line 102 "mini_l.y"
    {
				body << ": START" << endl;
				declare_mode = 0;
			;}
    break;

  case 11:
#line 118 "mini_l.y"
    {
				string aug_name = "_"+string((yyvsp[(1) - (1)].s));
				if(declare_mode) {
					pair<string, int> id(string(aug_name),-1);
					if (!identifiers.insert(id).second) {
						//declaration error already defined
						string error = "error, declaration name '"+string((yyvsp[(1) - (1)].s))
						+"' already defined.";
						yyerror(error.c_str());
					}			
				}
				else {
					if (
					string((yyvsp[(1) - (1)].s)) == "integer" ||
					string((yyvsp[(1) - (1)].s)) == "array" ||
					string((yyvsp[(1) - (1)].s)) == "loop" ||
					string((yyvsp[(1) - (1)].s)) == "endloop" ||
					string((yyvsp[(1) - (1)].s)) == "break" ||
					string((yyvsp[(1) - (1)].s)) == "continue" ||
					string((yyvsp[(1) - (1)].s)) == "read" ||
					string((yyvsp[(1) - (1)].s)) == "write" ||
					string((yyvsp[(1) - (1)].s)) == "of" ||
					string((yyvsp[(1) - (1)].s)) == "if" ||
					string((yyvsp[(1) - (1)].s)) == "while" ||
					string((yyvsp[(1) - (1)].s)) == "and" ||
					string((yyvsp[(1) - (1)].s)) == "or" ||
					string((yyvsp[(1) - (1)].s)) == "not" ||
					string((yyvsp[(1) - (1)].s)) == "then" ||
					string((yyvsp[(1) - (1)].s)) == "else" ||
					string((yyvsp[(1) - (1)].s)) == "endif" ||
					string((yyvsp[(1) - (1)].s)) == "true" ||
					string((yyvsp[(1) - (1)].s)) == "false" ||
					string((yyvsp[(1) - (1)].s)) == "program" ||
					string((yyvsp[(1) - (1)].s)) == "endprogram"
					) {
						string error = "error, "+string((yyvsp[(1) - (1)].s))
						+" is not declared.";
						yyerror(error.c_str());
					}
					else if(identifiers.find(aug_name)==identifiers.end()) {
						//variable error
						string error = "error, "+string((yyvsp[(1) - (1)].s))
						+" is not declared.";
						yyerror(error.c_str());
					}
					
				}
				id_stack.push(string(aug_name));
			;}
    break;

  case 12:
#line 169 "mini_l.y"
    {
				for(string dec; !id_stack.empty(); id_stack.pop()) {
					dec = id_stack.top();
					declare << ". " << dec << endl;
				}
									
			;}
    break;

  case 13:
#line 176 "mini_l.y"
    {
				if ((yyvsp[(3) - (6)].ival) <= 0) {
					string error = "error, array size must be a positive value.";
					yyerror(error.c_str());
				}
				for (string dec; !id_stack.empty(); id_stack.pop()) {
					dec = id_stack.top();
					identifiers[dec] = (yyvsp[(3) - (6)].ival);
					declare << ".[] " << dec << ", " << identifiers[dec] << endl;
				}
			;}
    break;

  case 14:
#line 187 "mini_l.y"
    {
				string error = "error, array size must be a positive value.";
				yyerror(error.c_str());
			;}
    break;

  case 15:
#line 193 "mini_l.y"
    {body << "\t:= EndLabel" << endl;;}
    break;

  case 16:
#line 194 "mini_l.y"
    {
				if (cont_stack.size())
					body << ":= " << cont_stack.top() << endl;
				else {
					string error = "error, 'continue' occurs outside of loop scope.";
					yyerror(error.c_str());
				}
			;}
    break;

  case 17:
#line 202 "mini_l.y"
    {
				if (brk_stack.size())
					body << ":= " << brk_stack.top() << endl;
				else {
					string error = "error, 'break' occurs outside of loop scope.";
					yyerror(error.c_str());
				}
			;}
    break;

  case 18:
#line 210 "mini_l.y"
    {
				stringstream conv;
				while (id_stack.size()) {
					if(index_stack.top() == "-1") {
						conv.str("");
						conv << "\t.> " << id_stack.top();
						io_stack.push(conv.str());
					}
					else {
						conv.str("");
						conv << "\t.[]> " << id_stack.top() << ", " 
						<< index_stack.top();
						io_stack.push(conv.str());
					}
					id_stack.pop();
					index_stack.pop();
				}
				while(io_stack.size()) {
					body << io_stack.top() << endl;
					io_stack.pop();
				}
			;}
    break;

  case 19:
#line 232 "mini_l.y"
    {
				stringstream conv;
				while (id_stack.size()) {
					if(index_stack.top() == "-1") {
						conv.str("");
						conv << "\t.< " << id_stack.top();
						io_stack.push(conv.str());
					}
					else {
						conv.str("");
						conv << "\t.[]< " << id_stack.top() << ", " 
						<< index_stack.top();
						io_stack.push(conv.str());
					}
					id_stack.pop();
					index_stack.pop();
				}
				while(io_stack.size()) {
					body << io_stack.top() << endl;
					io_stack.pop();
				}
			;}
    break;

  case 21:
#line 256 "mini_l.y"
    {
				body << ": " << cont_stack.top() << endl;
				cont_stack.pop();
				body << "\t?:= " << label_stack.top() << ", " << pred_stack.top() << endl;
				label_stack.pop();
				pred_stack.pop();
				body << ": " << brk_stack.top() << endl;
				brk_stack.pop();
			;}
    break;

  case 23:
#line 266 "mini_l.y"
    {
			        string saver = from_array(id_stack.top());
			        id_stack.pop();
			        index_stack.pop();
			        if(index_stack.top()!="-1") {
			                body << "\t[]= " << id_stack.top() << ", " 
			                << index_stack.top() << ", " << saver << endl;
			        }
			        else {
			                body << "\t= " << id_stack.top() << ", " << saver << endl;
			        }
			        id_stack.pop();
			        index_stack.pop();
			;}
    break;

  case 24:
#line 282 "mini_l.y"
    {
				if (label_stack.top() != "dummy")
					body << ": " << label_stack.top() << endl;
				label_stack.pop();
				body << ": " << label_stack.top() << endl;
				label_stack.pop();
			;}
    break;

  case 25:
#line 291 "mini_l.y"
    {
				//end label
				stringstream conv;
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;

                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				conv.str("");
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
			;}
    break;

  case 26:
#line 315 "mini_l.y"
    {

				
			;}
    break;

  case 27:
#line 319 "mini_l.y"
    {
				
			;}
    break;

  case 29:
#line 325 "mini_l.y"
    {
				string iter = label_stack.top();
				label_stack.pop();
				body << "\t:= " << label_stack.top() << endl;
				body << ": " << iter << endl;
				label_stack.push("dummy");
			;}
    break;

  case 30:
#line 334 "mini_l.y"
    {
				string iter = label_stack.top();
				label_stack.pop();
				body << "\t:= " << label_stack.top() << endl;
				body << ": " << iter << endl;
			;}
    break;

  case 31:
#line 342 "mini_l.y"
    {
                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << "\t. p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
			;}
    break;

  case 32:
#line 360 "mini_l.y"
    {
				stringstream conv;
				conv << "L" << label_num;
				brk_stack.push(conv.str());
				label_num++;
                                body << ": L" << label_num << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
				conv.str("");
				conv << "L" << label_num;
				cont_stack.push(conv.str());
				label_num++;
			;}
    break;

  case 33:
#line 377 "mini_l.y"
    {
                                body << ": L" << label_num << endl;
				stringstream conv;
				conv << "L" << label_num;
				label_stack.push(conv.str());
				cont_stack.push(conv.str());
				label_num++;
                        ;}
    break;

  case 34:
#line 387 "mini_l.y"
    {
                                body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				brk_stack.push(conv.str());
				label_num++;
                        ;}
    break;

  case 35:
#line 406 "mini_l.y"
    {
				cont_stack.pop();
				brk_stack.pop();
                                string end_label = label_stack.top();
                                label_stack.pop();
                                body << "\t:= " << label_stack.top() << endl;
                                label_stack.pop();
                                body << ": " << end_label << endl;
                        ;}
    break;

  case 37:
#line 418 "mini_l.y"
    {
			        string result = id_stack.top();
			        id_stack.pop();
			        if (index_stack.top()!="-1")
                        		body << "\t=[] " << id_stack.top() << ", " << result
                        		<< ", " << index_stack.top() << endl;
	                        else
	                                body << "\t= " << id_stack.top() << ", " << result << endl;
	                        index_stack.pop();

			        body << ": " << label_stack.top() << endl;
                                label_stack.pop();
			;}
    break;

  case 38:
#line 433 "mini_l.y"
    {
                        
			        if (index_stack.top()!="-1")
                        		body << "\t=[] t" << temp_num << ", " << id_stack.top()
                        		<< ", " << index_stack.top() << endl;
	                        else
	                                body << "\t= t" << temp_num << ", " << id_stack.top() << endl;
	                        index_stack.pop();
	                        id_stack.pop();
	                        declare << ". t" << temp_num << endl;
	                        stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
	                        

                                body << "\t:= L" << label_num << endl;
                                body << ": " << label_stack.top() << endl;
                                label_stack.pop();
                                conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
                        ;}
    break;

  case 39:
#line 460 "mini_l.y"
    {
				body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				string inverse = conv.str();
				pred_num++;
				
				body << "\t?:= L" << label_num << ", " << inverse << endl;
				conv.str("");
				conv << "L" << label_num;
				label_stack.push(conv.str());
				label_num++;
                        ;}
    break;

  case 41:
#line 481 "mini_l.y"
    {
                                string saver = pred_stack.top();
                                pred_stack.pop();
                                string savel = pred_stack.top();
                                pred_stack.pop();
                                declare << ". p" << pred_num << endl;
                                body << "\t|| p" << pred_num << ", " << savel << ", " << saver << endl;
                                stringstream conv;
                                conv << "p" << pred_num;
                                pred_stack.push(conv.str());
                                pred_num++;
                        ;}
    break;

  case 44:
#line 499 "mini_l.y"
    {
                                string saver = pred_stack.top();
                                pred_stack.pop();
                                string savel = pred_stack.top();
                                pred_stack.pop();
                                declare << ". p" << pred_num << endl;
                                body << "\t&& p" << pred_num << ", " << savel << ", " << saver << endl;
                                stringstream conv;
                                conv << "p" << pred_num;
                                pred_stack.push(conv.str());
                                pred_num++;
                        ;}
    break;

  case 46:
#line 514 "mini_l.y"
    {
				body << "\t! p" << pred_num
				<< ", " << pred_stack.top() << endl;
				pred_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				pred_stack.push(conv.str());
				pred_num++;
			;}
    break;

  case 48:
#line 527 "mini_l.y"
    {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t" <<logic_stack.top() << " p" << pred_num
				<< ", " << savel << ", " << saver << endl;
				logic_stack.pop();
				stringstream conv;
				conv << "p" << pred_num;
				declare << ". p" << pred_num << endl;
				pred_stack.push(conv.str());
				pred_num++;
			;}
    break;

  case 49:
#line 543 "mini_l.y"
    {pred_stack.push("1");;}
    break;

  case 50:
#line 544 "mini_l.y"
    {pred_stack.push("0");;}
    break;

  case 52:
#line 548 "mini_l.y"
    {logic_stack.push("==");;}
    break;

  case 53:
#line 549 "mini_l.y"
    {logic_stack.push("!=");;}
    break;

  case 54:
#line 550 "mini_l.y"
    {logic_stack.push("<");;}
    break;

  case 55:
#line 551 "mini_l.y"
    {logic_stack.push(">");;}
    break;

  case 56:
#line 552 "mini_l.y"
    {logic_stack.push("<=");;}
    break;

  case 57:
#line 553 "mini_l.y"
    {logic_stack.push(">=");;}
    break;

  case 59:
#line 559 "mini_l.y"
    {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t";
				switch ((yyvsp[(1) - (3)].ival)) {
					case 0: //addition
						body << "+";
					break;
					case 1: //subtraction
						body << "-";
					break;
				}
				declare << ". t" << temp_num << endl;
 				body << " t" << temp_num << ", " << savel << ", " << saver << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			;}
    break;

  case 61:
#line 586 "mini_l.y"
    {(yyval.ival) = 0;;}
    break;

  case 62:
#line 587 "mini_l.y"
    {(yyval.ival) = 1;;}
    break;

  case 64:
#line 593 "mini_l.y"
    {
				string saver = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				string savel = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				body << "\t";
				switch ((yyvsp[(1) - (3)].ival)) {
					case 0: //multiplication
						body << "*";
					break;
					case 1: //division
						body << "/";
					break;
					case 2: //modulus
						body << "%";
					break;
				}
				declare << ". t" << temp_num << endl;
 				body << " t" << temp_num << ", " << savel << ", " << saver << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			;}
    break;

  case 66:
#line 623 "mini_l.y"
    {(yyval.ival) = 0;;}
    break;

  case 67:
#line 624 "mini_l.y"
    {(yyval.ival) = 1;;}
    break;

  case 68:
#line 625 "mini_l.y"
    {(yyval.ival) = 2;;}
    break;

  case 69:
#line 628 "mini_l.y"
    {
				string save = from_array(id_stack.top());
				id_stack.pop();
				index_stack.pop();
				declare << ". t" << temp_num << endl;
				body << "\t- t" << temp_num << ", 0, " << save << endl;
				stringstream conv;
				conv << temp_num;
				id_stack.push("t"+conv.str());
				index_stack.push("-1");
				temp_num++;
			;}
    break;

  case 72:
#line 644 "mini_l.y"
    {
				stringstream conv;
				conv << (yyvsp[(1) - (1)].ival);
				id_stack.push(string(conv.str()));
				index_stack.push("-1");
			;}
    break;

  case 74:
#line 653 "mini_l.y"
    {
				if (identifiers.find(id_stack.top())!=identifiers.end() &&
				identifiers[id_stack.top()]!=-1) {
					//error: this is an array with index
					string error = "error, "+id_stack.top().substr(1)
					+" is an array and requires an index.";
					yyerror(error.c_str());
				}
				index_stack.push("-1");
			;}
    break;

  case 75:
#line 663 "mini_l.y"
    {
				index_stack.pop();
				index_stack.push(id_stack.top());
				id_stack.pop();
			
			;}
    break;

  case 76:
#line 670 "mini_l.y"
    {
				if (identifiers.find(id_stack.top())!=identifiers.end() &&
				identifiers[id_stack.top()]==-1) {
					string error = "error, "+id_stack.top().substr(1)
					+" is not an array.";
					yyerror(error.c_str());
				}
			;}
    break;


/* Line 1267 of yacc.c.  */
#line 2314 "y.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 693 "mini_l.y"


int main() {
   yyparse();
   return 0;
}

void yyerror(const char *msg) {
   error_flag = 1;
   printf("** Line %d, column %d: %s\n", currLine, currPos, msg);
}

