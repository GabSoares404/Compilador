#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Contador universal para a emissão contínua de labels do MIPS
int label_count = 0;

// DESCRIÇÃO: Função Auxiliar que varre a AST na ramificação de declarações (var x, y;)
// Ela insere essas variáveis na Tabela de Símbolos garantindo que ganhem uma 'Posição' (pos) contínua para MIPS.
void extrairVariaveisMIPS(AST* nodeIds, Stack* scopes) {
    if (nodeIds == NULL) return;
    AST* varNode = nodeIds->left;
    if (varNode != NULL && varNode->type == NODE_IDENTIFICADOR) {
        insertSymbol(scopes, varNode->lexema, 0); // O 'tipo' aqui nao importa. So a Posicão MIPS
    }
    extrairVariaveisMIPS(nodeIds->right, scopes);
}

// DESCRIÇÃO: O Coração Matemático (Expression Generator). 
// Serve para processar as Equações da G-V1 transformando-as em arquitetura de "Máquina de Pilha" do MIPS.
// Ele calcula e sempre guarda o resultado no Acumulador ($s0), usando a Pilha ($sp) como memória provisória.
void cgenEx(AST* expr, FILE* out, Stack* scopes) {
    if (!expr) return;

    if (expr->type == NODE_INTCONST || expr->type == NODE_CARCONST) {
        // Caso Base 1: Se for um número solto ou caractere ('c', 5), ele carrega no Acumulador
        fprintf(out, "\t# Lendo valor primitivo: %s\n", expr->lexema);
        fprintf(out, "\tli $s0, %s\n", expr->lexema);
    } 
    else if (expr->type == NODE_IDENTIFICADOR) {
        // Caso Base 2: É uma variável! Consultamos onde ela está escondida no FP e puxamos
        Symbol* var = lookup(scopes, expr->lexema);
        if (var) {
            int offset = -(var->pos * 4);
            fprintf(out, "\t# Lendo variavel %s\n", expr->lexema);
            fprintf(out, "\tlw $s0, %d($fp)\n", offset);
        }
    } 
    else if (expr->type == NODE_OP) {
        // Caso Recursivo: Sinal de Operação!
        
        // 1. Tratando OPEradores UNÁRIOS (Ex: !x, -5). O nó só tem o lado direito preenchido
        if (expr->left == NULL && expr->right != NULL) {
            cgenEx(expr->right, out, scopes); // Ex: Avalia o número 5 pro $s0
            if (strcmp(expr->lexema, "-") == 0) {
                fprintf(out, "\tneg $s0, $s0\n"); // Inverte o sinal pra Negativo
            } else if (strcmp(expr->lexema, "!") == 0) {
                fprintf(out, "\tseq $s0, $s0, 0\n"); // Negação Lógica (Se 0 vira 1, se não, 0)
            }
            return; // Terminou o Operatório Unário, vai pro próximo galho da recursidade.
        }

        // 2. Tratando OPERADORES BINÁRIOS! A Famosa RECEITA da Máquina de Pilha
        fprintf(out, "\t# [Operador %s]: Avaliando lado Esquerdo\n", expr->lexema);
        cgenEx(expr->left, out, scopes); 
        
        fprintf(out, "\t# Empilhando esquerdo de (%s)\n", expr->lexema);
        fprintf(out, "\tsw $s0, 0($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
        
        fprintf(out, "\t# [Operador %s]: Avaliando lado Direito\n", expr->lexema);
        cgenEx(expr->right, out, scopes);
        
        fprintf(out, "\t# Desempilhando pra efetuar calculo com Direito ($s0)\n");
        fprintf(out, "\tlw $t1, 4($sp)\n");
        fprintf(out, "\taddiu $sp, $sp, 4\n");

        // 3. O embate final: Unindo as peças da Balança ($t1 operando $s0)
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
            fprintf(out, "\tor $s0, $t1, $s0\n");     // Junta tudo
            fprintf(out, "\tsne $s0, $s0, 0\n");      // Força a saída a ser só 1 ou 0 real
        }
        else if (strcmp(expr->lexema, "&") == 0) {
            fprintf(out, "\tsne $t1, $t1, 0\n");      // Limpa pra Booleanos Perfeitos
            fprintf(out, "\tsne $s0, $s0, 0\n");
            fprintf(out, "\tand $s0, $t1, $s0\n"); 
        }
        fprintf(out, "\t# [Fim Operador %s]\n", expr->lexema);
    }
}

