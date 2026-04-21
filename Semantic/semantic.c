#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

/* ============================================================================
 * IMPLEMENTAÇÃO DO ANALISADOR SEMÂNTICO (.C)
 * ============================================================================
 * PAPEL DO ARQUIVO:
 * Realiza as validações dependentes de contexto (semântica).
 * Percorre a AST para verificar declarações, escopos e verificar a compatibilidade 
 * de tipos do compilador G-V1.
 * ============================================================================
 */


/* 
 * ----------------------------------------------------------------------------
 * 1. INFERÊNCIA DE TIPOS EM EXPRESSÕES (inferirTipo)
 * ----------------------------------------------------------------------------
 * Determina recursivamente o tipo final (0 = inteiro, 1 = caractere) gerado
 * pela avaliação de um nó e seus respectivos filhos.
 * 
 * Lógica base de validação:
 * - Constantes: São imediatamente validadas (inteiro ou caractere).
 * - Variáveis (Identificadores): Pesquisam na Tabela de Símbolos ativa
 *   para obter o tipo original de sua declaração.
 * - Nós Operadores (Lógicos e Aritméticos): Checam os operandos esquerdo e direito.
 *   Se houver operação aritmética envolvendo tipos incompatíveis (ex: caractere),
 *   um erro semântico é disparado. Todas as operações relacionais resultam 
 *   em inteiro representando o booleano de retorno (0 ou 1).
 */
int inferirTipo(AST* expr, Stack* scopes) {
    if (expr == NULL) return -1;

    if (expr->type == NODE_INTCONST) return 0;       
    if (expr->type == NODE_CARCONST) return 1;       
    
    if (expr->type == NODE_IDENTIFICADOR) {
        Symbol* var = lookup(scopes, expr->lexema);
        if (var != NULL) {
            return var->tipo; 
        }
    }
    
    if (expr->type == NODE_OP) {
        int ladoE = inferirTipo(expr->left, scopes);
        int ladoD = inferirTipo(expr->right, scopes);
        
        // Verifica validações em operandos relacionais ('<', '>', '==')
        if (strcmp(expr->lexema, "==") == 0 || strcmp(expr->lexema, "!=") == 0 ||
            strcmp(expr->lexema, "<") == 0 || strcmp(expr->lexema, "<=") == 0 ||
            strcmp(expr->lexema, ">") == 0 || strcmp(expr->lexema, ">=") == 0) {
            
            if (ladoE != ladoD && ladoE != -1 && ladoD != -1) {
                printf("ERRO SEMANTICO: Comparacao de tipos incompativeis! Linha %d\n", expr->linha);
                exit(1);
            }
            return 0; // Operadores relacionais sempre retornam inteiro
        }
        
        // Impede uso de caracteres em outros fluxos operacionais matemáticos
        if (ladoE == 1 || ladoD == 1) {
            printf("ERRO SEMANTICO: Caractere operado em expressao matematica! Linha %d\n", expr->linha);
            exit(1);
        }
        return 0; 
    }

    return -1; 
}


/* 
 * ----------------------------------------------------------------------------
 * 2. VERIFICAÇÃO DE REDECLARAÇÃO NO ESCOPO ATUAL (extrairVariaveis)
 * ----------------------------------------------------------------------------
 * Coleta recursivamente subárvores de variáveis de mesmo tipo (ex: int a, b;) e
 * insere seus nós na Tabela de Símbolos, garantindo unicidade no escopo atual.
 * 
 * Verificação de redeclaração:
 * Chama a busca estrita 'lookupCurrentScope' priorizando checar apenas no 
 * nível de escopo vigente. Se a variável já existir ali, a compilação é abortada.
 */
void extrairVariaveis(AST* nodeIds, Stack* scopes, int tipoHerdado) {
    if (nodeIds == NULL) return;
    
    AST* varNode = nodeIds->left;
    
    if (varNode != NULL && varNode->type == NODE_IDENTIFICADOR) {
        
        Symbol* jaExiste = lookupCurrentScope(scopes, varNode->lexema);
        if (jaExiste != NULL) {
            printf("ERRO SEMANTICO: Variavel '%s' ja declarada neste escopo! Linha %d\n", varNode->lexema, varNode->linha);
            exit(1);
        }

        insertSymbol(scopes, varNode->lexema, tipoHerdado);
        
        printf("[SEMANTICA] Variavel '%s' do tipo %d inserida na Tabela de Simbolos.\n", varNode->lexema, tipoHerdado);
    }

    extrairVariaveis(nodeIds->right, scopes, tipoHerdado);
}


