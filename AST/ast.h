#ifndef AST_H
#define AST_H

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

typedef struct AST {
    NodeType type;            
    char* lexema;             
    int linha;                

    struct AST* left;         
    struct AST* right;        
    struct AST* extra;        
} AST;

AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra);

#endif
