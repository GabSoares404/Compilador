#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variável global usada para criar IDs únicos nos rótulos (labels) do Assembly MIPS
int label_count = 0;

/* ============================================================================
 * IMPLEMENTAÇÃO DO GERADOR DE CÓDIGO (.C)
 * ============================================================================
 * O QUE FAZ:
 * Transforma a Árvore Sintática Abstrata (AST) do compilador C em código
 * final na linguagem de máquina Assembly MIPS.
 * 
 * COMO FUNCIONA:
 * O código gerado segue a arquitetura de "Máquina de Pilha" (Stack Machine):
 * - O registrador `$s0` é usado como ACUMULADOR (guarda resultados temporários).
 * - A pilha da memória (`$sp`) guarda o lado esquerdo das expressões matemáticas.
 * - O Frame Pointer (`$fp`) é usado como base fixa para localizar variáveis.
 * ============================================================================
 */


/* 
 * ----------------------------------------------------------------------------
 * 1. ALOCAÇÃO DE VARIÁVEIS NA MEMÓRIA (extrairVariaveisMIPS)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Lê as variáveis recém-declaradas na AST e as coloca na Tabela de Símbolos
 * temporariamente, garantindo que o compilador lhes dê um endereço (offset) no MIPS.
 */
void extrairVariaveisMIPS(AST* nodeIds, Stack* scopes) {
    if (nodeIds == NULL) return;
    
    AST* varNode = nodeIds->left;
    if (varNode != NULL && varNode->type == NODE_IDENTIFICADOR) {
        // O tipo não importa para o Gerador (todas as variáveis ocupam 4 bytes).
        // Isso apenas força a tabela a dar um novo 'pos_livre' para a variável.
        insertSymbol(scopes, varNode->lexema, 0); 
    }
    
    // Continua lendo a lista de variáveis
    extrairVariaveisMIPS(nodeIds->right, scopes);
}


/* 
 * ----------------------------------------------------------------------------
 * 2. GERAÇÃO DE EXPRESSÕES MATEMÁTICAS (cgenEx)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Resolve nós de cálculo lendo os valores da esquerda e da direita.
 * 
 * COMO FUNCIONA (Máquina de Pilha na prática):
 * 1. Calcula o lado esquerdo e EMPILHA o resultado (usa a `$sp`).
 * 2. Calcula o lado direito e guarda o resultado direto no `$s0` (Acumulador).
 * 3. DESEMPILHA o resultado esquerdo para um registrador temporário (`$t1`).
 * 4. Executa a conta final (ex: add $s0, $t1, $s0).
 */
