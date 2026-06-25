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
    s->escopo_atual = -1; /* [GV2]: Inicializa para que o primeiro pushScope seja 0 (Global) */
}

void pushScope(Stack* s) {
    if (s->topo >= MAX_ESCOPOS - 1) {
        printf("ERRO: Pilha de escopos cheia!\n");
        exit(1);
    }
    
    s->topo++;
    s->tabelas[s->topo] = (SymbolTable*)malloc(sizeof(SymbolTable));
    s->escopo_atual++; /* [GV2]: Entra em novo nível de escopo */
    
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
            
            /* [GV2]: Limpar lista de parâmetros formalizada se for uma função */
            if (atual->kind == SYM_FUNC && atual->params != NULL) {
                Param* p = atual->params;
                while (p != NULL) {
                    Param* next_p = p->next;
                    free(p->nome);
                    free(p);
                    p = next_p;
                }
            }
            
            free(atual->nome); 
            free(atual);
            
            atual = proximo; 
        }
    }
    
    free(s->tabelas[s->topo]);
    s->topo--;
    s->escopo_atual--; /* [GV2]: Volta o nível do escopo */
}

/* [GV2]: LIMPEZA TOTAL DA PILHA */
void destroyStack(Stack* s) {
    while (s->topo >= 0) {
        popScope(s);
    }
}

void insertSymbol(Stack* s, char* nome, int tipo) {
    if (s->topo < 0) return;

    Symbol* novavar = (Symbol*)malloc(sizeof(Symbol));
    
    novavar->nome = strdup(nome);
    novavar->tipo = tipo;
    novavar->pos = s->pos_livre++; 
    
    /* [GV2]: Inicializando campos do novo Symbol */
    novavar->escopo = s->escopo_atual;
    novavar->kind = SYM_VAR;
    novavar->is_vetor = 0;
    novavar->tamanho_vetor = 0;
    novavar->num_params = 0;
    novavar->tipo_retorno = 0;
    novavar->params = NULL;

    unsigned int bucket = hashFunction(nome);

    novavar->next = s->tabelas[s->topo]->baldes[bucket];
    s->tabelas[s->topo]->baldes[bucket] = novavar;
}

/* [GV2]: INSERÇÃO DE VETOR */
void insertVetor(Stack* s, char* nome, int tipo, int tamanho) {
    if (s->topo < 0) return;

    Symbol* novavar = (Symbol*)malloc(sizeof(Symbol));
    
    novavar->nome = strdup(nome);
    novavar->tipo = tipo;
    /* Um vetor de N elementos ocupa N espaços */
    novavar->pos = s->pos_livre;
    s->pos_livre += tamanho; 
    
    novavar->escopo = s->escopo_atual;
    novavar->kind = SYM_VAR;
    novavar->is_vetor = 1;
    novavar->tamanho_vetor = tamanho;
    novavar->num_params = 0;
    novavar->tipo_retorno = 0;
    novavar->params = NULL;

    unsigned int bucket = hashFunction(nome);
    novavar->next = s->tabelas[s->topo]->baldes[bucket];
    s->tabelas[s->topo]->baldes[bucket] = novavar;
}

/* [GV2]: INSERÇÃO DE FUNÇÃO */
void insertFunction(Stack* s, char* nome, int num_params, int tipo_retorno, Param* params) {
    if (s->topo < 0) return;

    Symbol* nova_func = (Symbol*)malloc(sizeof(Symbol));
    
    nova_func->nome = strdup(nome);
    nova_func->tipo = tipo_retorno; 
    nova_func->pos = 0; /* Funções não usam posição na stack local */
    
    nova_func->escopo = s->escopo_atual;
    nova_func->kind = SYM_FUNC;
    nova_func->is_vetor = 0;
    nova_func->tamanho_vetor = 0;
    nova_func->num_params = num_params;
    nova_func->tipo_retorno = tipo_retorno;
    nova_func->params = params;

    unsigned int bucket = hashFunction(nome);
    nova_func->next = s->tabelas[s->topo]->baldes[bucket];
    s->tabelas[s->topo]->baldes[bucket] = nova_func;
}

/* [GV2]: INSERÇÃO DE PARÂMETRO FORMAL */
void insertParam(Stack* s, char* nome, int tipo, int pos, int is_vetor) {
    if (s->topo < 0) return;

    Symbol* novo_param = (Symbol*)malloc(sizeof(Symbol));
    
    novo_param->nome = strdup(nome);
    novo_param->tipo = tipo;
    novo_param->pos = pos; /* Calculado pela semântica (offset na frame MIPS) */
    
    novo_param->escopo = s->escopo_atual;
    novo_param->kind = SYM_PARAM;
    novo_param->is_vetor = is_vetor;
    novo_param->tamanho_vetor = 0;
    novo_param->num_params = 0;
    novo_param->tipo_retorno = 0;
    novo_param->params = NULL;

    unsigned int bucket = hashFunction(nome);
    novo_param->next = s->tabelas[s->topo]->baldes[bucket];
    s->tabelas[s->topo]->baldes[bucket] = novo_param;
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
