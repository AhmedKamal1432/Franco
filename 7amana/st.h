int offsets[20] = {0};
int count = 0;
int data_offset = 1; 
char buff[20];
int type = 0;
char tp = 'a';
int const_type;   
char* type1;
int type2;
float type3;      

int data_location()   {
	return data_offset++;
}

union Data {
   int i;
   float f;
   char* str;
};

struct symrec{
	char *name;
	int offset;
	int type;   /*{1 -> string , 2 -> int, 3-> float}*/
	char tp;	/* for constat integer or float */
	union Data data; /* have the actual data*/
	struct symrec *next;
};

typedef struct symrec symrec;
symrec *identifier;


symrec *sym_table = (symrec *)0;


symrec * get2 (int of){
	symrec *ptr;
	for ( ptr =  sym_table; ptr != (symrec *) 0; ptr =  (symrec *)ptr->next )
		if (ptr->offset == of)
		return ptr;
	return 0;
}

void putval2 (float f)
{

}

void putval ()
{
	symrec *p;
	int i,tmp;
	for(i=1;i<=count;i++){
		tmp = offsets[i];
		p = get2 (tmp);
		if ( p == 0 ){
			errors++;
			printf( "%s\n", "undeclared identifier"  );
		}
		else{
			if(type == 1){
				p->tp = 's';
				p->type = 1;
				p->data.str = (char *) malloc (strlen(type1));
				strcpy (p->data.str,type1);
				printf("data: %s\n", p->data.str);
				gen_code( STORE, p->data.str,"",p->name );
			}
			else if (type == 2 && tp == 'i')
			{
				p->tp = 'i';
				p->type = 2;
				p->data.i = type2;
				printf("data: %d\n", p->data.i);
				snprintf(buff, 20,"%d",p->data.i);
				gen_code( STORE, buff,"",p->name );
			}
			else if (type == 2 && tp == 'f')
			{
				p->tp = 'f';
				p->type = 2;
				p->data.f=type2;
				printf("data: %f\n", p->data.f);
				snprintf(buff, 20, "%f", p->data.f);
				gen_code( STORE, buff,"",p->name );
			}
			else if (type == 2 && tp == 'c')
			{
				p->tp = 'c';
				p->type = 2;
				p->data.i =type2;
				printf("data: %d\n", p->data.i);
				snprintf(buff, 20,"%d",p->data.i);
				gen_code( STORE, buff,"",p->name );
			}
			else if (type == 3 && tp == 'f')
			{
				p->tp = 'f';
				p->type = 3;
				p->data.f=type3;
				printf("data: %f\n", p->data.f);
				snprintf(buff, 20, "%f", p->data.f);
				gen_code( STORE, buff,"",p->name );
			}
			else if (type == 3 && tp == 'c')
			{
				p->tp = 'c';
				p->type = 3;
				p->data.f=type3;
				printf("data: %f\n", p->data.f);
				snprintf(buff, 20, "%f", p->data.f);
				gen_code( STORE, buff,"",p->name );
			}
		}
	}
}


symrec * putsym (char *sym_name){
	count++;
	//printf("count: %d\n",count);
	symrec *ptr;
	printf("variable name -> %s\n",sym_name);
	ptr = (symrec *) malloc (sizeof(symrec));
	ptr->name = (char *) malloc (strlen(sym_name)+1);
	strcpy (ptr->name,sym_name);
	ptr->offset = data_location();
	offsets[count] = ptr->offset;
	ptr->next = (struct symrec *)sym_table;
	sym_table = ptr;
	return ptr;
}


symrec * getsym (char *sym_name){
	symrec *ptr;
	
	for ( ptr =  sym_table; ptr != (symrec *) 0; ptr =  (symrec *)ptr->next )
		if (strcmp (ptr->name,sym_name) == 0)
			return ptr;
	return 0;
}

