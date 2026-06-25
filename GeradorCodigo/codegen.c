#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variável global usada para criar IDs únicos nos rótulos (labels) do Assembly MIPS
int label_count = 0;
FILE* data_out = NULL;

int current_func_params = 0;
void cgenEx(AST* expr, FILE* out, Stack* scopes);

void cgenArgs(AST* argNode, FILE* out, Stack* scopes) {
    if (!argNode) return;
    
    if (strcmp(argNode->lexema, "lista_arg") == 0) {
        cgenArgs(argNode->right, out, scopes); 
        cgenEx(argNode->left->left, out, scopes); 
        fprintf(out, "\t# Empilhando argumento\n");
        fprintf(out, "\tsw $s0, 0($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
    } else if (strcmp(argNode->lexema, "arg") == 0) {
        cgenEx(argNode->left, out, scopes);
        fprintf(out, "\t# Empilhando argumento unico/final\n");
        fprintf(out, "\tsw $s0, 0($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
    }
}

void extrairVariaveisMIPS(AST* nodeIds, Stack* scopes) {
    if (nodeIds == NULL) return;
    
    AST* varNode = nodeIds->left;
    if (varNode != NULL) {
        if (varNode->type == NODE_IDENTIFICADOR) {
            int pos = scopes->pos_livre++;
            insertSymbol(scopes, varNode->lexema, 0, pos); 
        } else if (varNode->type == NODE_VETOR_DECL) {
            int tamanho = atoi(varNode->right->lexema);
            int pos = scopes->pos_livre;
            scopes->pos_livre += tamanho;
            insertVetor(scopes, varNode->left->lexema, 0, tamanho, pos);
        }
    }
    
    extrairVariaveisMIPS(nodeIds->right, scopes);
}

void cgenEx(AST* expr, FILE* out, Stack* scopes) {
    if (!expr) return;

    if (expr->type == NODE_INTCONST || expr->type == NODE_CARCONST) {
        fprintf(out, "\t# Lendo constante primitiva: %s\n", expr->lexema);
        fprintf(out, "\tli $s0, %s\n", expr->lexema);
    } 
    else if (expr->type == NODE_IDENTIFICADOR) {
        // CASO 2: Leitura de Variável
        Symbol* var = lookup(scopes, expr->lexema);
        if (var) {
            int offset;
            char reg_base[5];
            
            /* [GV2]: Diferenciar acesso Global ($s1) vs Local ($fp) vs Parametros ($fp positivo) */
            if (var->kind == SYM_PARAM) {
                offset = var->pos;
                strcpy(reg_base, "$fp");
            } else if (var->escopo == 0) {
                offset = -((var->pos - 1) * 4);
                strcpy(reg_base, "$s1");
            } else {
                offset = -(var->pos * 4);
                strcpy(reg_base, "$fp");
            }
            
            fprintf(out, "\t# Lendo variavel %s\n", expr->lexema);
            
            /* [GV2]: Etapa 6C - Vetores passados por parametro. */
            if (var->is_vetor) {
                if (var->kind == SYM_PARAM) {
                    fprintf(out, "\t# Repassando parametro vetor (Endereco)\n");
                    fprintf(out, "\tlw $s0, %d(%s)\n", offset, reg_base);
                } else {
                    fprintf(out, "\t# Passando variavel vetor (Calculando Endereco Base)\n");
                    fprintf(out, "\taddiu $s0, %s, %d\n", reg_base, offset);
                }
            } else {
                fprintf(out, "\tlw $s0, %d(%s)\n", offset, reg_base);
            }
        }
    } 
    else if (expr->type == NODE_VETOR_ACESSO) {
        Symbol* var = lookup(scopes, expr->left->lexema);
        if (var) {
            fprintf(out, "\t# --- Acessando Vetor: %s[indice] ---\n", var->nome);
            
            /* 1. Calcula o índice (resultado no $s0) */
            cgenEx(expr->right, out, scopes);
            
            /* 2. Offset = -(indice * 4) */
            fprintf(out, "\tsll $s0, $s0, 2\n");
            fprintf(out, "\tneg $s0, $s0\n");
            
            /* 3. Base do vetor */
            int offset;
            char reg_base[5];
            if (var->kind == SYM_PARAM) {
                offset = var->pos;
                strcpy(reg_base, "$fp");
            } else if (var->escopo == 0) {
                offset = -((var->pos - 1) * 4);
                strcpy(reg_base, "$s1");
            } else {
                offset = -(var->pos * 4);
                strcpy(reg_base, "$fp");
            }
            
            if (var->kind == SYM_PARAM) {
                fprintf(out, "\tlw $t1, %d(%s)\n", offset, reg_base);
            } else {
                fprintf(out, "\taddiu $t1, %s, %d\n", reg_base, offset);
            }
            
            /* 4. Soma Endereço Base com Offset do Índice */
            fprintf(out, "\tadd $s0, $t1, $s0\n");
            
            /* 5. Lê da Memória o valor e guarda no Acumulador */
            fprintf(out, "\tlw $s0, 0($s0)\n");
            fprintf(out, "\t# --- Fim Acesso Vetor ---\n");
        }
    }
    else if (expr->type == NODE_CHAMADA_FUNC) {
        char* funcName = expr->left->lexema;
        fprintf(out, "\t# --- Iniciando Chamada de Funcao: %s ---\n", funcName);
        fprintf(out, "\t# 1. Empilhando o $fp atual do chamador\n");
        fprintf(out, "\tsw $fp, 0($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
        
        fprintf(out, "\t# 2. Avaliando e Empilhando argumentos\n");
        cgenArgs(expr->right, out, scopes);
        
        fprintf(out, "\t# 3. Jump and Link\n");
        fprintf(out, "\tjal f_entry_%s\n", funcName);
        fprintf(out, "\t# --- Fim Chamada de Funcao ---\n");
    }
    else if (expr->type == NODE_OP) {
        
        // --- CASO 3a: Operador Unário (só existe o lado direito) ---
        if (expr->left == NULL && expr->right != NULL) {
            cgenEx(expr->right, out, scopes); 
            
            if (strcmp(expr->lexema, "-") == 0) {
                fprintf(out, "\tneg $s0, $s0\n"); // Inverte conta (Negativo)
            } else if (strcmp(expr->lexema, "!") == 0) {
                fprintf(out, "\tseq $s0, $s0, 0\n"); // Inversor Lógico (Not)
            }
            return;
        }

        // --- CASO 3b: Operações Binárias Normais (+, -, ==, &&) ---
        fprintf(out, "\t# [Operador %s]: Processando lado Esquerdo\n", expr->lexema);
        cgenEx(expr->left, out, scopes); 
        
        // Passo 1 da Máquina de Pilha: Salva o esquerdo temporariamente na pilha (em $sp).
        fprintf(out, "\t# Empilhando valor esquerdo do operador (%s)\n", expr->lexema);
        fprintf(out, "\tsw $s0, 0($sp)\n");      // Grava $s0 na nova gaveta segura da pilha
        fprintf(out, "\taddiu $sp, $sp, -4\n");  // Move a pilha 4 bytes para trás
        
        fprintf(out, "\t# [Operador %s]: Processando lado Direito\n", expr->lexema);
        cgenEx(expr->right, out, scopes);
        
        // Passo 2 da Máquina de Pilha: Puxa o esquerdo de volta mas joga no $t1.
        fprintf(out, "\t# Desempilhando valor para calculo final\n");
        fprintf(out, "\taddiu $sp, $sp, 4\n");  // Devolve a pilha para a base original
        fprintf(out, "\tlw $t1, 0($sp)\n");     // Lê o valor que estava salvo de volta para o $t1

        // Passo 3 da Máquina de Pilha: Operação Assembly correspondente  (resultado no $s0).
        if (strcmp(expr->lexema, "+") == 0) fprintf(out, "\tadd $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "-") == 0) fprintf(out, "\tsub $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "*") == 0) fprintf(out, "\tmul $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "/") == 0) fprintf(out, "\tdiv $s0, $t1, $s0\n");
        // Lógica Relacional (Menor/Maior/Igual)
        else if (strcmp(expr->lexema, "<") == 0) fprintf(out, "\tslt $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, ">") == 0) fprintf(out, "\tsgt $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "<=") == 0) fprintf(out, "\tsle $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, ">=") == 0) fprintf(out, "\tsge $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "==") == 0) fprintf(out, "\tseq $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "!=") == 0) fprintf(out, "\tsne $s0, $t1, $s0\n");
        // Lógica AND/OR
        else if (strcmp(expr->lexema, "||") == 0) {
            fprintf(out, "\tor $s0, $t1, $s0\n");     
            fprintf(out, "\tsne $s0, $s0, 0\n");  // Força o resultado para booleano puro (1 ou 0)
        }
        else if (strcmp(expr->lexema, "&") == 0) {
            fprintf(out, "\tsne $t1, $t1, 0\n");      
            fprintf(out, "\tsne $s0, $s0, 0\n"); 
            fprintf(out, "\tand $s0, $t1, $s0\n"); 
        }
        
        fprintf(out, "\t# [Fim Operador %s]\n", expr->lexema);
    }
}

void cgenIf(AST* left, AST* right, AST* extra, FILE* out, Stack* scopes) {
    int lbl = label_count++; // Cria um número de label contínuo para evitar duplicatas no arquivo
    
    // Resolve a condição matemática (ex: (a > 5)). O resultado vai ficar no $s0 (0=Falso, 1=Verdadeiro)
    cgenEx(left, out, scopes); 
    
    if (extra == NULL) {
        // [IF SIMPLES] 
        // Se o $s0 for falso (0), ele "salta" ignorando o bloco THEN e vai pro fim.
        fprintf(out, "\tbeq $s0, $zero, fim_if_%d\n", lbl); 
        cgenCmd(right, out, scopes); // Escreve o código interno do bloco THEN
        fprintf(out, "fim_if_%d:\n", lbl);
    } else {
        // [IF-ELSE] 
        // Se $s0 for falso, ele desvia para a label correspondente ao bloco ELSE.
        fprintf(out, "\tbeq $s0, $zero, senao_%d\n", lbl);
        
        cgenCmd(right, out, scopes); // Escreve bloco THEN
        
        // Um IF nunca roda o Else no mesmo fluxo, então forçamos um Jump incondicional pro Fim.
        fprintf(out, "\tj fim_if_%d\n", lbl); 
        
        fprintf(out, "senao_%d:\n", lbl);
        cgenCmd(extra, out, scopes); // Escreve bloco ELSE
        
        fprintf(out, "fim_if_%d:\n", lbl);
    }
}

void cgenWhile(AST* left, AST* right, FILE* out, Stack* scopes) {
    int lbl = label_count++;
    
    // Label fixa onde os elos recursivos sempre vão bater a cabeça
    fprintf(out, "inicio_while_%d:\n", lbl);
    
    // Analisa a condição do bloco loop de novo (O $s0 vira 0 ou 1)
    cgenEx(left, out, scopes);
    
    // Se condição for Falsa, ele vaza pulando a label inteira
    fprintf(out, "\tbeq $s0, $zero, fim_while_%d\n", lbl);
    
    // Processa linha a linha todos os comandos internos do laço
    cgenCmd(right, out, scopes);
    
    // Final do bloco: pula de volta imediatamente pro início (Jump puro).
    fprintf(out, "\tj inicio_while_%d\n", lbl);
    
    fprintf(out, "fim_while_%d:\n", lbl);
}

void cgenCmd(AST* cmd, FILE* out, Stack* scopes) {
    if (!cmd) return;

    switch (cmd->type) {
        case NODE_PROGRAMA:
            if (strcmp(cmd->lexema, "programa") == 0) {
                pushScope(scopes); // Inicializa o escopo global
                
                int posLivreAntiga = scopes->pos_livre;
                
                // 1. Processar Globais
                cgenCmd(cmd->left, out, scopes); 
                
                int varsDeclaradas = scopes->pos_livre - posLivreAntiga;
                if (varsDeclaradas > 0) {
                    fprintf(out, "\t# [GV2]: Alocando espaco para variaveis globais\n");
                    fprintf(out, "\taddiu $sp, $sp, -%d\n", varsDeclaradas * 4);
                }
                
                fprintf(out, "\tj inicio_principal\n\n");
                
                // Rótulo da Main
                fprintf(out, "\ninicio_principal:\n");
                cgenCmd(cmd->extra, out, scopes);
                
                // Sistema Syscall: Exit (encerra o programa antes de cair no código das funções)
                fprintf(out, "\n# Sair do Programa MIPS (Exit)\n");
                fprintf(out, "\tli $v0, 10\n");
                fprintf(out, "\tsyscall\n\n");
                
                // Processar Funções (após o Exit para evitar execução linear indesejada)
                cgenCmd(cmd->right, out, scopes);
            } else {
                // Fallback G-V1
                pushScope(scopes); 
                cgenCmd(cmd->left, out, scopes);
            }
            break;
            
            case NODE_DECL_GLOBAL:
            cgenCmd(cmd->left, out, scopes);
            break;

        case NODE_LISTA_FUNC:
            cgenCmd(cmd->left, out, scopes);
            cgenCmd(cmd->right, out, scopes);
            break;

        case NODE_DECL_FUNC: {
            char* funcName = cmd->lexema;
            Param* paramsHead = NULL;
            Param* paramsTail = NULL;
            int num_params = 0;
            
            AST* pNode = cmd->params;
            while (pNode != NULL) {
                AST* currParam = pNode;
                if (strcmp(pNode->lexema, "lista_param") == 0) currParam = pNode->left;
                
                Param* novo = (Param*)malloc(sizeof(Param));
                novo->nome = strdup(currParam->left->lexema);
                novo->tipo = 0; 
                novo->is_vetor = (strcmp(currParam->lexema, "param_vetor") == 0) ? 1 : 0;
                novo->next = NULL;
                
                if (paramsTail == NULL) { paramsHead = novo; paramsTail = novo; }
                else { paramsTail->next = novo; paramsTail = novo; }
                
                num_params++;
                if (strcmp(pNode->lexema, "lista_param") == 0) pNode = pNode->right;
                else pNode = NULL;
            }
            
            insertFunction(scopes, funcName, num_params, 0, paramsHead);
            
            fprintf(out, "\n# ========================================\n");
            fprintf(out, "# FUNCAO: %s\n", funcName);
            fprintf(out, "# ========================================\n");
            fprintf(out, "f_entry_%s:\n", funcName);
            
            // Prologue do Callee
            fprintf(out, "\tmove $fp, $sp\n");
            fprintf(out, "\tsw $ra, 0($sp)\n");
            fprintf(out, "\taddiu $sp, $sp, -4\n");
            
            pushScope(scopes);
            Param* p = paramsHead;
            int paramOffset = 4;
            while (p != NULL) {
                insertParam(scopes, p->nome, p->tipo, paramOffset, p->is_vetor);
                paramOffset += 4;
                p = p->next;
            }
            
            current_func_params = num_params;
            
            // Salva a contagem global e reseta para alocação local da função
            int posLivreGlobal = scopes->pos_livre;
            scopes->pos_livre = 1;
            
            cgenCmd(cmd->right, out, scopes); // Analisa o Bloco da funcao
            
            // Restaura
            scopes->pos_livre = posLivreGlobal;
            
            // Fallback de epílogo caso o programador não tenha colocado 'retorne'
            fprintf(out, "f_end_%s:\n", funcName);
            fprintf(out, "\tlw $ra, 0($fp)\n");
            fprintf(out, "\tmove $sp, $fp\n");
            fprintf(out, "\taddiu $sp, $sp, %d\n", (num_params + 1) * 4);
            fprintf(out, "\tlw $fp, 0($sp)\n");
            fprintf(out, "\tjr $ra\n\n");
            
            popScope(scopes);
            break;
        }

        case NODE_COMANDO:
            // --- A. ATRIBUIÇÃO ---
            if (strcmp(cmd->lexema, "=") == 0) {
                if (cmd->left->type == NODE_VETOR_ACESSO) {
                    /* Atribuição de Vetor: v[i] = expr */
                    Symbol* var = lookup(scopes, cmd->left->left->lexema);
                    if (var) {
                        fprintf(out, "\t# --- Atualizando Vetor: %s[indice] ---\n", var->nome);
                        /* Resolve lado direito e empilha o valor para protegê-lo */
                        /* durante a avaliação do índice (que pode usar a pilha) */
                        cgenEx(cmd->right, out, scopes);
                        fprintf(out, "\tsw $s0, 0($sp)\n");
                        fprintf(out, "\taddiu $sp, $sp, -4\n");
                        
                        /* Resolve o índice */
                        cgenEx(cmd->left->right, out, scopes);
                        fprintf(out, "\tsll $s0, $s0, 2\n");
                        fprintf(out, "\tneg $s0, $s0\n");
                        
                        /* Obtém a Base */
                        int offset;
                        char reg_base[5];
                        if (var->kind == SYM_PARAM) {
                            offset = var->pos;
                            strcpy(reg_base, "$fp");
                        } else if (var->escopo == 0) {
                            offset = -((var->pos - 1) * 4);
                            strcpy(reg_base, "$s1");
                        } else {
                            offset = -(var->pos * 4);
                            strcpy(reg_base, "$fp");
                        }
                        
                        if (var->kind == SYM_PARAM) {
                            fprintf(out, "\tlw $t1, %d(%s)\n", offset, reg_base);
                        } else {
                            fprintf(out, "\taddiu $t1, %s, %d\n", reg_base, offset);
                        }
                        
                        /* Endereço exato */
                        fprintf(out, "\tadd $s0, $t1, $s0\n");
                        
                        /* Puxa valor direito salvo da pilha pra $t1 */
                        fprintf(out, "\taddiu $sp, $sp, 4\n");
                        fprintf(out, "\tlw $t1, 0($sp)\n");
                        
                        /* Salva valor no vetor */
                        fprintf(out, "\tsw $t1, 0($s0)\n");
                        fprintf(out, "\t# --- Fim Atualizacao Vetor ---\n");
                    }
                } else {
                    // Acha o deslocamento da variável alvo na tabela de Símbolos.
                    Symbol* var = lookup(scopes, cmd->left->lexema);
                    if (var) {
                        // Resolve primeiro a conta inteira para guardar tudo pronto no acumulador ($s0)
                        cgenEx(cmd->right, out, scopes); 
                        
                        int offset;
                        char reg_base[5];
                        if (var->kind == SYM_PARAM) {
                            offset = var->pos;
                            strcpy(reg_base, "$fp");
                        } else if (var->escopo == 0) {
                            offset = -((var->pos - 1) * 4);
                            strcpy(reg_base, "$s1");
                        } else {
                            offset = -(var->pos * 4);
                            strcpy(reg_base, "$fp");
                        }
                        
                        fprintf(out, "\t# Atualizando variavel %s\n", var->nome);
                        fprintf(out, "\tsw $s0, %d(%s)\n", offset, reg_base); // Grava a palavra originada no $s0 para endereço de RAM do offset.
                    }
                }
            } 
            // --- B. CONDICIONAL (SE) ---
            else if (strcmp(cmd->lexema, "se") == 0 || strcmp(cmd->lexema, "se_senao") == 0) {
                cgenIf(cmd->left, cmd->right, cmd->extra, out, scopes);
            }
            // --- C. LAÇO (ENQUANTO) ---
            else if (strcmp(cmd->lexema, "enquanto") == 0) {
                cgenWhile(cmd->left, cmd->right, out, scopes);
            }
            // --- D. FUNÇÃO LEIA ---
            else if (strcmp(cmd->lexema, "leia") == 0) {
                if (cmd->left->type == NODE_VETOR_ACESSO) {
                    Symbol* var = lookup(scopes, cmd->left->left->lexema);
                    if (var) {
                        /* 1. Resolve índice */
                        cgenEx(cmd->left->right, out, scopes);
                        fprintf(out, "\tsll $s0, $s0, 2\n");
                        fprintf(out, "\tneg $s0, $s0\n");
                        
                        /* 2. Endereço base */
                        int offset;
                        char reg_base[5];
                        if (var->kind == SYM_PARAM) {
                            offset = var->pos;
                            strcpy(reg_base, "$fp");
                        } else if (var->escopo == 0) {
                            offset = -((var->pos - 1) * 4);
                            strcpy(reg_base, "$s1");
                        } else {
                            offset = -(var->pos * 4);
                            strcpy(reg_base, "$fp");
                        }
                        
                        if (var->kind == SYM_PARAM) {
                            fprintf(out, "\tlw $t1, %d(%s)\n", offset, reg_base);
                        } else {
                            fprintf(out, "\taddiu $t1, %s, %d\n", reg_base, offset);
                        }
                        
                        /* 3. Soma Endereço */
                        fprintf(out, "\tadd $t1, $t1, $s0\n");
                        
                        /* 4. Realiza Leitura Teclado */
                        fprintf(out, "\t# Chamada syscall 5 - Ler Inteiro\n");
                        fprintf(out, "\tli $v0, 5\n");
                        fprintf(out, "\tsyscall\n");
                        fprintf(out, "\tsw $v0, 0($t1)\n");
                    }
                } else {
                    Symbol* var = lookup(scopes, cmd->left->lexema);
                    if (var) {
                        int offset;
                        char reg_base[5];
                        if (var->kind == SYM_PARAM) {
                            offset = var->pos;
                            strcpy(reg_base, "$fp");
                        } else if (var->escopo == 0) {
                            offset = -((var->pos - 1) * 4);
                            strcpy(reg_base, "$s1");
                        } else {
                            offset = -(var->pos * 4);
                            strcpy(reg_base, "$fp");
                        }
                        
                        // Syscall código 5: Lê inteiro digitado do teclado gravando no $v0
                        fprintf(out, "\t# Chamada syscall 5 - Ler Inteiro\n");
                        fprintf(out, "\tli $v0, 5\n");
                        fprintf(out, "\tsyscall\n");
                        fprintf(out, "\tsw $v0, %d(%s)\n", offset, reg_base); // Transfere a resposta para a gaveta local do MIPS
                    }
                }
            }
            // --- E. FUNÇÃO ESCREVA ---
            else if (strcmp(cmd->lexema, "escreva") == 0) {
                // Checa se o usuário mandou imprimir uma palavra literal aspas (Ex: escreva "Hello")
                if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\"') {
                    int str_lbl = label_count++;
                    
                    // Coloca de propósito na seção .data para criar a String estática (Asciiz)
                    fprintf(data_out, "str_lit_%d:\n", str_lbl);
                    fprintf(data_out, "\t.asciiz %s\n", cmd->left->lexema);
                    
                    // Syscall código 4: Imprime textos alocados string base apontados pelo $a0
                    fprintf(out, "\t# Chamada syscall 4 - Escrever Texto Literal\n");
                    fprintf(out, "\tli $v0, 4\n");
                    fprintf(out, "\tla $a0, str_lit_%d\n", str_lbl);
                    fprintf(out, "\tsyscall\n");
                }  
                else {
                    // Impressões Numéricas ou de Variáveis 
                    cgenEx(cmd->left, out, scopes); 
                    
                    // Se for caractere forte (Ex: 'A' em pascal), usa syscall 11. 
                    // Do contrário, todas as contas matemáticas caem na syscall 1 (Inteiro)
                    if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\'') {
                        fprintf(out, "\t# Chamada syscall 11 - Caractere Unico\n");
                        fprintf(out, "\tli $v0, 11\n");
                        fprintf(out, "\tmove $a0, $s0\n");
                        fprintf(out, "\tsyscall\n");
                    } else {
                        fprintf(out, "\t# Chamada syscall 1 - Imprimir Inteiro\n");
                        fprintf(out, "\tli $v0, 1\n");
                        fprintf(out, "\tmove $a0, $s0\n");
                        fprintf(out, "\tsyscall\n");
                    }
                }
            }
            break;
        
        case NODE_NOVALINHA: {
            int nl_lbl = label_count++;
            fprintf(data_out, "nl_lit_%d:\n", nl_lbl);
            fprintf(data_out, "\t.asciiz \"\\n\"\n");
            fprintf(out, "\tli $v0, 4\n");
            fprintf(out, "\tla $a0, nl_lit_%d\n", nl_lbl);
            fprintf(out, "\tsyscall\n");
            break;
        }

        case NODE_CHAMADA_FUNC:
            // Reaproveita a lógia de cgenEx que já emite os parâmetros, salva na pilha, e dá jump and link
            cgenEx(cmd, out, scopes);
            break;

        case NODE_BLOCO:
            pushScope(scopes); // Abre um novo escopo local
            
            // Guarda referencial do peso da pilha antido para poder abater o tamanho final perfeitamente MIPS
            int posLivreAntiga = scopes->pos_livre;
            cgenCmd(cmd->left, out, scopes); 
            
            // Computa quantos itens novos foram declarados neste novo escopo ({int a; int b;) e desce a Pilha $SP para criar memória vazia (-4 bytes p/ cada int)
            int varsDeclaradas = scopes->pos_livre - posLivreAntiga;
            if (varsDeclaradas > 0) {
                fprintf(out, "\t# Reservando %d variaveis locais na Pilha MIPS\n", varsDeclaradas);
                fprintf(out, "\taddiu $sp, $sp, -%d\n", varsDeclaradas * 4); 
            }

            // Descida contínua processando os comandos (Ifs, Maths, etc)
            cgenCmd(cmd->right, out, scopes); 
            
            popScope(scopes); // Reverte bloco, matando o escopo
            break;

        case NODE_DECL_VAR:
            // Faz um rastreio MIPS pra setar Offsets negativos na listagem das variáveis achadas lá em cima.
            if (strcmp(cmd->lexema, "lista_var") == 0) {
                extrairVariaveisMIPS(cmd->left, scopes);
                cgenCmd(cmd->right, out, scopes);
            } else if (strcmp(cmd->lexema, "ids") == 0) {
                extrairVariaveisMIPS(cmd, scopes);
            }
            break;

        case NODE_RETORNE:
            fprintf(out, "\t# Comando RETORNE\n");
            cgenEx(cmd->left, out, scopes);
            
            fprintf(out, "\t# Restaurando pilha e retornando para Chamador\n");
            fprintf(out, "\tlw $ra, 0($fp)\n");
            fprintf(out, "\tmove $sp, $fp\n");
            fprintf(out, "\taddiu $sp, $sp, %d\n", (current_func_params + 1) * 4);
            fprintf(out, "\tlw $fp, 0($sp)\n");
            fprintf(out, "\tjr $ra\n");
            break;

        default:
            cgenCmd(cmd->left, out, scopes);
            cgenCmd(cmd->right, out, scopes);
            break;
    }
}

