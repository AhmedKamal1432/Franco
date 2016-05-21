%{
void yyerror (char *s);
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>
int errors;
#include "cg.h"
#include "st.h"   


void install ( char *sym_name )
{
	symrec *s;
	s = getsym (sym_name);
	if (s == 0)
		s = putsym (sym_name);
	else{
		errors++;
		printf( "%s is already defined\n", sym_name );
	}
}
%}

%union {char * id;int num;float fnum;char * name;};
%token COLON NAME NUMBER FNUMBER CONST
%token <name> STRING
%token <num> VALUE
%token <fnum> FVALUE
%token <id> IDENTIFIER
%token INC DEC S_OP L_OP SE_OP LE_OP E_OP NE_OP AND OR
%token t3refat brnamg _5lsana NL SC

%token CASE DEFAULT IF THEN ELSEIF ELSE SWITCH WHILE REPEAT UNTIL FOR GOTO CONTINUE BREAK RETURN
%token READ PRINT EXIT

%left '+' '-'
%left '*' '/'

%type <fnum> exp term

%start program
%%


/* descriptions of expected inputs     corresponding actions (in C) */

program 		: t3refat
					decl
		  		  brnamg
		  		  	code
		  		  _5lsana   { gen_code( HALT, "","","" );YYACCEPT;return;};

decl 			: decl dec {putval();count = 0;}
				| dec {putval();count = 0;};

dec 			: s_dec {printf("correct statement\n");tp = 's';} 
				| i_dec {printf("correct statement\n");tp = 'i';}
				| f_dec {printf("correct statement\n");tp = 'f';}
				| c_dec {printf("correct statement\n");tp = 'c';};

s_dec			: NAME COLON IDENTIFIERS s_dec_def;
i_dec			: NUMBER COLON IDENTIFIERS i_dec_def;
f_dec			: FNUMBER COLON IDENTIFIERS f_dec_def | FNUMBER COLON IDENTIFIERS i_dec_def;
c_dec			: CONST COLON IDENTIFIERS f_dec_def | CONST COLON IDENTIFIERS i_dec_def;

s_dec_def		: COLON STRING {type1=(char *) malloc (strlen($2)+1);strcpy (type1,$2);type = 1;};
i_dec_def		: COLON VALUE  {type2 = $2;type = 2;};
f_dec_def		: COLON FVALUE {type3 = $2;type = 3;};

IDENTIFIERS		: IDENTIFIERS ',' IDENTIFIER {install($3);}
				| IDENTIFIER {install($1);};

code 			: code line
				| line;

line 			: assignment;

assignment		: IDENTIFIER COLON exp SC {putval2($3);printf("correct statement\n");};

exp    			: '(' exp ')'
       			| exp '+' exp      	{gen_code(ADD,"","","tmp");}
       			| exp '-' exp    	{gen_code(SUB,"","","tmp");}
       			| exp '*' exp      	{gen_code(MULT,"","","tmp");}
       			| exp '/' exp   	{gen_code(DIV,"","","tmp");}
       			| term;				
term   			: VALUE             {type = 2;gen_code(LD_INT,"","","");}
				| FVALUE            {type = 3;gen_code(LD_FLOAT,"","","");}     
				| IDENTIFIER 		{gen_code(LD_VAR,"","","");};	

%%                     /* C code */




main()
{
errors = 0;
//printf("%d\n\n",offsets[10]);
yyparse ();
printf ( "Parse Completed\n\n");
if(errors == 0){
	print_code();
}
}

void yyerror (char *s) {errors++;fprintf (stderr, "%s\n", s);} 
