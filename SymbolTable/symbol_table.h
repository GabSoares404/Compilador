#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* Limite máximo predefinido de escopos (scopes) aninhados suportados pelo compilador */
#define MAX_ESCOPOS 100 

/* Tamanho da tabela hash (número primo utilizado para reduzir a probabilidade de colisões) */
#define HASH_SIZE 97 

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] ARQUITETURA DA TABELA DE SÍMBOLOS (CABEÇALHO)
 * ============================================================================
 * O QUE É: Este arquivo define as estruturas de dados centrais responsáveis por 
 * gerenciar as variáveis e seus respectivos contextos ao longo da compilação: 
 * o registro de Símbolo (Symbol), a Tabela de Símbolos local (Symbol Table)
 * e o gerenciador central via Pilha de Escopos (Stack).
 * 
 * PARA QUE SERVE: A Tabela de Símbolos acompanha a validade (determinado pelo escopo),
 * a tipagem atrelada e a localização física na memória de cada variável do programa. 
 * É consultada iterativamente pelo Analisador Semântico para checagem de regras de nomeação
 * e acionada pelo Gerador de Código para descobrir o offset físico no assembly MIPS. 
 * Usando uma Tabela Hash, o custo de busca do identificador ocorre, em média, em tempo O(1).
 * ============================================================================
 */

/* 
 * ----------------------------------------------------------------------------
 * 1. REGISTRO DE SÍMBOLO (REPRESENTAÇÃO DE VARIÁVEL UNITÁRIA E COLISÃO)
 * ----------------------------------------------------------------------------
 * Armazena as propriedades contínuas de uma única variável mapeada em um escopo.
 * Esta estrutura também incorpora o comportamento intrínseco de um Nó de Lista 
 * Encadeada para resolver colisões na Hash Table através de encadeamento separado (Chaining).
 * 
 * CAMPOS RELEVANTES:
 * - nome: O identificador em cadeia de texto (string) lido no código-fonte.
 * - tipo: A tag de formatação da variável inferida pelo Parser (ex: token de INT ou CAR).
 * - pos: O offset (deslocamento fixo) alocado linearmente para simulação espacial. 
 *        Garante e indica a posição final da variável atrelada ao Frame Pointer do MIPS: (-pos * 4)($fp).
 * - next: Ponteiro hierárquico direcionado ao próximo símbolo armazenado com colisão matemática
 *         no mesmo bucket da Tabela Hash.
 */
typedef struct Symbol {
    char* nome;
    int tipo; 
    int pos;  
    struct Symbol* next; 
} Symbol;

/* 
 * ----------------------------------------------------------------------------
 * 2. TABELA DE SÍMBOLOS LOCAL (CONTEÚDO E GESTOR DE UM ESCOPO EXCLUSIVO)
 * ----------------------------------------------------------------------------
 * A estrutura que materializa as variáveis atreladas restritamente a um bloco (um escopo fechado).
 * O modelo é composto pelo formato de uma Tabela Hash indexada de forma linear.
 * 
 * CAMPOS RELEVANTES:
 * - baldes: Um array estático (vetor de ponteiros) com limites demarcados baseando-se em `HASH_SIZE`. 
 *           Cada índice deste array representa um respectivo Bucket e hospeda o topo (Cabeça) de 
 *           suas próprias listas resultantes do algorítmo hash e da política de encadeamento local.
 */
typedef struct SymbolTable {
    Symbol* baldes[HASH_SIZE]; // Array atuando como Buckets
} SymbolTable;

/* 
 * ----------------------------------------------------------------------------
 * 3. PILHA DE ESCOPOS (O GERENCIADOR CENTRAL ALOCADO COM LIFO)
 * ----------------------------------------------------------------------------
 * Contêiner encarregado de viabilizar a hierarquia das múltiplas Tabelas Hash criadas,
 * coordenando contextualmente a visibilidade local usando empilhamento estrito LIFO (Pilha). 
 * Ela rege a sobreposição funcional momentânea de diferentes blocos chamada de Sombreamento (Shadowing).
 * 
 * CAMPOS RELEVANTES:
 * - tabelas: Array de preenchimento alocacional suportando a vida das instâncias sobrejacentes de escopos no código.
 * - topo:    O indexador primário (Inteiro marcador LIFO) sinalizando qual nível/casa deste arranjo é 
 *            atualmente o andar superior ativado (o ambiente léxico mais enclausurado em processamento).
 * - pos_livre: Controlador de formatação arquitetural contábil. Diferente dos escopos que encerram e são dados pop(),
 *              esta restrição cresce irreversível para toda e qualquer variável de qualquer profundidade garantindo
 *              um offset absoluto e irrepetível na simulação unitária sem em sobreposições para a finalização MIPS.
 */
typedef struct Stack {
    SymbolTable* tabelas[MAX_ESCOPOS];
    int topo;
    int pos_livre; 
} Stack;

/* ============================================================================
 * 4. ASSINATURAS DAS OPERAÇÕES DO ENGENHO DA TABELA (DISPONIBILIDADE ESTÁTICA)
 * ============================================================================
 */
void initStack(Stack* s);
void pushScope(Stack* s);
void popScope(Stack* s);
void insertSymbol(Stack* s, char* nome, int tipo);
Symbol* lookup(Stack* s, char* nome);
Symbol* lookupCurrentScope(Stack* s, char* nome);

#endif
