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
    
    /* 
     * ----------------------------------------------------------------------------
     * 1. ALOCAÇÃO DE MEMÓRIA DINÂMICA
     * ----------------------------------------------------------------------------
     * A função `malloc` é usada em nível de C para reservar um bloco de espaço 
     * contíguo no 'heap', com tamanho estrito correspondente à estrutura do nó. 
     * Ao fazermos isso durante o processamento do Parser, garantimos que os dados
     * da AST fiquem seguros e duradouros na memória ao longo de todo o ciclo de 
     * vida da compilação.
     */
    AST* node = (AST*) malloc(sizeof(AST));
    
    /* 
     * ----------------------------------------------------------------------------
     * 2. INICIALIZAÇÃO DOS DADOS BÁSICOS (TIPAGEM E LINHA)
     * ----------------------------------------------------------------------------
     * Preenchemos os parâmetros essenciais diretamente nas propriedades do nó recém 
     * construído. Isso inclui sua classificação semântica (`type`) e de qual linha 
     * do código original a expressão se originou (`linha`).
     */
    node->type = type;   
    node->linha = linha; 
    
    /* 
     * ----------------------------------------------------------------------------
     * 3. CONEXÃO HIERÁRQUICA E ESTRUTURAL
     * ----------------------------------------------------------------------------
     * Os ponteiros `left`, `right` e `extra` (passados pelo Bison nos parâmetros) 
     * referenciam fragmentos de nós processados no passo anterior da análise.
     * Ao atrelar essas referências de memória no nó atual, ele imediatamente se torna 
     * o "Pai" desses nós, cimentando a montagem conectada da árvore sintática.
     */
    node->left = left;
    node->right = right;
    node->extra = extra;
    
    /* 
     * ----------------------------------------------------------------------------
     * 4. GESTÃO SEGURA DE MEMÓRIA DE STRINGS (LEXEMA)
     * ----------------------------------------------------------------------------
     * O Lexer atualiza continuamente os textos extraídos de forma dinâmica em 
     * um loop contínuo sob uma mesma área de buffer nativa (`yytext` ou ponteiros locais).
     * Se associássemos as variáveis através de uma simples igualdade (node->lexema = lexema),
     * a estrutura perderia os dados originais no minuto em que o próximo token fosse escaneado.
     * 
     * SOLUÇÃO TÉCNICA:
     * Utilizamos explicitamente `strdup()` caso haja texto acompanhando o nó.
     * Essa função aloca mais uma área livre silenciosamente e realiza a cópia idêntica  
     * e exclusiva da string repassada, garantindo a sua imutabilidade durante 
     * toda a execução do programa principal. Se não houver string alocável no contexto, 
     * registramos `NULL` para economizar memória e indicarmos um nó de comando fechado.
     */
    if (lexema != NULL) {
        node->lexema = strdup(lexema); 
    } else {
        node->lexema = NULL; 
    }

    /* O ponteiro referencial do nó recém-concluído é devolvido ao Parser C */
    return node;
}
