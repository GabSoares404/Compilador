#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* Limite máximo predefinido de escopos (scopes) aninhados suportados pelo compilador */
#define MAX_ESCOPOS 100 

/* Tamanho da tabela hash (número primo utilizado para reduzir a probabilidade de colisões) */
#define HASH_SIZE 97 

typedef struct Param {
    char* nome;
    int tipo;           /* 0=int, 1=car */
    int is_vetor;       /* 1 se for vetor */
    struct Param* next;
} Param;

/* [GV2] TIPOS DE SÍMBOLOS (SymbolKind)*/
typedef enum {
    SYM_VAR,
    SYM_FUNC,
    SYM_PARAM
} SymbolKind;

typedef struct Symbol {
    char* nome;
    int tipo; 
    int pos;  
    
    /* [GV2] NOVOS CAMPOS PARA SUPORTE À FUNÇÕES E VETORES */
    int escopo;
    SymbolKind kind;
    
    int is_vetor;
    int tamanho_vetor;
    
    /* Só para funções */
    int num_params;
    int tipo_retorno;
    Param* params;
    
    struct Symbol* next; 
} Symbol;

typedef struct SymbolTable {
    Symbol* baldes[HASH_SIZE]; // Array atuando como Buckets
} SymbolTable;

typedef struct Stack {
    SymbolTable* tabelas[MAX_ESCOPOS];
    int topo;
    int pos_livre; 
    /* [GV2] CONTROLE DE ESCOPO GLOBAL VS LOCAL */
    int escopo_atual;
} Stack;

/* 4. ASSINATURAS DAS OPERAÇÕES DA TABELA */
void initStack(Stack* s);
void pushScope(Stack* s);
void popScope(Stack* s);
void destroyStack(Stack* s); 

/* Inserção de variáveis normais */
void insertSymbol(Stack* s, char* nome, int tipo);

/* Inserção de vetores */
void insertVetor(Stack* s, char* nome, int tipo, int tamanho);

/* Inserção de funções */
void insertFunction(Stack* s, char* nome, int num_params, int tipo_retorno, Param* params);

/* Inserção de parâmetros formais no escopo */
void insertParam(Stack* s, char* nome, int tipo, int pos, int is_vetor);

Symbol* lookup(Stack* s, char* nome);
Symbol* lookupCurrentScope(Stack* s, char* nome);

#endif
