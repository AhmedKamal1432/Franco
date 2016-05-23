#include <stdio.h> 
#include "sym_tb.h"
#include "y.tab.h"

static int lbl;
#define TMP "tmp"
#define SWITCH_VAR "sw_var"
int sw_exit_lbl;

Symbole *sym;

void cmp_one_top_stack();
void cmp_zero_top_stack();

int ex(nodeType *p) { 
    int lbl1, lbl2; 
    if (!p) return 0; 

    switch(p->type) { 

    case typeCon:
        if(p->dt == FloatType)
            printf("\tfpush\t%f\n", p->con.fvalue);
        else
            printf("\tpush\t%d\n", p->con.value);
        break; 

    case typeId:
        sym = get_sym(p);
        if(sym == NULL){
            break;
        }
        if(sym->dt == FloatType)
            printf("\tfpush\t%c\n", p->id.i + 'a');
        else
            printf("\tpush\t%c\n", p->id.i + 'a');  
        // printf("in type id %d %c\n", sym->dt, sym->name  + 'a');
        p->dt = sym->dt;
        break; 

    case typeOpr: 

        switch(p->opr.oper) { 
        case WHILE: 

            push_scope();

            printf("L%03d:\n", lbl1 = lbl++); 
            ex(p->opr.op[0]); 

            /* compare  "top of stack != 0"  if it's value is integer*/
            if(p->opr.op[0]->dt != BoolType)
                cmp_one_top_stack(p->opr.op[0]);

            printf("\tjz\tL%03d\n", lbl2 = lbl++); 
            ex(p->opr.op[1]); 
            printf("\tjmp\tL%03d\n", lbl1); 
            printf("L%03d:\n", lbl2); 

            pop_scope();

            break; 

        case REPEAT: 

            push_scope();

            printf("L%03d:\n", lbl1 = lbl++); 
            ex(p->opr.op[0]); 

            ex(p->opr.op[1]); 
            /* compare  "top of stack != 0"  if it's value is integer*/
            if(p->opr.op[1]->dt != BoolType)
                cmp_one_top_stack(p->opr.op[0]);

            printf("\tjnz\tL%03d\n", lbl2 = lbl++); 
            printf("\tjmp\tL%03d\n", lbl1); 
            printf("L%03d:\n", lbl2); 

            pop_scope();

            break; 

        case FOR: 

            
            ex(p->opr.op[0]); 
            printf("L%03d:\n", lbl1 = lbl++); 

            ex(p->opr.op[1]); 
            /* compare  "top of stack != 0"  if it's value is integer*/
            if(p->opr.op[1]->dt != BoolType)
                cmp_one_top_stack(p->opr.op[0]);

            printf("\tjz\tL%03d\n", lbl2 = lbl++); 

            push_scope();
            ex(p->opr.op[3]); 
            pop_scope();

            ex(p->opr.op[2]); 
            printf("\tjmp\tL%03d\n", lbl1); 
            printf("L%03d:\n", lbl2); 


            break; 

        case IF: 

            ex(p->opr.op[0]);

            /* compare  "top of stack != 0"  if it's value is integer*/
            if(p->opr.op[0]->dt != BoolType)
                cmp_one_top_stack(p->opr.op[0]);

            if (p->opr.nops > 2) { 
                /* if else */ 
                printf("\tjz\tL%03d\n", lbl1 = lbl++); 

                push_scope();
                ex(p->opr.op[1]); 
                pop_scope();

                printf("\tjmp\tL%03d\n", lbl2 = lbl++); 
                printf("L%03d:\n", lbl1); 

                push_scope();
                ex(p->opr.op[2]);
                pop_scope(); 

                printf("L%03d:\n", lbl2); 

            } else { 
                /* if */ 
                printf("\tjz\tL%03d\n", lbl1 = lbl++); 

                push_scope();
                ex(p->opr.op[1]); 
                pop_scope();

                printf("L%03d:\n", lbl1); 

            } 
            break; 

        case CASE: 
            ex(p->opr.op[0]);
            printf("\tpush\t%s\n",SWITCH_VAR ); 
            printf("\tcompEQ\n"); 
            printf("\tjz\tL%03d\n", lbl1 = lbl++); 
            
            push_scope();
            ex(p->opr.op[1]); 
            pop_scope();

            printf("\tjmp\tL%03d\n", sw_exit_lbl);
            printf("L%03d:\n", lbl1); 
            break; 

        case CASES: 
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            break; 

        case DEFAULT: 
            push_scope();
            ex(p->opr.op[0]);
            pop_scope();

            break; 

        case SWITCH: 
            // printf("in switch\n");
            ex(p->opr.op[0]);
            // save switch var
            printf("\tpop\t%s\n",SWITCH_VAR ); 
            sw_exit_lbl = lbl++;

            push_scope();
            if (p->opr.nops > 2) {
                //with default
                ex(p->opr.op[1]);
                ex(p->opr.op[2]);

            } else { 
                ex(p->opr.op[1]);
            } 
            pop_scope();

            printf("L%03d:\n", sw_exit_lbl); 
            break; 

        case PRINT:      
            ex(p->opr.op[0]); 
            printf("\tprint\n"); 
            break; 

        case '=':
            // printf("=\n");
            sym = get_sym(p->opr.op[0]);
            if(sym == NULL){
                break;
            }


            if(sym ->constant){
               /* Raise constant assigment error  */
                sprintf(decl_err,"constant assigmenet error %c", sym->name + 'a');
                yyerror(decl_err);
                // printf("constant assigmenet error %c", sym->name + 'a');
            }

            ex(p->opr.op[1]);
            /*  Switch on yhe type of lfh   */
            sym = get_sym(p->opr.op[0]);
            // printf("%d %d\n", sym->dt, p->opr.op[1]->dt);
            switch(sym->dt){
                case IntType:
                    switch(p->opr.op[1]->dt){
                        case FloatType:
                            sprintf(decl_err,"cannot assign float expression to int");
                            yyerror(decl_err);
                            break;

                        case CharType:
                            sprintf(decl_err,"Assign char to int");
                            yywarning(decl_err);
                            break;

                        case BoolType:
                           sprintf(decl_err,"Assign Bool to int");
                            yywarning(decl_err);
                             break;
 
                        default:
                                break;
                    }
                    printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
                    break;

                //////////////////////////////////////////////////////////
                case FloatType:
                    switch(p->opr.op[1]->dt){
                        case IntType:
                            sprintf(decl_err,"Assign int to float");
                            yywarning(decl_err);
                            break;

                        case CharType:
                            sprintf(decl_err,"Assign char to float");
                            yywarning(decl_err);
                            break;

                        case BoolType:
                           sprintf(decl_err,"Assign Bool to float");
                            yywarning(decl_err);
                             break;
 
                        default:
                            break;
                    }
                    printf("\tfpop\t%c\n", p->opr.op[0]->id.i + 'a');
                    break;

                ////////////////////////////////////////////////////////////
                case CharType:
                    switch(p->opr.op[1]->dt){
                        case IntType:
                            sprintf(decl_err,"Cannot Assign int expression to Char");
                            yyerror(decl_err);
                            break;

                        case FloatType:
                            sprintf(decl_err,"Cannot Assign Float expression to char");
                            yyerror(decl_err);
                            break;

                        case BoolType:
                           sprintf(decl_err,"Assign Bool to char");
                            yywarning(decl_err);
                             break;
 
                        default:
                            break;
                    }
                    printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
                    break;

                //////////////////////////////////////////////////////////
                case BoolType:
                    switch(p->opr.op[1]->dt){
                        case IntType:
                            sprintf(decl_err,"Cannot Assign int expression to bool");
                            yyerror(decl_err);
                            break;

                        case FloatType:
                            sprintf(decl_err,"Cannot Assign float expression to bool");
                            yyerror(decl_err);
                            break;

                        case CharType:
                           sprintf(decl_err,"Assign char to bool");
                            yywarning(decl_err);
                             break;
 
                        default:
                            break;
                    }
                    printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a');
                    break;
            }
    
            p->dt = dt_of_node(p->opr.op[0]);
            break; 

        /* Ignored Not implemented */
        case UMINUS:     
            ex(p->opr.op[0]); 
            printf("\tneg\n"); 
            break;

        case NUMBER:
            insert_sym(p->opr.op[0], IntType, false);
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a');
            p->dt = IntType;
            break;

        case FNUMBER:
            insert_sym(p->opr.op[0], FloatType, false);
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a'); 
            p->dt = FloatType;
            break;

        case CHAR:
            insert_sym(p->opr.op[0], CharType, false);
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
            p->dt = CharType;
            break;

        case BOOL:
            insert_sym(p->opr.op[0], BoolType, false);
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
            p->dt = BoolType;
            break;

        case CONST:
            /* TODO: these variables should raise error att assign them (constants) */
            switch(p->opr.op[0]->id.i){
                case 1:
                    printf("\tDD\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    insert_sym(p->opr.op[1]->opr.op[0], IntType, false);
                    break;
                case 2:
                    printf("\tDD\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    insert_sym(p->opr.op[1]->opr.op[0], FloatType, false);
                    break;
                case 3:
                    printf("\tDB\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    insert_sym(p->opr.op[1]->opr.op[0], CharType, false);
                    break;
                case 4:
                    printf("\tDB\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    insert_sym(p->opr.op[1]->opr.op[0], BoolType, false);
                    break;                
                default:
                    /* raise error unhandled value for costat type*/
                    break;
            }

            p->dt = dt_of_node(p->opr.op[0]);

            /* Execute assignment */
            ex(p->opr.op[1]);

            sym = get_sym(p->opr.op[1]->opr.op[0]);
            if(sym == NULL){
                /* mosta7eel te7sal */
                break;
            }
            sym->constant = true;
            // printf("end of const\n");
            break;

        case NOT:
            /* just push the value of the operand in  the stack */
            ex(p->opr.op[0]);
                switch(p->dt){
                    case FloatType:
                        sprintf(decl_err,"Not operation not permitted for float type" );
                        yyerror(decl_err);
                        break;
                    default:
                        break; 
                }
                p->dt = dt_of_node(p->opr.op[0]);
                break;

        case L_NOT:
            /* just push the value of the operand in  the stack */
            ex(p->opr.op[0]);
                switch(p->dt){
                    case FloatType:
                        sprintf(decl_err,"Not operation not permitted for float type");
                        yyerror(decl_err);
                        break;
                    default:
                        printf("\tNot\n"); break;                         
                        break; 
                }
                p->dt = dt_of_node(p->opr.op[0]);
                break;

        default: 
            ex(p->opr.op[0]); 
            ex(p->opr.op[1]);
            switch(p->dt){
                case IntType:
                    switch(p->opr.oper) { 
                    case '+':   printf("\tadd\n"); break; 
                    case '-':   printf("\tsub\n"); break;  
                    case '*':   printf("\tmul\n"); break; 
                    case '/':   printf("\tdiv\n"); break; 
                    case '%':   printf("\tmod\n"); break; 

                    case '<':
                       printf("\tcompLT\n");
                        p->dt = BoolType;
                        break; 
                    case '>':   
                        printf("\tcompGT\n"); 
                        p->dt = BoolType;
                        break; 

                    case GE:
                        printf("\tcompGE\n"); 
                        p->dt = BoolType;
                        break; 
                    case LE:
                        printf("\tcompLE\n"); 
                        p->dt = BoolType;
                        break; 
                    case NE:
                        printf("\tcompNE\n"); 
                        p->dt = BoolType;
                        break; 
                    case EQ: 
                        printf("\tcompEQ\n"); 
                        p->dt = BoolType;
                        break;

                    case AND_AND: 
                        printf("\tand\n"); 
                        p->dt = BoolType;
                        break;

                    case OR_OR: 
                        printf("\tor\n"); 
                        p->dt = BoolType;
                        break;

                    case AND: 
                        printf("\tand\n"); 
                        break;

                    case OR: 
                        printf("\tor\n"); 
                        break;

                    case XOR: 
                        printf("\txor\n"); 
                        break;
                    default :
                        break;

                    }
                    break;

                case FloatType:
                    switch(p->opr.oper) { 
                    case '+':   printf("\tfadd\n"); break; 
                    case '-':   printf("\tfsub\n"); break;  
                    case '*':   printf("\tfmul\n"); break; 
                    case '/':   printf("\tfdiv\n"); break; 

                    case '%':  
                        sprintf(decl_err,"Mod operation not permitted for float type" );
                        yyerror(decl_err);
                          break; 
                    case '<':
                       printf("\tfcompLT\n");
                        p->dt = BoolType;
                        break; 
                    case '>':   
                        printf("\tfcompGT\n"); 
                        p->dt = BoolType;
                        break; 

                    case GE:
                        printf("\tfcompGE\n"); 
                        p->dt = BoolType;
                        break; 
                    case LE:
                        printf("\tfcompLE\n"); 
                        p->dt = BoolType;
                        break; 
                    case NE:
                        printf("\tfcompNE\n"); 
                        p->dt = BoolType;
                        break; 
                    case EQ: 
                        printf("\tfcompEQ\n"); 
                        p->dt = BoolType;
                        break; 

                    /* TODO: Raise errors here*/
                    case AND_AND: 
                        sprintf(decl_err,"And operation not permitted for float type" );
                        yyerror(decl_err);
                        break;

                    case OR_OR: 
                        sprintf(decl_err,"Or operation not permitted for float type" );
                        yyerror(decl_err);
                        break;

                    case AND: 
                        sprintf(decl_err,"Logical And operation not permitted for float type" );
                        yyerror(decl_err);
                        break;

                    case OR: 
                        sprintf(decl_err,"Logical Or operation not permitted for float type" );
                        yyerror(decl_err);
                        break;

                    case XOR: 
                        sprintf(decl_err,"Logical Xor operation not permitted for float type" );
                        yyerror(decl_err);
                        break;
                    } 
                    break;
                default:   
                    /* error */
                    break;
            }
            p->dt = dt_of_children(p->opr.op[0], p->opr.op[1]);
        }
    }
    return 0; 
} 

void cmp_one_top_stack(){
        printf("\tpush\t0\n");
        printf("\tcompNE\n");
        printf("\tpop\t%s\n",TMP);
}

void cmp_zero_top_stack(){
        printf("\tpush\t0\n");
        printf("\tcompE\n");
        printf("\tpop\t%s\n",TMP);
}

