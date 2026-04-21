#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* ============================================================================
 * IMPLEMENTAÇÃO DA TABELA DE SÍMBOLOS E PILHA DE ESCOPOS (.C)
 * ============================================================================
 * O QUE FAZ: 
 * Este arquivo contém as funções que criam e manipulam a memória onde as
 * variáveis do compilador são guardadas (Tabela de Símbolos).
 * 
 * COMO FUNCIONA:
 * Utiliza duas estruturas clássicas:
 * 1. Tabela Hash: Para buscar o nome de uma variável rapidamente (tempo O(1)).
 * 2. Pilha (Stack): Para organizar blocos de código aninhados (escopos LIFO).
 * ============================================================================
 */

/* 
 * ----------------------------------------------------------------------------
 * 1. ALGORITMO DE HASHING DE STRINGS (hashFunction)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Transforma o nome (string) de uma variável em um número inteiro (índice), 
 * que será usado como a posição (bucket) direta no array da Tabela Hash.
 * 
 * COMO FUNCIONA:
 * - O laço `while (*nome)` percorre cada caractere da string até achar o `\0`.
 * - `h = (h << 4) + *nome++`: Desloca os bits de 'h' para a esquerda (multiplica)
 *   e soma o valor ASCII do caractere atual, avançando o ponteiro da string.
 * - `g = h & 0xF0000000`: Usa uma máscara para isolar os 4 bits mais significativos.
 * - `if (g)`: Se esses bits mais altos existirem, mistura eles de volta no hash
 *   usando a porta lógica XOR (`^= g >> 24`) visando diminuir colisões simples.
 * - `h &= ~g`: Limpa esses bits altos originais para evitar memória 'overflow'.
 * - O retorno usa `h % HASH_SIZE` para garantir que o número gerado caiba  
 *   rigorosamente dentro do tamanho limite do tamanho nosso array local.
 */
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

/* 
 * ----------------------------------------------------------------------------
 * 2. INICIALIZAÇÃO DA PILHA DE ESCOPOS (initStack)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Prepara a pilha central (Stack) para começar a hospedar os escopos do programa.
 * 
 * COMO FUNCIONA:
 * - `s->topo = -1`: Indica que a pilha está vazia. O primeiro bloco ocupará a casa 0.
 * - `s->pos_livre = 1`: Prepara a contagem global de memória para o assembly MIPS. O valor 1
 *   indica que a primeira variável declarada ficará na posição `-4($fp)`.
 */
void initStack(Stack* s) {
    s->topo = -1;
    s->pos_livre = 1; 
}

/* 
 * ----------------------------------------------------------------------------
 * 3. CRIAÇÃO DE UM NOVO ESCOPO (pushScope)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Abre e reserva memória para um nível de bloco (ex: logo após ler uma chave `{`).
 * 
 * COMO FUNCIONA:
 * - Verifica se a pilha já lotou (retorna erro limitador MAX_ESCOPOS).
 * - Aumenta o índice ativo da pilha de controle (`s->topo++`).
 * - Usa `malloc` para alocar e instanciar uma nova Tabela de Símbolos base no C.
 * - Executa um laço `for` preenchendo as posições (baldes) da nova tabela com `NULL`.
 * 
 * DETALHES IMPORTANTES:
 * Marcar os baldes com `NULL` é obrigatório para garantir que a leitura base e as colisões 
 * de hash não conflitem com restos de memória C que estavam ocupando aquela mesma RAM.
 */
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

/* 
 * ----------------------------------------------------------------------------
 * 4. ENCERRAMENTO DE UM ESCOPO (popScope)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Destrói as estruturas e apaga as variáveis de um nível de bloco finalizado (`}`).
 * 
 * COMO FUNCIONA:
 * - O laço `for` itera através de todos os índices limites da tabela do escopo atual.
 * - Em cada posição vetorial, atua com um `while (atual != NULL)` caminhando as colisões 
 *   de 'Lista Encadeada' se houverem referências presas dentro de um mesmo nó.
 * - Usa `Symbol* proximo = atual->next;` salvando sempre o acesso para a próxima variável.
 *   Esse clone é vital pois ao chamarmos o `free(atual)`, perderíamos o ponteiro sem ele!
 * - Usa `free` no nome da string e no bloco da struct.
 * - Após apagar todas as variáveis cadastradas, o array hash base é liberado com o `free` .
 * - A propriedade de visualização da pilha diminui permanentemente (`s->topo--`).
 */
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

