/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 12 "mini_l.y"
{
  int ival;
  char s[20];
}
/* Line 1529 of yacc.c.  */
#line 156 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

