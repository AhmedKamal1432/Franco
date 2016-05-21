typedef enum { typeCon, typeId, typeOpr } nodeEnum; 
typedef enum { IntType, FloatType, CharType, BoolType } DataTyprEnum; 

/* constants */ 
typedef struct { 
    int value;                  /* value of constant */
    float fvalue;                  /* value of constant */
} conNodeType; 

/* identifiers */ 
typedef struct { 
    int i;                      /* subscript to sym array */ 
} idNodeType; 

/* operators */ 
typedef struct { 
    int oper;                   /* operator */ 
    int nops;                   /* number of operands */ 
    struct nodeTypeTag *op[1];  /* operands (expandable) */ 
} oprNodeType; 

typedef struct nodeTypeTag { 
    nodeEnum type;              /* type of node */ 
    DataTyprEnum dt;     /* Datatype of the constat */

    /* union must be last entry in nodeType */ 
    /* because operNodeType may dynamically increase */ 
    union { 
        conNodeType con;        /* constants */ 
        idNodeType id;          /* identifiers */ 
        oprNodeType opr;        /* operators */ 
    }; 
} nodeType; 

extern int sym[26]; 
