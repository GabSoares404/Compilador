#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

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

void checkSemantics(AST* node, Stack* scopes) {
    if (node == NULL) return;

    switch (node->type) {

        case NODE_PROGRAMA:
            initStack(scopes);
            pushScope(scopes);
            checkSemantics(node->left, scopes);
            popScope(scopes);
            break;

        case NODE_BLOCO:
            printf("[SEMANTICA] Entrando em um bloco na linha %d...\n", node->linha);
            
            pushScope(scopes);
            
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            
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
