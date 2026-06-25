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
    
    if (expr->type == NODE_VETOR_ACESSO) {
        Symbol* var = lookup(scopes, expr->left->lexema);
        if (var != NULL) {
            int tipoIndice = inferirTipo(expr->right, scopes);
            if (tipoIndice != 0) {
                printf("ERRO SEMANTICO: Indice do vetor '%s' deve ser inteiro! Linha %d\n", expr->left->lexema, expr->linha);
                exit(1);
            }
            return var->tipo;
        } else {
            printf("ERRO SEMANTICO: Vetor '%s' nao declarado! Linha %d\n", expr->left->lexema, expr->linha);
            exit(1);
        }
    }

    if (expr->type == NODE_CHAMADA_FUNC) {
        Symbol* var = lookup(scopes, expr->left->lexema);
        if (var != NULL) {
            if (var->kind != SYM_FUNC) {
                printf("ERRO SEMANTICO: '%s' nao e uma funcao! Linha %d\n", var->nome, expr->linha);
                exit(1);
            }
            
            /* Validação de argumentos */
            AST* argNode = expr->right;
            Param* p = var->params;
            int argCount = 0;
            
            while (argNode != NULL && p != NULL) {
                AST* currArg = argNode;
                if (strcmp(argNode->lexema, "lista_arg") == 0) {
                    currArg = argNode->left;
                }
                
                int tipoArg = inferirTipo(currArg, scopes);
                if (tipoArg != p->tipo && tipoArg != -1) {
                    printf("ERRO SEMANTICO: Tipo incompativel no parametro '%s' da funcao '%s'! Linha %d\n", p->nome, var->nome, expr->linha);
                    exit(1);
                }
                
                argCount++;
                p = p->next;
                
                if (strcmp(argNode->lexema, "lista_arg") == 0) {
                    argNode = argNode->right;
                } else {
                    argNode = NULL;
                }
            }
            
            if (argNode != NULL || p != NULL) {
                printf("ERRO SEMANTICO: Numero incorreto de argumentos para a funcao '%s'! Linha %d\n", var->nome, expr->linha);
                exit(1);
            }
            
            return var->tipo_retorno;
        } else {
            printf("ERRO SEMANTICO: Funcao '%s' nao declarada! Linha %d\n", expr->left->lexema, expr->linha);
            exit(1);
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
    
    if (varNode != NULL) {
        if (varNode->type == NODE_IDENTIFICADOR) {
            Symbol* jaExiste = lookupCurrentScope(scopes, varNode->lexema);
            if (jaExiste != NULL) {
                printf("ERRO SEMANTICO: Variavel '%s' ja declarada neste escopo! Linha %d\n", varNode->lexema, varNode->linha);
                exit(1);
            }
            int pos = scopes->pos_livre++;
            insertSymbol(scopes, varNode->lexema, tipoHerdado, pos);
            printf("[SEMANTICA] Variavel '%s' do tipo %d inserida na pos %d.\n", varNode->lexema, tipoHerdado, pos);
        } 
        else if (varNode->type == NODE_VETOR_DECL) {
            char* nomeVetor = varNode->left->lexema;
            Symbol* jaExiste = lookupCurrentScope(scopes, nomeVetor);
            if (jaExiste != NULL) {
                printf("ERRO SEMANTICO: Vetor '%s' ja declarado neste escopo! Linha %d\n", nomeVetor, varNode->linha);
                exit(1);
            }
            int tamanho = atoi(varNode->right->lexema);
            if (tamanho <= 0) {
                printf("ERRO SEMANTICO: Vetor '%s' com tamanho invalido! Linha %d\n", nomeVetor, varNode->linha);
                exit(1);
            }
            int pos = scopes->pos_livre;
            scopes->pos_livre += tamanho;
            insertVetor(scopes, nomeVetor, tipoHerdado, tamanho, pos);
            printf("[SEMANTICA] Vetor '%s' de tamanho %d inserido na pos %d.\n", nomeVetor, tamanho, pos);
        }
    }

    extrairVariaveis(nodeIds->right, scopes, tipoHerdado);
}

int tipo_retorno_atual = -1; /* [GV2]: Variável global temporária para validar tipos de RETORNE */

void checkSemantics(AST* node, Stack* scopes) {
    if (node == NULL) return;

    switch (node->type) {

        case NODE_PROGRAMA:
            if (strcmp(node->lexema, "programa") == 0) {
                initStack(scopes);
                pushScope(scopes); // Inicializa o escopo global
                
                checkSemantics(node->left, scopes);  /* [GV2]: Globais */
                checkSemantics(node->right, scopes); /* [GV2]: Funções */
                
                tipo_retorno_atual = -1; /* O bloco principal não possui retorno estrito */
                checkSemantics(node->extra, scopes); /* [GV2]: Principal */
            } else if (strcmp(node->lexema, "principal") == 0) {
                /* É o nó gerado por DeclPrograma. O bloco dele está em node->left */
                checkSemantics(node->left, scopes);
            } else {
                /* Compatibilidade G-V1 */
                initStack(scopes);
                pushScope(scopes);
                checkSemantics(node->left, scopes);
            }
            break;

        case NODE_DECL_GLOBAL:
            checkSemantics(node->left, scopes);
            break;

        case NODE_LISTA_FUNC:
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            break;

        case NODE_DECL_FUNC: {
            char* funcName = node->lexema;
            Symbol* jaExiste = lookupCurrentScope(scopes, funcName);
            if (jaExiste != NULL) {
                printf("ERRO SEMANTICO: Funcao '%s' ja declarada! Linha %d\n", funcName, node->linha);
                exit(1);
            }
            
            int tipo_ret = -1;
            if (node->left != NULL) {
                if (strcmp(node->left->lexema, "int") == 0) tipo_ret = 0;
                else if (strcmp(node->left->lexema, "car") == 0) tipo_ret = 1;
            }
            
            /* Processar parâmetros da função */
            Param* paramsHead = NULL;
            Param* paramsTail = NULL;
            int num_params = 0;
            
            AST* pNode = node->params;
            while (pNode != NULL) {
                AST* currParam = pNode;
                if (strcmp(pNode->lexema, "lista_param") == 0) {
                    currParam = pNode->left;
                }
                
                char* pName = currParam->left->lexema;
                int pTipo = -1;
                if (strcmp(currParam->right->lexema, "int") == 0) pTipo = 0;
                else if (strcmp(currParam->right->lexema, "car") == 0) pTipo = 1;
                int pIsVetor = (strcmp(currParam->lexema, "param_vetor") == 0) ? 1 : 0;
                
                Param* novo = (Param*)malloc(sizeof(Param));
                novo->nome = strdup(pName);
                novo->tipo = pTipo;
                novo->is_vetor = pIsVetor;
                novo->next = NULL;
                
                if (paramsTail == NULL) { paramsHead = novo; paramsTail = novo; }
                else { paramsTail->next = novo; paramsTail = novo; }
                
                num_params++;
                if (strcmp(pNode->lexema, "lista_param") == 0) pNode = pNode->right;
                else pNode = NULL;
            }
            
            insertFunction(scopes, funcName, num_params, tipo_ret, paramsHead);
            
            pushScope(scopes);
            
            /* Inserir os parâmetros formais no novo escopo local da função */
            Param* p = paramsHead;
            int offsetParam = 12;
            while (p != NULL) {
                insertParam(scopes, p->nome, p->tipo, offsetParam, p->is_vetor);
                offsetParam += 4;
                p = p->next;
            }
            
            tipo_retorno_atual = tipo_ret;
            checkSemantics(node->right, scopes); /* Analisa o Corpo da Função */
            tipo_retorno_atual = -1;
            
            popScope(scopes);
            break;
        }

        case NODE_BLOCO:
            printf("[SEMANTICA] Entrando em um bloco na linha %d...\n", node->linha);
            
            pushScope(scopes); // Abre um novo escopo local
            
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            
            popScope(scopes); // Encerra o escopo local
            
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

        case NODE_RETORNE: {
            if (tipo_retorno_atual == -1) {
                printf("ERRO SEMANTICO: Comando 'retorne' fora de uma funcao! Linha %d\n", node->linha);
                exit(1);
            }
            int exprTipo = inferirTipo(node->left, scopes);
            if (exprTipo != tipo_retorno_atual && exprTipo != -1) {
                printf("ERRO SEMANTICO: Tipo de retorno incompativel! Linha %d\n", node->linha);
                exit(1);
            }
            break;
        }
        
        case NODE_VETOR_ACESSO:
        case NODE_CHAMADA_FUNC:
            /* Inferência de tipo já cuida das regras e busca das chamadas/acessos */
            inferirTipo(node, scopes);
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
