#include <stdio.h> 
#include "def.h" 
#include "y.tab.h" 
static int lbl;
#define TMP "tmp"

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
        printf("\tpush\t%c\n", p->id.i + 'a');  
        break; 

    case typeOpr: 

        switch(p->opr.oper) { 
        case WHILE: 
            printf("L%03d:\n", lbl1 = lbl++); 
            ex(p->opr.op[0]); 

            /* compare  "top of stack != 0"  if it's value is integer*/
            if(p->opr.op[0]->dt != BoolType)
                cmp_one_top_stack(p->opr.op[0]);

            printf("\tjz\tL%03d\n", lbl2 = lbl++); 
            ex(p->opr.op[1]); 
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
                ex(p->opr.op[1]); 
                printf("\tjmp\tL%03d\n", lbl2 = lbl++); 
                printf("L%03d:\n", lbl1); 
                ex(p->opr.op[2]); 
                printf("L%03d:\n", lbl2); 
            } else { 
                /* if */ 
                printf("\tjz\tL%03d\n", lbl1 = lbl++); 
                ex(p->opr.op[1]); 
                printf("L%03d:\n", lbl1); 
            } 
            break; 

        case PRINT:      
            ex(p->opr.op[0]); 
            printf("\tprint\n"); 
            break; 

        case '=':        
            ex(p->opr.op[1]); 
            printf("\tpop\t%c\n", p->opr.op[0]->id.i + 'a'); 
            break; 

        /* Ignored Not implemented */
        case UMINUS:     
            ex(p->opr.op[0]); 
            printf("\tneg\n"); 
            break;

        case NUMBER:
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case FNUMBER:
            printf("\tDD\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case CHAR:
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case BOOL:
            printf("\tDB\t%c\n",p->opr.op[0]->id.i + 'a'); 
            break;

        case CONST:
            /* TODO: these variables should raise error att assign them (constants) */
            switch(p->opr.op[0]->id.i){
                case 1:
                    printf("\tDD\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    break;
                case 2:
                    printf("\tDD\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    break;
                case 3:
                    printf("\tDB\t%c\n",p->opr.op[1]->opr.op[0]->id.i + 'a'); 
                    break;
                default:
                    /* raise error unhandled value for costat type*/
                    break;
            }

            /* Execute assignment */
            ex(p->opr.op[1]);

            /* TODO: these variables should raise error att assign them (constants) */
            break;

        case NOT:
            /* just push the value of the operand in  the stack */
            ex(p->opr.op[0]);
                switch(p->dt){
                    case FloatType:
                        /* raise error */
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
                        printf("\tand\n"); 
                        p->dt = BoolType;
                        break;

                    case AND: 
                        printf("\tand\n"); 
                        break;

                    case XOR: 
                        printf("\tXOR\n"); 
                        break;

                    }
                    break;

                case FloatType:
                    switch(p->opr.oper) { 
                    case '+':   printf("\tfadd\n"); break; 
                    case '-':   printf("\tfsub\n"); break;  
                    case '*':   printf("\tfmul\n"); break; 
                    case '/':   printf("\tfdiv\n"); break; 
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
                        break;

                    case OR_OR: 
                        break;

                    case AND: 
                        break;

                    case XOR: 
                        break;
                    } 
                    break;
                default:   
                    /* error */
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
