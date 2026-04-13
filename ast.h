#ifndef AST_H
#define AST_H

// 1. Nossa cartela de categorias de etiqueta da Árvore
typedef enum {
    NODE_PROGRAMA,
    NODE_BLOCO,
    NODE_DECL_VAR,
    NODE_COMANDO,
    NODE_EXPR,
    NODE_IDENTIFICADOR,
    NODE_NOVALINHA, // Adicionei esse para o break line
    NODE_OP
} NodeType;

// 2. O design da caixinha (nó)
typedef struct AST {
    NodeType type;            // Salva qual etiqueta ele é (ex: NODE_OP)
    char* lexema;             // Salva a palavra real (ex: "+" ou "varX")
    int linha;                // Salva a linha de yylineno

    struct AST* left;         // Endereço para o filho 1 (Esquerdo)
    struct AST* right;        // Endereço para o filho 2 (Direito)
    struct AST* extra;        // Endereço para o filho 3 (Para opções do If/Senão)
} AST;

// 3. A assinatura da função responsável por preencher essa caixinha de cima
AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra);

#endif
