#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] IMPLEMENTAÇÃO DA CONSTRUÇÃO DA AST (.C)
 * ============================================================================
 * O QUE É: Este arquivo implementa efetivamente a alocação de memória e a 
 * inicialização dos nós que formam a Árvore Sintática Abstrata (AST).
 * 
 * COMO É USADO NO COMPILADOR:
 * O analisador sintático (Parser/Bison) invoca a função `createNode` cada vez que 
 * reduz (fecha) uma regra gramatical compatível e precisa abstraí-la. 
 * O nó retornado da memória é então passado como filho direto para regras 
 * subsequentes, criando dessa forma toda a hierarquia sintática de baixo para cima. 
 * 
 * Após varrer todo o arquivo, a árvore inteira ficará contida na variável 
 * global 'ast_raiz' definida no Parser, aguardando o Analisador Semântico.
 * ============================================================================
 */
AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra) {

    AST* node = (AST*) malloc(sizeof(AST));
    node->type = type;   
    node->linha = linha; 
    node->left = left;
    node->right = right;
    node->extra = extra;
    node->params = NULL;  /* G-V2: padrão NULL para nós que não são funções */
    
    if (lexema != NULL) {
        node->lexema = strdup(lexema); 
    } else {
        node->lexema = NULL; 
    }
    return node;
}

/* ============================================================================
 * [GV2] CONSTRUTOR ESPECIALIZADO PARA NÓS DE FUNÇÃO (G-V2)
 * ============================================================================
 * Cria um nó de declaração de função com todos os 4 ponteiros preenchidos:
 * - params:  Lista de parâmetros formais (AST encadeada de NODE_PARAM)
 * - left:    Tipo de retorno da função (NODE_TIPO)
 * - right:   Corpo/bloco da função (NODE_BLOCO)
 * - extra:   Uso livre (encadeamento de próxima função, etc.)
 */
AST* createFuncNode(NodeType type, char* lexema, int linha, AST* params, AST* tipoRetorno, AST* corpo, AST* extra) {

    AST* node = (AST*) malloc(sizeof(AST));
    node->type = type;
    node->linha = linha;
    node->left = tipoRetorno;
    node->right = corpo;
    node->extra = extra;
    node->params = params;
    
    if (lexema != NULL) {
        node->lexema = strdup(lexema);
    } else {
        node->lexema = NULL;
    }
    return node;
}