// DESCRIÇÃO: Macro para Estruturas Condicionais (SE / SENÃO).
// Serve para disparar a verificação matemática e, com base nela (0 falso, 1 verdadeiro), 
// desviar o código usando SALTOS (branchs) pulando blocos de execução MIPS se falhar.
void cgenIf(AST* left, AST* right, AST* extra, FILE* out, Stack* scopes) {
    int lbl = label_count++;
    
    // 1. Avalia a Condição e guarda no $s0
    cgenEx(left, out, scopes); 
    
    if (extra == NULL) {
        // [IF SIMPLES] Se $s0 for falso (0), pula direto pro fim do bloco
        fprintf(out, "\tbeq $s0, $zero, fim_if_%d\n", lbl);
        cgenCmd(right, out, scopes); // Código do IF (THEN)
        fprintf(out, "fim_if_%d:\n", lbl);
    } else {
        // [IF-ELSE] Se falso, pula pro Senão
        fprintf(out, "\tbeq $s0, $zero, senao_%d\n", lbl);
        
        cgenCmd(right, out, scopes); // Código do THEN
        fprintf(out, "\tj fim_if_%d\n", lbl); // Evita executar o Senão
        
        fprintf(out, "senao_%d:\n", lbl);
        cgenCmd(extra, out, scopes); // Código do ELSE
        
        fprintf(out, "fim_if_%d:\n", lbl);
    }
}

// DESCRIÇÃO: Macro para Laços de Repetição (ENQUANTO).
// Parecido com o if, porém forçando um JUMP contínuo de volta à âncora inicial,
// até que o Acumulador indique que a condição reprovou e quebre o ciclo indo pro Fim.
void cgenWhile(AST* left, AST* right, FILE* out, Stack* scopes) {
    int lbl = label_count++;
    
    fprintf(out, "inicio_while_%d:\n", lbl);
    
    // 1. Condição do Laço
    cgenEx(left, out, scopes);
    
    // 2. Quebra o laço se falhar (pula lá pro final)
    fprintf(out, "\tbeq $s0, $zero, fim_while_%d\n", lbl);
    
    // 3. Executa o cerne do código (Lista de Comandos de dentro do While)
    cgenCmd(right, out, scopes);
    
    // 4. Volta automaticamente pra re-testar
    fprintf(out, "\tj inicio_while_%d\n", lbl);
    
    fprintf(out, "fim_while_%d:\n", lbl);
}