void cgenEx(AST* expr, FILE* out, Stack* scopes) {
    if (!expr) return;

    if (expr->type == NODE_INTCONST || expr->type == NODE_CARCONST) {
        // CASO 1: Constante (número ou caractere avulso)
        // O 'li' (Load Immediate) carrega o valor fixo direto no acumulador $s0.
        fprintf(out, "\t# Lendo constante primitiva: %s\n", expr->lexema);
        fprintf(out, "\tli $s0, %s\n", expr->lexema);
    } 
    else if (expr->type == NODE_IDENTIFICADOR) {
        // CASO 2: Leitura de Variável
        Symbol* var = lookup(scopes, expr->lexema);
        if (var) {
            // Em MIPS, variáveis de 32-bits ocupam 4 bytes.
            // Multiplica o ID sequencial da variável por 4 para achar o offset negativo do Frame.
            int offset = -(var->pos * 4);
            fprintf(out, "\t# Lendo variavel %s do endereco %d($fp)\n", expr->lexema, offset);
            fprintf(out, "\tlw $s0, %d($fp)\n", offset);
        }
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
        fprintf(out, "\taddiu $sp, $sp, -4\n");  // Move a pilha 4 bytes para trás ANTES de salvar (protege as variáveis)
        fprintf(out, "\tsw $s0, 0($sp)\n");      // Grava $s0 na nova gaveta segura da pilha
        
        fprintf(out, "\t# [Operador %s]: Processando lado Direito\n", expr->lexema);
        cgenEx(expr->right, out, scopes);
        
        // Passo 2 da Máquina de Pilha: Puxa o esquerdo de volta mas joga no $t1.
        fprintf(out, "\t# Desempilhando valor para calculo final\n");
        fprintf(out, "\tlw $t1, 0($sp)\n");     // Lê o valor que estava salvo de volta para o $t1
        fprintf(out, "\taddiu $sp, $sp, 4\n");  // Devolve a pilha para a base original

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


/* 
 * ----------------------------------------------------------------------------
 * 3. DESVIO CONDICIONAL (cgenIf)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Controla os blocos "Se / Senao", escrevendo Rótulos de Salto (Branches) no MIPS.
 */
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


/* 
 * ----------------------------------------------------------------------------
 * 4. LAÇOS DE REPETIÇÃO CICLICA (cgenWhile)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Organiza rótulos contínuos para manter a execução travada repetindo até 
 * falhar a condição.
 */
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


/* 
 * ----------------------------------------------------------------------------
 * 5. CONVERSOR CENTRAL DA ÁRVORE (cgenCmd)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Lê qualquer Comando estrutural da AST e despacha a emissão correta do MIPS.
 */
void cgenCmd(AST* cmd, FILE* out, Stack* scopes) {
    if (!cmd) return;

    switch (cmd->type) {
        case NODE_PROGRAMA:
            cgenCmd(cmd->left, out, scopes);
            break;

        case NODE_COMANDO:
            // --- A. ATRIBUIÇÃO ---
            if (strcmp(cmd->lexema, "=") == 0) {
                // Acha o deslocamento da variável alvo na tabela de Símbolos.
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    // Resolve primeiro a conta inteira para guardar tudo pronto no acumulador ($s0)
                    cgenEx(cmd->right, out, scopes); 
                    
                    int offset = -(var->pos * 4); 
                    fprintf(out, "\t# Atualizando variavel %s\n", var->nome);
                    fprintf(out, "\tsw $s0, %d($fp)\n", offset); // Grava a palavra originada no $s0 para endereço de RAM do offset.
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
                Symbol* var = lookup(scopes, cmd->left->lexema);
                if (var) {
                    int offset = -(var->pos * 4);
                    // Syscall código 5: Lê inteiro digitado do teclado gravando no $v0
                    fprintf(out, "\t# Chamada syscall 5 - Ler Inteiro\n");
                    fprintf(out, "\tli $v0, 5\n");
                    fprintf(out, "\tsyscall\n");
                    fprintf(out, "\tsw $v0, %d($fp)\n", offset); // Transfere a resposta para a gaveta local do MIPS
                }
            }
            // --- E. FUNÇÃO ESCREVA ---
            else if (strcmp(cmd->lexema, "escreva") == 0) {
                // Checa se o usuário mandou imprimir uma palavra literal aspas (Ex: escreva "Hello")
                if (cmd->left->type == NODE_CARCONST && cmd->left->lexema[0] == '\"') {
                    int str_lbl = label_count++;
                    
                    // Coloca de propósito na seção .data para criar a String estática (Asciiz)
                    fprintf(out, "\t.data\n");
                    fprintf(out, "str_lit_%d:\n", str_lbl);
                    fprintf(out, "\t.asciiz %s\n", cmd->left->lexema);
                    fprintf(out, "\t.text\n");
                    
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
            // Imprime forçadamente quebras de linha '\n' pelo console.
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
            // Abre novo escopo local na árvore
            pushScope(scopes);
            
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
            
            // Reverte bloco, matando o escopo
            popScope(scopes);
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

        default:
            cgenCmd(cmd->left, out, scopes);
            cgenCmd(cmd->right, out, scopes);
            break;
    }
}


/* 
 * ----------------------------------------------------------------------------
 * 6. PONTO DE ENTRADA DO GERADOR (generateCode)
 * ----------------------------------------------------------------------------
 * O QUE FAZ:
 * Função principal acessada externamente pelo main().
 * Cria o arquivo `.s` novo, escreve as metadatas iniciais (Prólogo MIPS) e 
 * dá boot na travessia da AST.
 */
void generateCode(AST* root, Stack* scopes, const char* out_filename) {
    FILE* out = fopen(out_filename, "w");
    if (!out) {
        printf("ERRO FATAL: Nao foi possivel criar o arquivo %s!\n", out_filename);
        exit(1);
    }

    // Marcações MIPS (Data Segment e Main Entrypoint Globais)
    fprintf(out, ".text\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "main:\n");

    // Equalização Limítrofe: Faz com que o MIPS copie o endereço base que estava na Pilha $SP para ser nosso topo oficial imutável de Offset $FP.
    fprintf(out, "\tmove $fp, $sp\n\n");

    // Reinicia o offset de memória que foi usado pelo passo semântico, restaurando-o para uso físico no Code Gen.
    initStack(scopes);

    cgenCmd(root, out, scopes);

    // Sistema Syscall: Operador '10' requisita interrupção estática limpa fechando processo logado no emulador SPIM MIPS
    fprintf(out, "\n# Sair do Programa MIPS (Exit)\n");
    fprintf(out, "\tli $v0, 10\n");
    fprintf(out, "\tsyscall\n");

    fclose(out);
}
