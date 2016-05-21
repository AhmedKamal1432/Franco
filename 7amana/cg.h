enum code_ops { HALT, STORE, DATA, LD_INT, LD_FLOAT, LD_VAR, ADD, SUB, MULT, DIV };

char *op_name[] = {"halt","st","data", "ldi", "ldf","ldv","add", "sub", "mult", "div"};

int code_offset = 0;

struct instruction
{
enum code_ops op;
char *arg1,*arg2,*dest;
int x;
};

struct instruction code[999];

void gen_code( enum code_ops operation, char* arg1, char* arg2, char* dest ){
	code[code_offset].op    = operation;
	code[code_offset].x		= 7;
	
	if(strlen(arg1) != 0){
		code[code_offset].arg1=(char *) malloc (strlen(arg1));
		strcpy(code[code_offset].arg1, arg1);
	}
	else{
		code[code_offset].x = code[code_offset].x & (~4);
	}
	if(strlen(arg2) != 0){
		code[code_offset].arg2=(char *) malloc (strlen(arg2));
		strcpy(code[code_offset].arg2, arg2);
	}
	else {
		code[code_offset].x = code[code_offset].x & (~2);
	}
	if(strlen(dest) != 0){
		code[code_offset].dest=(char *) malloc (strlen(dest));
		strcpy(code[code_offset].dest, dest);
	}
	else {
		code[code_offset].x = code[code_offset].x & (~1);
	}
	code_offset++;
}

void print_code()
{
printf("Generated Code:\n\n");
int i = 0;
	while (i < code_offset) {
		if(code[i].x == 7)
			printf("%3d: %-10s%s  %s  %s\n",i,op_name[(int) code[i].op], code[i].arg1, code[i].arg2, code[i].dest);
		else if(code[i].x == 3)
			printf("%3d: %-10s%s  %s\n",i,op_name[(int) code[i].op], code[i].arg2, code[i].dest);
		else if(code[i].x == 5)
			printf("%3d: %-10s%s  %s\n",i,op_name[(int) code[i].op], code[i].arg1, code[i].dest);
		else if(code[i].x == 1)
			printf("%3d: %-10s%s\n",i,op_name[(int) code[i].op], code[i].dest);
		else if(code[i].x == 6)
			printf("%3d: %-10s%s  %s\n",i,op_name[(int) code[i].op], code[i].arg1, code[i].arg2);
		else if(code[i].x == 2)
			printf("%3d: %-10s%s\n",i,op_name[(int) code[i].op], code[i].arg2);
		else if(code[i].x == 4)
			printf("%3d: %-10s%s\n",i,op_name[(int) code[i].op], code[i].arg1);
		else if(code[i].x == 0)
			printf("%3d: %-10s\n",i,op_name[(int) code[i].op]);
		
		i++;
	}
}