// DESCRIÇÃO: O Caminhante do Ciclo de Vida MIPS (Command Generator)
// Ele vasculha a AST detectando e alocando Blocos Variáveis, e distribuindo as tarefas
// (Se for uma Atribuição salva em $fp, se for Syscall gera leitura/escrita com $v0).
void cgenCmd(AST* cmd, FILE* out, Stack* scopes) {
    if (!cmd) return;

    switch (cmd->type) {
        case NODE_PROGRAMA:
            cgenCmd(cmd->left, out, scopes);
            break;

        case NODE_COMANDO:
            // --- A. ATRIBUIÇÃO ---
            if (strcmp(cmd->lexema, "=") == 0) {
                // Acha de onde a variável herda sua Posição global!
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    cgenEx(cmd->right, out, scopes); // Roda toda a equação que existe no Lado Direito pro $s0
                    int offset = -(var->pos * 4);
                    fprintf(out, "\t# Guardando variavel alterada: %s\n", var->nome);
                    fprintf(out, "\tsw $s0, %d($fp)\n", offset);
                }
            } 
            // --- B. CONDICIONAL (IF) ---
            else if (strcmp(cmd->lexema, "se") == 0 || strcmp(cmd->lexema, "se_senao") == 0) {
                cgenIf(cmd->left, cmd->right, cmd->extra, out, scopes);
            }
            // --- C. LAÇO (WHILE) ---
            else if (strcmp(cmd->lexema, "enquanto") == 0) {
                cgenWhile(cmd->left, cmd->right, out, scopes);
            }
            // --- D. LEIA ---
            else if (strcmp(cmd->lexema, "leia") == 0) {
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    int offset = -(var->pos * 4);
                    fprintf(out, "\t# Syscall [5] - Leia Int do Teclado\n");
                    fprintf(out, "\tli $v0, 5\n");
                    fprintf(out, "\tsyscall\n");
                    fprintf(out, "\tsw $v0, %d($fp)\n", offset); // Transfere do $v0 fixo pra gaveta dela!
                }
            }
            // --- E. ESCREVA ---
            else if (strcmp(cmd->lexema, "escreva") == 0) {
                // Se for um bloco inteiro de texto (String Literal): "Olá Mundo!"
                if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\"') {
                    int str_lbl = label_count++;
                    fprintf(out, "\t.data\n");
                    fprintf(out, "str_lit_%d:\n", str_lbl);
                    fprintf(out, "\t.asciiz %s\n", cmd->left->lexema);
                    fprintf(out, "\t.text\n");
                    
                    fprintf(out, "\t# Syscall [4] - Escreva Cadeia Literaria\n");
                    fprintf(out, "\tli $v0, 4\n");
                    fprintf(out, "\tla $a0, str_lit_%d\n", str_lbl);
                    fprintf(out, "\tsyscall\n");
                }  
                else {
                    // Qualquer outra coisa (Inteiros ou variáveis processadas puras)
                    cgenEx(cmd->left, out, scopes); // Puxa pro $s0 (Se A=2 puxa 2, se 'c'=x puxa ASCII(c))
                    
                    // Se for caractere forte detectado ('C'), exigirá o tipo de print 11  
                    if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\'') {
                        fprintf(out, "\t# Syscall [11] - Imprime Caractere Unico\n");
                        fprintf(out, "\tli $v0, 11\n");
                        fprintf(out, "\tmove $a0, $s0\n");
                        fprintf(out, "\tsyscall\n");
                    } else {
                        fprintf(out, "\t# Syscall [1] - Imprime Inteiros\n");
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
            // 1. Cria a bandeja de escopo novo
            pushScope(scopes);
            
            // 2. Lemos as variáveis do escopo pra calcular quanto de Pilha descer
            int posLivreAntiga = scopes->pos_livre;
            cgenCmd(cmd->left, out, scopes); // Isso chamará os cases de NODE_DECL_VAR abaixo!
            
            int varsDeclaradas = scopes->pos_livre - posLivreAntiga;
            if (varsDeclaradas > 0) {
                fprintf(out, "\t# Alocando %d Variaveis Locais na Pilha\n", varsDeclaradas);
                fprintf(out, "\taddiu $sp, $sp, -%d\n", varsDeclaradas * 4);
            }

            // 3. Lê o miolo de código
            cgenCmd(cmd->right, out, scopes); 
            
            // 4. Joga a bandeja fora na saída
            popScope(scopes);
            break;

        case NODE_DECL_VAR:
            // Re-insere as Variáveis pra engatilhar e adquirir a Posição Global delas
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

// DESCRIÇÃO: Função Principal (Entry Point do Gerador).
// Serve para criar o arquivo .s final, configurar o prologo (.text) e o Frame Pointer da Função principal.
// Roda o empurrão inicial da esteira de Comandos enviando a Raiz.
void generateCode(AST* root, Stack* scopes, const char* out_filename) {
    FILE* out = fopen(out_filename, "w");
    if (!out) {
        printf("ERRO FATAL: Nao foi possivel criar o arquivo %s!\n", out_filename);
        exit(1);
    }

    printf("\n[GERACAO DE CODIGO] >>> Iniciando traducao MIPS...\n");

    // 1. Cabeçalho MIPS Padrão
    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "main:\n");

    // 2. Ancorar o Frame Pointer do MIPS
    fprintf(out, "\tmove $fp, $sp\n\n");

    // 3. Reiniciar a bateria da Tabela de Símbolos, 
    // pois o Semântico a usou e o Gerador vai usá-la denovo para indexar Memória Real (pos)
    initStack(scopes);

    // 4. Invocar o caminhante mor que engole Comandos
    cgenCmd(root, out, scopes);

    // 5. Finalizador (Instrução Syscall Exit no Arquivo Gerado)
    fprintf(out, "\n# Sair do Programa (Syscall Exit 10)\n");
    fprintf(out, "\tli $v0, 10\n");
    fprintf(out, "\tsyscall\n");

    fclose(out);
    printf("[GERACAO DE CODIGO] >>> Codigo MIPS finalizado e salvo com SUCESSO!\n");
}
