%{ 
#include <stdlib.h> 
#include "def.h" 
#include "y.tab.h" 
void yyerror(char *); 
%} 

%% 

"rkm"		return NUMBER;
"kasr"		return FNUMBER;
"7arf"		return CHAR;
"bool"		return BOOL;
"sabt"		return CONST;

"lw"		return IF;
"yb2a"		return THEN;
"aw lw"		return ELSEIF;
"aw"		return ELSE;
"d3bs"		return SWITCH;
"efsl"		return BREAK;
"lma"		return CASE;
"el3ady"		return DEFAULT;

"tool lma"		return WHILE;
"karar"		return REPEAT;
"l7d"		return UNTIL;
"leff"		return FOR;
"raga3"		return RETURN;


"e2ra"		return READ;
"etb3"		return PRINT;
"o5rog"		return EXIT;


[a-z] |
[A-Z]	{ /* VARIABLES are just one char*/
		yylval.sIndex = *yytext - 'a';
		return VARIABLE;
	}

0						|
[-]?[1-9][0-9]*		{ 
		yylval.iValue = atoi(yytext);
		return VALUE;
	}

[-]?[0-9]+[.e][0-9]+ 	|
[-]?[0-9]+[e]-?[0-9]+	|
[-]?[0-9]+[.][e]-?[0-9]+		{
		yylval.fValue = atof(yytext);
		return FVALUE;
	}

">="		return GE; 
"<="		return LE; 
"=="		return EQ; 
"!="		return NE; 

"&&"		return AND_AND;
"||"		return OR_OR;
"!"		return NOT;

"|"		return OR;
"&"		return AND;
"^"		return XOR;
"~"		return L_NOT;

"True"  {
	yylval.bValue = 1;
	return BVALUE;
}

"False"  {
	yylval.bValue = 0;
	return BVALUE;
}

[-()<>=+*/;{}] { 
                return yytext[0]; 
             } 

[ \t\n]+        ;       /* ignore whitespace */ 
.               yyerror("Unknown character"); 
%% 
int yywrap(void) { 
    return 1; 
} 
