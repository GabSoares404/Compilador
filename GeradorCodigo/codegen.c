#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int label_count = 0;

void extrairVariaveisMIPS(AST* nodeIds, Stack* scopes) {
    if (nodeIds == NULL) return;
    
    AST* varNode = nodeIds->left;
    if (varNode != NULL && varNode->type == NODE_IDENTIFICADOR) {
        insertSymbol(scopes, varNode->lexema, 0); 
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
        Symbol* var = lookup(scopes, expr->lexema);
        if (var) {
            int offset = -(var->pos * 4);
            fprintf(out, "\t# Lendo variavel %s do endereco %d($fp)\n", expr->lexema, offset);
            fprintf(out, "\tlw $s0, %d($fp)\n", offset);
        }
    } 
    else if (expr->type == NODE_OP) {
        if (expr->left == NULL && expr->right != NULL) {
            cgenEx(expr->right, out, scopes); 
            
            if (strcmp(expr->lexema, "-") == 0) {
                fprintf(out, "\tneg $s0, $s0\n");
            } else if (strcmp(expr->lexema, "!") == 0) {
                fprintf(out, "\tseq $s0, $s0, 0\n");
            }
            return;
        }
        fprintf(out, "\t# [Operador %s]: Processando lado Esquerdo\n", expr->lexema);
        cgenEx(expr->left, out, scopes); 
        
        fprintf(out, "\t# Empilhando valor esquerdo do operador (%s)\n", expr->lexema);
        fprintf(out, "\taddiu $sp, $sp, -4\n");
        fprintf(out, "\tsw $s0, 0($sp)\n");
        
        fprintf(out, "\t# [Operador %s]: Processando lado Direito\n", expr->lexema);
        cgenEx(expr->right, out, scopes);
        
        fprintf(out, "\t# Desempilhando valor para calculo final\n");
        fprintf(out, "\tlw $t1, 0($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, 4\n");

        if (strcmp(expr->lexema, "+") == 0) fprintf(out, "\tadd $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "-") == 0) fprintf(out, "\tsub $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "*") == 0) fprintf(out, "\tmul $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "/") == 0) fprintf(out, "\tdiv $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "<") == 0) fprintf(out, "\tslt $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, ">") == 0) fprintf(out, "\tsgt $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "<=") == 0) fprintf(out, "\tsle $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, ">=") == 0) fprintf(out, "\tsge $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "==") == 0) fprintf(out, "\tseq $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "!=") == 0) fprintf(out, "\tsne $s0, $t1, $s0\n");
        else if (strcmp(expr->lexema, "||") == 0) {
            fprintf(out, "\tor $s0, $t1, $s0\n");     
            fprintf(out, "\tsne $s0, $s0, 0\n"); 
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
    int lbl = label_count++; 
    
    cgenEx(left, out, scopes); 
    
    if (extra == NULL) {
        fprintf(out, "\tbeq $s0, $zero, fim_if_%d\n", lbl); 
        cgenCmd(right, out, scopes); 
        fprintf(out, "fim_if_%d:\n", lbl);
    } else {
        fprintf(out, "\tbeq $s0, $zero, senao_%d\n", lbl);
        
        cgenCmd(right, out, scopes); 
        
        fprintf(out, "\tj fim_if_%d\n", lbl); 
        
        fprintf(out, "senao_%d:\n", lbl);
        cgenCmd(extra, out, scopes); 
        
        fprintf(out, "fim_if_%d:\n", lbl);
    }
}

void cgenWhile(AST* left, AST* right, FILE* out, Stack* scopes) {
    int lbl = label_count++;
    
    fprintf(out, "inicio_while_%d:\n", lbl);
    
    cgenEx(left, out, scopes);
    
    fprintf(out, "\tbeq $s0, $zero, fim_while_%d\n", lbl);
    
    cgenCmd(right, out, scopes);
    
    fprintf(out, "\tj inicio_while_%d\n", lbl);
    
    fprintf(out, "fim_while_%d:\n", lbl);
}

void cgenCmd(AST* cmd, FILE* out, Stack* scopes) {
    if (!cmd) return;

    switch (cmd->type) {
        case NODE_PROGRAMA:
            pushScope(scopes);
            cgenCmd(cmd->left, out, scopes);
            break;

        case NODE_COMANDO:
            if (strcmp(cmd->lexema, "=") == 0) {
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    cgenEx(cmd->right, out, scopes); 
                    
                    int offset = -(var->pos * 4); 
                    fprintf(out, "\t# Atualizando variavel %s\n", var->nome);
                    fprintf(out, "\tsw $s0, %d($fp)\n", offset);
                }
            } 
            else if (strcmp(cmd->lexema, "se") == 0 || strcmp(cmd->lexema, "se_senao") == 0) {
                cgenIf(cmd->left, cmd->right, cmd->extra, out, scopes);
            }
            else if (strcmp(cmd->lexema, "enquanto") == 0) {
                cgenWhile(cmd->left, cmd->right, out, scopes);
            }
            else if (strcmp(cmd->lexema, "leia") == 0) {
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    int offset = -(var->pos * 4);
                    fprintf(out, "\tli $v0, 5\n");
                    fprintf(out, "\tsyscall\n");
                    fprintf(out, "\tsw $v0, %d($fp)\n", offset); 
                }
            }
            else if (strcmp(cmd->lexema, "escreva") == 0) {
                if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\"') {
                    int str_lbl = label_count++;
                    
                    fprintf(out, "\t.data\n");
                    fprintf(out, "str_lit_%d:\n", str_lbl);
                    fprintf(out, "\t.asciiz %s\n", cmd->left->lexema);
                    fprintf(out, "\t.text\n");
                    
                    fprintf(out, "\t# Chamada syscall 4 - Escrever Texto Literal\n");
                    fprintf(out, "\tli $v0, 4\n");
                    fprintf(out, "\tla $a0, str_lit_%d\n", str_lbl);
                    fprintf(out, "\tsyscall\n");
                }  
                else {
                    cgenEx(cmd->left, out, scopes); 
                    
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
            fprintf(out, "\t.data\n");
            fprintf(out, "nl_lit_%d:\n", nl_lbl);
            fprintf(out, "\t.asciiz \"\\n\"\n");
            fprintf(out, "\t.text\n");
            fprintf(out, "\tli $v0, 4\n");
            fprintf(out, "\tla $a0, nl_lit_%d\n", nl_lbl);
            fprintf(out, "\tsyscall\n");
            break;
        }

        case NODE_BLOCO:
            pushScope(scopes);
            
            int posLivreAntiga = scopes->pos_livre;
            cgenCmd(cmd->left, out, scopes); 
            
            int varsDeclaradas = scopes->pos_livre - posLivreAntiga;
            if (varsDeclaradas > 0) {
                fprintf(out, "\t# Reservando %d variaveis locais na Pilha MIPS\n", varsDeclaradas);
                fprintf(out, "\taddiu $sp, $sp, -%d\n", varsDeclaradas * 4); 
            }

            cgenCmd(cmd->right, out, scopes); 
            popScope(scopes);
            break;

        case NODE_DECL_VAR:
            if (strcmp(cmd->lexema, "lista_var") == 0) {
                extrairVariaveisMIPS(cmd->left, scopes);
                cgenCmd(cmd->right, out, scopes);
            } else if (strcmp(cmd->lexema, "ids") == 0) {
                extrairVariaveisMIPS(cmd, scopes);
            }
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

    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "main:\n");

    fprintf(out, "\tmove $fp, $sp\n\n");

    initStack(scopes);

    cgenCmd(root, out, scopes);

    fprintf(out, "\n# Sair do Programa MIPS (Exit)\n");
    fprintf(out, "\tli $v0, 10\n");
    fprintf(out, "\tsyscall\n");

    fclose(out);
}
