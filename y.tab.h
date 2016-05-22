/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    FNUMBER = 259,
    CHAR = 260,
    BOOL = 261,
    CONST = 262,
    CASE = 263,
    DEFAULT = 264,
    IF = 265,
    THEN = 266,
    ELSEIF = 267,
    ELSE = 268,
    SWITCH = 269,
    WHILE = 270,
    REPEAT = 271,
    UNTIL = 272,
    FOR = 273,
    GOTO = 274,
    CONTINUE = 275,
    BREAK = 276,
    RETURN = 277,
    READ = 278,
    PRINT = 279,
    EXIT = 280,
    VALUE = 281,
    FVALUE = 282,
    BVALUE = 283,
    VARIABLE = 284,
    OR_OR = 285,
    AND_AND = 286,
    OR = 287,
    XOR = 288,
    AND = 289,
    EQ = 290,
    NE = 291,
    GE = 292,
    LE = 293,
    NOT = 294,
    L_NOT = 295,
    IFX = 296,
    UMINUS = 297
  };
#endif
/* Tokens.  */
#define NUMBER 258
#define FNUMBER 259
#define CHAR 260
#define BOOL 261
#define CONST 262
#define CASE 263
#define DEFAULT 264
#define IF 265
#define THEN 266
#define ELSEIF 267
#define ELSE 268
#define SWITCH 269
#define WHILE 270
#define REPEAT 271
#define UNTIL 272
#define FOR 273
#define GOTO 274
#define CONTINUE 275
#define BREAK 276
#define RETURN 277
#define READ 278
#define PRINT 279
#define EXIT 280
#define VALUE 281
#define FVALUE 282
#define BVALUE 283
#define VARIABLE 284
#define OR_OR 285
#define AND_AND 286
#define OR 287
#define XOR 288
#define AND 289
#define EQ 290
#define NE 291
#define GE 292
#define LE 293
#define NOT 294
#define L_NOT 295
#define IFX 296
#define UMINUS 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 22 "yac.y" /* yacc.c:1909  */
 
    int iValue;                 /* integer value */ 
    float fValue ;                 /*  float value*/
    int bValue;                 /* bool value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */ 

#line 146 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
