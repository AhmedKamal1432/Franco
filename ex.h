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
           /* Raise Compilation error */
            break;
        }
        if(sym->dt = FloatType)
            printf("\tfpush\t%c\n", p->id.i + 'a');
        else
            printf("\tpush\t%c\n", p->id.i + 'a');  
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

            sym = get_sym(p->opr.op[0]);
            if(sym == NULL){
                /* symbol not found*/
                break;
            }


            if(sym ->constant){
               /* Raise constant assigment error  */
                printf("constant assigmenet error %c\n", sym->name + 'a');
            }

            ex(p->opr.op[1]);
            if(p->dt == FloatType)
                printf("\tfpop\t%c\n", p->opr.op[0]->id.i + 'a'); 
            else
                printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a'); 
            break; 

        /* Ignored Not implemented */
        case UMINUS:     
            ex(p->opr.op[0]); 
            printf("\tneg\n"); 
            break;

        case NUMBER:
            insert_sym(p->opr.op[0], IntType, false);
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case FNUMBER:
            insert_sym(p->opr.op[0], FloatType, false);
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case CHAR:
            insert_sym(p->opr.op[0], CharType, false);
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case BOOL:
            insert_sym(p->opr.op[0], BoolType, false);
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
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
                        /* raise error */
                        yyerror("Can't execute logical operation on float type");
                        break;
                    default:
                        break; 
                }
                break;

        case L_NOT:
            /* just push the value of the operand in  the stack */
            ex(p->opr.op[0]);
                switch(p->dt){
                    case FloatType:
                        /* raise error */
                        yyerror("Can't execute logical operation on float type");
                        break;
                    default:
                        printf("\tNot\n"); break;                         
                        break; 
                }
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
                         /*   raise error cannot make % with float variables  */
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
                    yyerror("Can't execute logical operation on float type");
                        break;

                    case OR_OR: 
                    yyerror("Can't execute logical operation on float type");
                        break;

                    case AND: 
                    yyerror("Can't execute logical operation on float type");
                        break;

                    case OR: 
                    yyerror("Can't execute logical operation on float type");
                        break;

                    case XOR: 
                    yyerror("Can't execute logical operation on float type");
                        break;
                    } 
                    default:    
                        break;
            }
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