/* 
 * ----------------------------------------------------------------------------
 * 3. PERCURSO RECURSIVO NA AST (checkSemantics)
 * ----------------------------------------------------------------------------
 * Executa uma travessia baseada em método de busca em profundidade (AST Traversal - DFS)
 * em todos os nós da Árvore Sintática e coordena as checagens dependendo do tipo do nó.
 * 
 * Etapas no fluxo recursivo:
 * - NODE_BLOCO: Gerencia escopos. Executa pushScope() empilhando um novo escopo
 *   antes de analisar os nós filhos e encerra removendo o escopo (popScope()).
 * - NODE_DECL_VAR: Associa tipos primitivos ('int' = 0, 'car' = 1) extraídos  
 *   da AST e repassa para a função responsavel por inserir as variaveis.
 * - NODE_COMANDO (Atribuições =): Invoca a inferência de tipo simultânea nos 
 *   nós esquerdos e direitos para checar compatibilidade cruzada (não permite misturar tipo).
 * - NODE_COMANDO (se/enquanto): Audita os condicionais requerendo que a condição
 *   teste seja unicamente derivada como resultado inteiro (como booleano).
 * - NODE_IDENTIFICADOR: Valida que a leitura ou edição está em uma variável real. 
 *   Verifica sob a busca do método `lookup` listre todas as tabelas pra testar se existe.
 */
void checkSemantics(AST* node, Stack* scopes) {
    if (node == NULL) return;

    switch (node->type) {

        case NODE_PROGRAMA:
            initStack(scopes);
            checkSemantics(node->left, scopes);
            break;

        case NODE_BLOCO:
            printf("[SEMANTICA] Entrando em um bloco na linha %d...\n", node->linha);
            
            // Criação de novo escopo
            pushScope(scopes);
            
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            
            // Remoção de escopo concluído
            popScope(scopes);
            
            printf("[SEMANTICA] Saindo do bloco da linha %d.\n", node->linha);
            break;

        case NODE_DECL_VAR:
             if(strcmp(node->lexema, "lista_var") == 0) {
                int meuTipo = -1;
                if (node->left != NULL && node->left->extra != NULL) {
                    if (strcmp(node->left->extra->lexema, "int") == 0) {
                        meuTipo = 0;
                    } else if (strcmp(node->left->extra->lexema, "car") == 0) {
                        meuTipo = 1; 
                    }
                }
                extrairVariaveis(node->left, scopes, meuTipo);
                checkSemantics(node->right, scopes);
            }
            else if (strcmp(node->lexema, "ids") == 0) {
                int meuTipo = -1;
                if (node->extra != NULL) {
                    if (strcmp(node->extra->lexema, "int") == 0) {
                        meuTipo = 0;
                    } else if (strcmp(node->extra->lexema, "car") == 0) {
                        meuTipo = 1; 
                    }
                }
                extrairVariaveis(node, scopes, meuTipo);
            }
            break;

        case NODE_LISTA_COMANDO:
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            break;

        case NODE_COMANDO:
            if (strcmp(node->lexema, "=") == 0) {
                int tipoRecebedor = inferirTipo(node->left, scopes);
                int tipoExpressao = inferirTipo(node->right, scopes);
                
                // Validação de compatibilidade de tipos em Atribuições 
                if(tipoRecebedor != -1 && tipoExpressao != -1 && tipoRecebedor != tipoExpressao) {
                    printf("ERRO SEMANTICO: Valores incompativeis na atribuicao! Linha %d\n", node->linha);
                    exit(1);
                }
            } 
            else if (strcmp(node->lexema, "se") == 0 || strcmp(node->lexema, "se_senao") == 0 || strcmp(node->lexema, "enquanto") == 0) {
                int tipoCondicao = inferirTipo(node->left, scopes);
                
                // Validação de condição do SE e do ENQUANTO
                if (tipoCondicao == 1) { 
                    printf("ERRO SEMANTICO: Expressao de condicao logica invalida (espera-se tipo Int)! Linha %d\n", node->linha);
                    exit(1);
                }
            }

            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            checkSemantics(node->extra, scopes);
            break;

        case NODE_OP:
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            break;

        case NODE_IDENTIFICADOR: 
            // Busca de identificador na tabela de símbolos para prever erro lógico
            if(lookup(scopes, node->lexema) == NULL){
                printf("ERRO SEMANTICO: Variavel '%s' nao declarada! Linha %d\n", node->lexema, node->linha);
                exit(1);
            }
            break;
            
        default:
            break;
    }
}
