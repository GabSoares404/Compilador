#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

// O Revisor Matemático (O Inferidor de Tipos)
// Ele vasculhará uma sub-árvore inteira de uma Matemática (x = a + 5) e descobrirá se é inteira ou caractere 
int inferirTipo(AST* expr, Stack* scopes) {
    if (expr == NULL) return -1;

    // Se batermos num Fim da linha, puxamos o seu Tipo nativo
    if (expr->type == NODE_INTCONST) return 0;       // INT é sempre Tipo 0
    if (expr->type == NODE_CARCONST) return 1;       // CAR é sempre Tipo 1
    
    if (expr->type == NODE_IDENTIFICADOR) {
        Symbol* var = lookup(scopes, expr->lexema);
        if (var != NULL) {
            return var->tipo; // Desconectamos o nome da variável e retornamos apenas o sangue dela (0 ou 1)
        }
    }
    
        if (expr->type == NODE_OP) {
        // Usa a "Inception": a função chama a si mesma até escavar o coração Matemático!
        int ladoE = inferirTipo(expr->left, scopes);
        int ladoD = inferirTipo(expr->right, scopes);
        
        // Checagem Regra 3 (Relacionais exigem pares iguais, mas geram booleano INT 0)
        if (strcmp(expr->lexema, "==") == 0 || strcmp(expr->lexema, "!=") == 0 ||
            strcmp(expr->lexema, "<") == 0 || strcmp(expr->lexema, "<=") == 0 ||
            strcmp(expr->lexema, ">") == 0 || strcmp(expr->lexema, ">=") == 0) {
            
            if (ladoE != ladoD && ladoE != -1 && ladoD != -1) {
                printf("ERRO SEMANTICO: Comapracao Incompativel entre Inteiro e Caractere! Linha %d\n", expr->linha);
                exit(1);
            }
            return 0; // A resposta de um "5 > 4" na G-V1 é Int (0 == False, 1 == True)
        }
        
        // Checagem Regras 1 e 2 (Matemática e AND/OR proíbem Letras brutalmente)
        if (ladoE == 1 || ladoD == 1) {
            printf("ERRO SEMANTICO: Variavel Caractere enviada a um calculo matematico! Linha %d\n", expr->linha);
            exit(1);
        }
        return 0; // Uma operação entre 0 e 0 (Int e Int)... sempre resultará 0 (Int)!
    }

    
    return -1; // Desconhecido ou Inválido
}

// Função Ajudante que passará como um zíper pela corrente "ids", recebendo o Tipo herdado
void extrairVariaveis(AST* nodeIds, Stack* scopes, int tipoHerdado) {
    if (nodeIds == NULL) return;
    
        // 1. Acessamos a gaveta Esquerda, que é onde a Gramática guardou o nome!
    AST* varNode = nodeIds->left;
    
    if (varNode != NULL && varNode->type == NODE_IDENTIFICADOR) {
        
        // 2. DEFESA SEMÂNTICA: O juiz entra em ação! 
        // Verificamos com o lookup se já existe alguém com esse nome no Escopo!
        Symbol* jaExiste = lookupCurrentScope(scopes, varNode->lexema);
        if (jaExiste != NULL) {
            printf("ERRO SEMANTICO: Variavel '%s' ja declarada neste escopo! Linha %d\n", varNode->lexema, varNode->linha);
            exit(1);
        }

        // 3. Tudo seguro! Enviamos oficialmente a variável para dentro do seu `.c` da Tabela de Símbolos.
        insertSymbol(scopes, varNode->lexema, tipoHerdado);
        
        // Um Log inofensivo para nossa depuração no WSL
        printf("[SEMANTICA] Variavel '%s' do tipo %d inserida na memoria global com sucesso.\n", varNode->lexema, tipoHerdado);
    }


    // Continua descendo recursivamente e transportando o tipo original lido lá de fora!
    extrairVariaveis(nodeIds->right, scopes, tipoHerdado);
}


// Essa função será chamada recursivamente e "descerá" na árvore (Left / Right)
void checkSemantics(AST* node, Stack* scopes) {
    if (node == NULL) return;

    // O Caminhante reage diferente dependendo da CAIXA onde ele pousou!
    switch (node->type) {

        case NODE_PROGRAMA:
            // "Pousei na raiz (Principal). Vou inicializar a Pilha Central e continuar descendo!"
            initStack(scopes);
            checkSemantics(node->left, scopes);
            break;

        case NODE_BLOCO:
            // Desafio: Aqui você deverá abrir os escopos (pushScope), percorrer os filhos (Left e Right)
            // e depois fechar e destruir o escopo (popScope). 
            printf("[SEMANTICA] Entrando em um bloco na linha %d...\n", node->linha);
            
            // ---------------- [ SEU CÓDIGO AQUI ] ----------------
            // 1. Chame a abertura de Escopo
            pushScope(scopes);
            
            // 2. Caminhe à esquerda (node->left) para ler as Variáveis se existirem
            checkSemantics(node->left, scopes);
            
            // 3. Caminhe à direita (node->right) para ler os Comandos se existirem
            checkSemantics(node->right, scopes);
            
            // 4. Chame o fechamento do Escopo
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
            // Uma lista solta de comandos, caminhe nela inteira!
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            break;

        case NODE_COMANDO:
            // Identificador genérico para Comandos matemáticos ou Desvios de código
            // O lado Esquerdo (=) ou a Expressão Matemática Central (Se, Enquanto) sempre devem ser calculados primeiro.
            if (strcmp(node->lexema, "=") == 0) {
                // EX: a = 5;
                // Descobrimos o tipo de quem vai receber (a), e descobrimos a matemática toda de quem vai tentar ser gravado (5)
                int tipoRecebedor = inferirTipo(node->left, scopes);
                int tipoExpressao = inferirTipo(node->right, scopes);
                
                // CUIDADO: O PDF do G-V1 exíge que Int não se misture com Char!
                if(tipoRecebedor != -1 && tipoExpressao != -1 && tipoRecebedor != tipoExpressao) {
                    printf("ERRO SEMANTICO: Tentativa de Atribuicao com Tipos Incompativeis (Int <> Car)! Linha %d\n", node->linha);
                    exit(1);
                }
            } 
            else if (strcmp(node->lexema, "se") == 0 || strcmp(node->lexema, "se_senao") == 0 || strcmp(node->lexema, "enquanto") == 0) {
                // Nesses comandos o Right e o Extra são só listas de código. O grande teste matemático reside na ESQUERDA!
                int tipoCondicao = inferirTipo(node->left, scopes);
                if (tipoCondicao == 1) { // 1 == Car
                    printf("ERRO SEMANTICO: Expressao Logica dentro do laco nao e resolvivel como Inteiro! Linha %d\n", node->linha);
                    exit(1);
                }
            }

            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            checkSemantics(node->extra, scopes);
            break;

        case NODE_OP:
            // "Bati num Sinal de Operador + - * / ou Lógicos!"
            checkSemantics(node->left, scopes);
            checkSemantics(node->right, scopes);
            break;

        case NODE_IDENTIFICADOR: 
            if(lookup(scopes, node->lexema) == NULL){
                printf("ERRO SEMANTICO: Variavel '%s' nao declarada! Linha %d\n", node->lexema, node->linha);
                exit(1);
            }
            break;
        default:
            // Quaisquer outras caixas folha (Literais, Números, Varivaceis Genéricas) a gente encerra a busca nas "pontas"
            // e retorna pra cima. Elas não têm filhos!
            break;
    }
}