/* 
 * ----------------------------------------------------------------------------
 * 5. INSERÇÃO NA TABELA (insertSymbol)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Grava uma declaração de variável inédita no escopo ativo alojando tipo e offset MIPS.
 * 
 * COMO FUNCIONA:
 * - Usa dinamicidade via `malloc` providenciando o nó representativo do Symbol .
 * - Clona o nome lido nativamente usando `strdup`, não permitindo que resíduos de cache 
 *   do buffer Flex/Bison afetem o registro longo retido na struct.
 * - Aloca o tipo da variável em `tipo` e resguarda irreversivelmente o `pos_livre++`.
 * - Interroga índice alvo `bucket` através do cálculo rápido feito na `hashFunction`.
 * 
 * DETALHES IMPORTANTES (Tratamento de Colisão via Lista Encadeada):
 * Usa a clássica aproximação de "Encadeamento Separado" (Chaining): Caso a conta referencial do
 * bucket seja `15` e lá já tenha referenciado antes uma variável antiga lá, o nó dessa variável 
 * atual é redirecionado apontando `->next` pra antiga. A nossa assume o topo oficial da corrente no vetor index `15`. 
 */
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

/* 
 * ----------------------------------------------------------------------------
 * 6. BUSCA DE VARIÁVEL EM TODOS OS ESCOPOS ATIVOS (lookup)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Pesquisa exaustivamente se uma referida variável literária atrelada ("id") está em foco ativo.
 * Acionada recorrentemente no Type Checking quando o código exige chamar alguém (ex: x + 2).
 * 
 * COMO FUNCIONA:
 * - Isola instantâneamente em qual endereço vetor buscar, adquirindo a chave na `hashFunction`.
 * - O fluxo de laço `for` analisa a descida retroativa `s->topo` (Tabela mais aninhada de blocos) 
 *   descendendo rumo ao base fixo `0` (Escopo das variáveis de fora/globais).
 * - Desconsidera varreduras falhas nos outros arrays. Ele entra avaliando exclusivamente 
 *   e objetivamente a lista encadeada `.next` do balde index que o Hash apontou.
 * - Usa `strcmp` dentro de colisão avaliativa pra comparar com exatidão textual C retornando o ponteiro.
 * 
 * DETALHES IMPORTANTES (Shadowing / Sombreamento de Memória):
 * O compilador permite criar variáveis locais homônimas as listagens mais afastadas `(int z = 1; { int z = 5; })`.
 * Por causa do 'for' reverso pesquisando em descida do topo, o iterador acerta sempre o 
 * valor em profundidade atrelado mais localmente ao bloco, e defende a ocultação lexical fielmente!
 */
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

/* 
 * ----------------------------------------------------------------------------
 * 7. BUSCA RESTRITA SOMENTE AO ESCOPO ATUAL (lookupCurrentScope)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Procura ocorrências da variável lida SOMENTE na listagem mais alta da pilha ativada sem acionar LIFO.
 * É o controlador ativado unicamente na hora que a regra constrói declaração nova (Evitando ex: int x, x;). 
 * 
 * COMO FUNCIONA:
 * - Tranca retornos falsos caso os domínios de `s->topo` mostrem pilhas inacessíveis `< 0`.
 * - Concede atalho O(1) gerando a `hashFunction(nome)`.
 * - Mergulha direta e apenas no limite referencial `s->tabelas[s->topo]` descartando o laço LIFO ascendente passado.
 * - Em colisão, o `while` checa de fato usando o comparativo `strcmp` restritivo listando o bucket isolado atual.
 */
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
