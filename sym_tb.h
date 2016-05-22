#include "def.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DELEMETER 0
#define MAX_SYM_COUNT 100

/* variables of our program */
typedef struct {
    char name;
    DataTyprEnum dt;
}Symbole;

/* global variable*/
Symbole *sym_tb[MAX_SYM_COUNT];                    /* symbol table */
int sym_count;

Symbole *get_sym(nodeType *p){
	// printf("search for variable %c\n",p->id.i +'a');
    int i = sym_count-1;
    for(;i >=0; i--){
        // printf("%d\n",i );
        if(sym_tb[i]->name == p->id.i){
            // found
		// printf("found  %d\n", i);
            return sym_tb[i];
        }
    }
    printf("\terror: Variable %c not declared before\n", p->id.i + 'a');
    // compilation error for not defined
    return NULL;
}

bool insert_sym(nodeType *p, DataTyprEnum dt){
    int i = sym_count - 1;
    for(;i >= 0; i--){
        if(sym_tb[i]->name == p->id.i){
		printf("\terror: Variable %c definded before\n", sym_tb[i]->name + 'a');
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

    sym_tb[sym_count] = s;
    sym_count++;
    // printf("%d\n",sym_count );
    return true;
}

