#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_ESCOPOS 100 // O nosso limite de chaves dentro de chaves

// 1. A menor parte: O registro de uma variável simples. 
// Ela usa "next" pois funciona como uma corrente (Linked List) com outras variáveis.
typedef struct Symbol {
    char* nome;
    int tipo; // Receberá a etiqueta de Int ou Car lá do Bison
    int pos;  // Posição calculada para ser traduzida em endereço MIPS (-pos * 4) do Frame Pointer
    struct Symbol* next; // Ponteiro para a próxima variável da mesma lista
} Symbol;

// 2. O Escopo (O nosso Prato). Ele apenas aponta pro início daquela corrente acima.
typedef struct SymbolTable {
    Symbol* lista;
} SymbolTable;

// 3. A Pilha (A torre de Pratos). Registra a altura atual no "topo", e os pratos numa matriz.
typedef struct Stack {
    SymbolTable* tabelas[MAX_ESCOPOS];
    int topo;
    int pos_livre; // Controlador de offset físico consecutivo pra arquitetura MIPS
} Stack;

// ASSINATURAS DAS NOSSAS FUNÇÕES MÁGICAS PARA O PRÓXIMO PASSO
void initStack(Stack* s);
void pushScope(Stack* s);
void popScope(Stack* s);
void insertSymbol(Stack* s, char* nome, int tipo);
Symbol* lookup(Stack* s, char* nome);
Symbol* lookupCurrentScope(Stack* s, char* nome);

#endif
