#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* Limite máximo predefinido de escopos (scopes) aninhados suportados pelo compilador */
#define MAX_ESCOPOS 100 

/* Tamanho da tabela hash (número primo utilizado para reduzir a probabilidade de colisões) */
#define HASH_SIZE 97 

typedef struct Symbol {
    char* nome;
    int tipo; 
    int pos;  
    struct Symbol* next; 
} Symbol;

typedef struct SymbolTable {
    Symbol* baldes[HASH_SIZE]; // Array atuando como Buckets
} SymbolTable;

typedef struct Stack {
    SymbolTable* tabelas[MAX_ESCOPOS];
    int topo;
    int pos_livre; 
} Stack;

void initStack(Stack* s);
void pushScope(Stack* s);
void popScope(Stack* s);
void insertSymbol(Stack* s, char* nome, int tipo);
Symbol* lookup(Stack* s, char* nome);
Symbol* lookupCurrentScope(Stack* s, char* nome);

#endif
