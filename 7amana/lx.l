%{
	#include "y.tab.h"
%}

%%
:				return COLON;
"esm"			return NAME;
"rkm"			return NUMBER;
"kasr"			return FNUMBER;
"sabt"			return CONST;


"lw"			return IF;
"yb2a"			return THEN;
"aw lw"			return ELSEIF;
"aw"			return ELSE;
"d3bs"			return SWITCH;
"efsl"			return BREAK;
"lma"			return CASE;
"el3ady"		return DEFAULT;

"tool lma"		return WHILE;
"e3ml"			return REPEAT;
"l7d"			return UNTIL;
"leff"			return FOR;
"efks"			return RETURN;

"t3refat"		return t3refat;
"brnamg"		return brnamg;
"_5lsana"		return _5lsana;

"e2ra"			return READ;
"etb3"			return PRINT;
"o5rog"			return EXIT;

[a-zA-Z][_a-zA-Z0-9]*		{yylval.id = yytext;return IDENTIFIER;}
0						|
[-]?[1-9][0-9]*				{yylval.num = atoi(yytext);return VALUE;}
[-]?[0-9]+[.e][0-9]+ 	|
[-]?[0-9]+[e]-?[0-9]+	|
[-]?[0-9]+[.][e]-?[0-9]+	{yylval.fnum = atof(yytext);return FVALUE;}
["][a-zA-z0-9]*["]			{yylval.name = yytext;return STRING;}
[-+*/]           	   		return yytext[0];

">"				return L_OP;
">="			return LE_OP;
"<"				return S_OP;
"<="			return SE_OP;
"=="			return E_OP;
"!="			return NE_OP;
"&&"			return AND;
"||"			return OR;

"--"			return DEC;
"++"			return INC;

";"				return SC;
"{"				return('{');
"}"				return('}');
","				return(',');
"="				return('=');
"("				return('(');
")"				return(')');
"["				return('[');
"]"				return(']');
"&"				return('&');
"!"				return('!');
"~"				return('~');
"%"				return('%');
"^"				return('^');
"|"				return('|');
"?"				return('?');

[ \t]+						;
[\n]+						yylineno++;


.                      {ECHO; yyerror ("unexpected character");}

%%
