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
    NODE_OP,
    /* [GV2] NOVOS TIPOS DE NÓS (NODE TYPES) 
       Representam declarações globais, funções, retornos e vetores na AST. */
    NODE_DECL_GLOBAL,      /* Bloco de variáveis globais */
    NODE_DECL_FUNC,        /* Declaração de função */
    NODE_LISTA_FUNC,       /* Lista encadeada de funções */
    NODE_PARAM,            /* Parâmetro formal */
    NODE_CHAMADA_FUNC,     /* Chamada de função (como expressão) */
    NODE_RETORNE,          /* Comando retorne */
    NODE_VETOR_DECL,       /* Declaração de vetor (IDENT[INTCONST]) */
    NODE_VETOR_ACESSO      /* Acesso a vetor (IDENT[Expr]) */
} NodeType;

typedef struct AST {
    NodeType type;            
    char* lexema;             
    int linha;                

    struct AST* left;         
    struct AST* right;        
    struct AST* extra;
    /* [GV2] QUARTO PONTEIRO: Usado para guardar a lista encadeada de parâmetros formais nas funções (ou lista de argumentos nas chamadas). */
    struct AST* params;
} AST;

AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra);
AST* createFuncNode(NodeType type, char* lexema, int linha, AST* params, AST* tipoRetorno, AST* corpo, AST* extra);

#endif
