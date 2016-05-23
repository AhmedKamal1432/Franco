#include "def.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DELEMETER (Symbole*) 5
#define MAX_SYM_COUNT 100

char decl_err[40];
void yyerror(char *s); 
void yywarning(char *s); 


/* variables of our program */
typedef struct {
    char name;
    DataTyprEnum dt;
    bool constant;
}Symbole;

/* global variable*/
Symbole *sym_tb[MAX_SYM_COUNT];                    /* symbol table */
int sym_count;

Symbole *get_sym(nodeType *p){
	// printf("search for variable %c\n",p->id.i +'a');
    int i = sym_count-1;
    for(;i >=0; i--){
        if(sym_tb[i] == DELEMETER)
        	continue;
        // printf("%d\n",i );
        if(sym_tb[i]->name == p->id.i){
            // found
	// printf("found  %c type = %d\n", sym_tb[i]->name+ 'a',sym_tb[i]->dt );
            return sym_tb[i];
        }
    }
    sprintf(decl_err,"Variable %c used before being defined", p->id.i + 'a');
    yyerror(decl_err);
    // compilation error for not defined
    return NULL;
}

bool insert_sym(nodeType *p, DataTyprEnum dt, bool constant){
    int i = sym_count - 1;
    for(;i >= 0; i--){
        if(sym_tb[i] == DELEMETER)
        	continue;
        if(sym_tb[i]->name == p->id.i){
		sprintf(decl_err,"Variable %c defined before", p->id.i + 'a');
        yyerror(decl_err);
            // compilation erorr declared before
		return false;
        }
    }
    if(sym_count >= MAX_SYM_COUNT ){
	// compilation error max count of symboles
	return false;
    }

    Symbole *s;
    s = malloc(sizeof(Symbole)) ;
    s->name = p->id.i;
    s->dt = dt;
    s->constant  = false;

    sym_tb[sym_count] = s;
    sym_count++;
    // printf("insert Symbole %c sym_count = %d\n",p->id.i+'a', sym_count );
    return true;
}

bool push_scope(){
	sym_tb[sym_count] = DELEMETER;
	sym_count++;
	// printf("push_scope sym_count = %d\n",sym_count);
}

bool pop_scope(){
    int i = sym_count - 1;
    for(;i >= 0; i--){
        sym_count--;
        if(sym_tb[i]  == DELEMETER){
        	sym_tb[i] == NULL;
        	break;
        }
    }
	// printf("pop_scope sym_count = %d\n",sym_count);
}


DataTyprEnum dt_of_node(nodeType *p){
    if(p == NULL)
        return IntType;
    return p->dt;
}

DataTyprEnum dt_of_children(nodeType *p1, nodeType *p2){
    if(p1 == NULL || p2 == NULL)
        return IntType;

    if(dt_of_node(p1) == FloatType || dt_of_node(p2) == FloatType)
        return FloatType;
    return IntType;
}
