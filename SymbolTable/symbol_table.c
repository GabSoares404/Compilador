#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static unsigned int hashFunction(char* nome) {
    unsigned int h = 0;
    
    while (*nome) {
        h = (h << 4) + *nome++;
        unsigned int g = h & 0xF0000000;
        
        if (g) {
            h ^= g >> 24;
        }
        
        h &= ~g;
    }
    
    return h % HASH_SIZE; 
}

void initStack(Stack* s) {
    s->topo = -1;
    s->pos_livre = 1; 
}

void pushScope(Stack* s) {
    if (s->topo >= MAX_ESCOPOS - 1) {
        printf("ERRO: Pilha de escopos cheia!\n");
        exit(1);
    }
    
    s->topo++;
    s->tabelas[s->topo] = (SymbolTable*)malloc(sizeof(SymbolTable));
    
    for (int i = 0; i < HASH_SIZE; i++) {
        s->tabelas[s->topo]->baldes[i] = NULL;
    }
}

void popScope(Stack* s) {
    if (s->topo < 0) return;

    for (int i = 0; i < HASH_SIZE; i++) {
        Symbol* atual = s->tabelas[s->topo]->baldes[i];
        
        while (atual != NULL) {
            Symbol* proximo = atual->next;
            
            free(atual->nome); 
            free(atual);
            
            atual = proximo; 
        }
    }
    
    free(s->tabelas[s->topo]);
    s->topo--;
}

void insertSymbol(Stack* s, char* nome, int tipo) {
    if (s->topo < 0) return;

    Symbol* novavar = (Symbol*)malloc(sizeof(Symbol));
    
    novavar->nome = strdup(nome);
    novavar->tipo = tipo;
    novavar->pos = s->pos_livre++; 

    unsigned int bucket = hashFunction(nome);

    novavar->next = s->tabelas[s->topo]->baldes[bucket];
    s->tabelas[s->topo]->baldes[bucket] = novavar;
}

Symbol* lookup(Stack* s, char* nome) {
    unsigned int bucket = hashFunction(nome);
    
    for (int i = s->topo; i >= 0; i--) {
        Symbol* var = s->tabelas[i]->baldes[bucket]; 
        
        while (var != NULL) {   
            if (strcmp(var->nome, nome) == 0) return var;
            
            var = var->next;
        }
    }
    
    return NULL; 
}

Symbol* lookupCurrentScope(Stack* s, char* nome) {
    if (s->topo < 0) return NULL;
    
    unsigned int bucket = hashFunction(nome);
    Symbol* var = s->tabelas[s->topo]->baldes[bucket];
    
    while (var != NULL) {
        if (strcmp(var->nome, nome) == 0) return var;
        
        var = var->next;
    }
    
    return NULL;
}
