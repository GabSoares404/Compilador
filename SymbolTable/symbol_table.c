#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

// 1. Inicia a torre de pratos zerada
void initStack(Stack* s) {
    s->topo = -1;
    s->pos_livre = 1; // Segundo a simulação MIPS dita, variáveis assumirão (-pos*4) do $fp. Inicia no slot 1.
}

// 2. Bateu numa Chave '{': Pega um prato novo e põe na pilha
void pushScope(Stack* s) {
    if (s->topo >= MAX_ESCOPOS - 1) {
        printf("ERRO: Pilha de escopos cheia!\n");
        exit(1);
    }
    s->topo++;
    s->tabelas[s->topo] = (SymbolTable*)malloc(sizeof(SymbolTable));
    s->tabelas[s->topo]->lista = NULL; // Prato nulo, nenhuma variável declarada nele ainda!
}

// 3. Bateu numa Chave '}': Joga todo o Prato do topo fora e desce 1 andar
void popScope(Stack* s) {
    if (s->topo < 0) return;

    // Destroi fisicamente e liberta na RAM as variáveis locais que sobraram na corrente daquele chassi
    Symbol* atual = s->tabelas[s->topo]->lista;
    while (atual != NULL) {
        Symbol* proximo = atual->next;
        free(atual->nome);
        free(atual);
        atual = proximo;
    }
    
    // Agora que o lixo local foi excluído (Amnésia de Bloco), destrói a bandeja e desce o topo
    free(s->tabelas[s->topo]);
    s->topo--;
}

// 4. Se ele ler "int a;", essa função é disparada: ela adiciona no Prato mais alto sempre!
void insertSymbol(Stack* s, char* nome, int tipo) {
    if (s->topo < 0) return;

    // Cria a caixinha da variável nova
    Symbol* novavar = (Symbol*)malloc(sizeof(Symbol));
    novavar->nome = strdup(nome);
    novavar->tipo = tipo;
    novavar->pos = s->pos_livre++; // Adquire a Posição Dinâmica atual, e sobe pra próxima em escada

    // Conecta na corrente. Ela toma a vanguarda e empurra a lista velha pro ponteiro 'next'
    novavar->next = s->tabelas[s->topo]->lista;
    s->tabelas[s->topo]->lista = novavar;
}

// 5. Leu uma variavel A? O Cérebro procura do prato atual ATE O GLOBAL por segurança!
Symbol* lookup(Stack* s, char* nome) {

    // Reparou que o Loop "i" anda de trás pra frente (Desce no elevador)?!! 
    // É por isso que Variáveis Locais "ofuscam" Variáveis Globais de mesmo nome na vida real!
    for (int i = s->topo; i >= 0; i--) {
        Symbol* var = s->tabelas[i]->lista;
        
        while (var != NULL) {   // Procura na corrente das variáveis locais desse andar
            if (strcmp(var->nome, nome) == 0) {
                return var;    // Achou a desgraçada! Retorna ela.
            }
            var = var->next;
        }
    }
    return NULL; // Desceu a torre até o Zero, checou todas, e ela não existe! O usuário usou "a" sem declarar "a"!
}

// 6. Procura uma variável EXCLUSIVAMENTE no Escopo atual (Topo) para permitir Shadowing!
Symbol* lookupCurrentScope(Stack* s, char* nome) {
    if (s->topo < 0) return NULL;
    
    Symbol* var = s->tabelas[s->topo]->lista;
    while (var != NULL) {
        if (strcmp(var->nome, nome) == 0) return var;
        var = var->next;
    }
    return NULL;
}
