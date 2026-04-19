#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Essa é a Função Fábrica de caixas
AST* createNode(NodeType type, char* lexema, int linha, AST* left, AST* right, AST* extra) {
    
    // O comando sagrado do 'malloc'. Ele pede um terreno vazio na Memória RAM
    // equivalente ao tamanho exato do chassi da struct AST.
    AST* node = (AST*) malloc(sizeof(AST));
    
    // Agora preenchemos as gavetas com o operador `->`
    node->type = type;
    node->linha = linha;
    
    // AS CORDAS: Conectamos os endereços de memória dos nossos eventuais Nétos à nós mesmos!
    node->left = left;
    node->right = right;
    node->extra = extra;
    
    // A GRANDE PEGADINHA: Diferente de int, não podemos simplesmente igualar Strings em C.
    // O texto lido foi capturado pelo yylex, mas se copiarmos ele bruto, ele some no 
    // lixo da memória rápida. O 'strdup' clona fielmente a palavra.
    if (lexema != NULL) {
        node->lexema = strdup(lexema); 
    } else {
        node->lexema = NULL;
    }

    return node;
}
