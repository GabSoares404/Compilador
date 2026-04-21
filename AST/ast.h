#ifndef AST_H
#define AST_H

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] ARQUITETURA DA ÁRVORE SINTÁTICA ABSTRATA (AST)
 * ============================================================================
 * O QUE É: Este arquivo de cabeçalho (.h) define a estrutura de dados principal do 
 * compilador: a Árvore Sintática Abstrata (AST). A AST é a representação em memória 
 * do código-fonte, estruturada de forma hierárquica.
 * 
 * PARA QUE SERVE: Após o Lexer identificar os tokens e o Parser verificar a sintaxe,
 * o compilador precisa construir um modelo estrutural do código para as próximas fases.
 * A AST abstrai detalhes irrelevantes (como parênteses ou ponto e vírgula) e mantém
 * apenas a lógica do programa. Cada parte estrutural (uma declaração, uma expressão)
 * se torna um "Nó" dessa árvore. A AST inteira será percorrida em pós-ordem pelo analisador 
 * semântico e pelo gerador de código.
 * 
 * ============================================================================
 * 1. CATEGORIAS DE NÓS (COMO LER A TIPAGEM DA AST)
 * ============================================================================
 * O enum NodeType classifica o papel semântico de cada nó na árvore.
 * As etapas seguintes de análise e geração de código usam este tipo (switch/case) 
 * para determinar como processar cada bloco de execução.
 * 
 * DICIONÁRIO DE TIPOS DE NÓS:
 * -> NODE_PROGRAMA: Nó raiz e ponto de entrada absoluto do programa compilado.
 * -> NODE_BLOCO: Representa um escopo léxico ({ ... }), agrupando comandos e variáveis locais.
 * -> NODE_DECL_VAR: Subárvore que agrupa processos de declaração de variáveis.
 * -> NODE_LISTA_COMANDO: Nó recursivo (invisível na sintaxe) que liga subcomandos sequenciais.
 * -> NODE_COMANDO: Ações e fluxos de controle gerais (atribuições, if, while, leia, escreva).
 * -> NODE_OP: Representa junções por operações lógicas ou matemáticas (+, -, *, <, >).
 * -> NÓS FOLHA: NODE_IDENTIFICADOR, NODE_INTCONST, NODE_CARCONST. Ficam sempre nas extremidades 
 *    mais distantes da árvore e armazenam os valores literais crus vindos do Lexer.
 * ============================================================================
 */
typedef enum {
    NODE_PROGRAMA,
    NODE_BLOCO,
    NODE_DECL_VAR,
    NODE_LISTA_COMANDO,
    NODE_COMANDO,
    NODE_EXPR,
    NODE_IDENTIFICADOR,
    NODE_INTCONST,
    NODE_CARCONST,
    NODE_NOVALINHA, 
    NODE_TIPO,
    NODE_OP
} NodeType;

/* ============================================================================
 * 2. ESTRUTURA FÍSICA DO NÓ NA MEMÓRIA
 * ============================================================================
 * COMO LER A STRUCT E SEU PAPEL:
 * A `struct AST` é a célula padrão unificada que alicerça todos os nós da árvore.
 * Ela guarda dados a respeito do construtor e armazena os ponteiros para unir os nós filhos.
 * 
 * INFORMAÇÕES INTRA-NÓ:
 * - type:   O tipo semântico (originado do enum acima) indicando qual regra construiu este nó.
 * - lexema: O texto lido do código-fonte atrelado à criação. Pode ser nulo ("NULL") para nós
 *           estruturais puros onde preenchimentos com texto não são aplicáveis (ex: blocos vazios).
 * - linha:  Retém em qual linha exata o erro ou código foi registrado (do 'yylineno' no Lexer). Essencial
 *           para relatar bugs na tela durante a análise semântica.
 * 
 * PONTEIROS DE HIERARQUIA (COMO OS RAMOS SE ALONGAM):
 * - left:   Um ponteiro direto em memória para o 1º filho do nó. Usualmente aponta para o 
 *           termo à esquerda nas equações abstratas ou para a regra de testes condicionais.
 * - right:  Ponteiro em memória para o 2º filho. Guarda blocos de ramificações direitas, 
 *           o resto de uma lista recursiva ou os corpos afirmativos de controles e loops.
 * - extra:  Ponteiro para o 3º filho. A linguagem G-V1 o usa estritamente em nós IF-ELSE
 *           de 3 pontas, permitindo ligar o bloco e o fluxo da contingência alternativa ('senao').
 * ============================================================================
 */
typedef struct AST {
    NodeType type;            
    char* lexema;             
    int linha;                

    struct AST* left;         
    struct AST* right;        
    struct AST* extra;        
} AST;

/* ============================================================================
 * 3. PROTÓTIPO GERENCIADOR DE ALOCAÇÃO DOS NÓS
 * ============================================================================
 * Contém a assinatura base do construtor de células e inicialização dos referenciadores
 * na área do Parser. A implementação reside no `ast.c`.
 */
AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra);

#endif