void generateCode(AST* root, Stack* scopes, const char* out_filename) {
    FILE* out = fopen(out_filename, "w");
    if (!out) {
        printf("ERRO FATAL: Nao foi possivel criar o arquivo %s!\n", out_filename);
        exit(1);
    }
    
    data_out = fopen("data_section.tmp", "w+");
    if (!data_out) {
        printf("ERRO FATAL: Nao foi possivel criar arquivo temporario!\n");
        exit(1);
    }

    // Marcações MIPS (Data Segment e Main Entrypoint Globais)
    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "main:\n");

    // Equalização Limítrofe: Faz com que o MIPS copie o endereço base que estava na Pilha $SP para ser nosso topo oficial imutável de Offset $FP.
    fprintf(out, "\t# [GV2]: $s1 aponta para a base das variaveis globais\n");
    fprintf(out, "\tmove $s1, $sp\n");
    fprintf(out, "\tmove $fp, $sp\n\n");

    // Reinicia o offset de memória que foi usado pelo passo semântico, restaurando-o para uso físico no Code Gen.
    initStack(scopes);

    cgenCmd(root, out, scopes);
    
    fprintf(out, "\n.data\n");
    rewind(data_out);
    char buf[1024];
    while(fgets(buf, sizeof(buf), data_out)) {
        fputs(buf, out);
    }
    fclose(data_out);
    remove("data_section.tmp");
    
    fclose(out);
}
