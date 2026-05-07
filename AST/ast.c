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
    

    if (lexema != NULL) {
        node->lexema = strdup(lexema); 
    } else {
        node->lexema = NULL; 
    }

    return node;
}
