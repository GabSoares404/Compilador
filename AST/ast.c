#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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
