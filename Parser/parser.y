%{
/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 1 - DEFINIÇÕES C GLOBAIS E IMPORTAÇÕES
 * ============================================================================
 * O QUE É: Este cabeçalho delimitado entre '%{' e '%}' injeta código direto em C 
 * no futuro arquivo 'parser.tab.c' gerado automaticamente pelo Bison.
 * 
 * PARA QUE SERVE: Estabelece as pontes de comunicação vital do projeto. Ele liga 
 * o Motor Matemático Sintático (Bison) às bibliotecas auxiliares, importa os Nodes 
 * da Árvore de Sintaxe (AST) e possibilita a inserção das assinaturas globais 
 * requeridas (como funções Semânticas e o Gerador de Código).
 * 
 * COMO FUNCIONA: Durante a geração `.c`, o Bison transcreve esse bloco ao topo. 
 * As diretivas 'extern' são cruciais: Elas exigem permissão do Sistema Operacional 
 * para acessar a RAM indexada pelo 'Lexer', escancarando acesso às strings brutas lidas 
 * no 'yytext' e aos logs de falha na contagem ativa do 'yylineno'.
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../AST/ast.h"
#include "../Semantic/semantic.h"
#include "../GeradorCodigo/codegen.h"

extern int yylineno; /* Puxa a contagem de linhas do motor do Flex */
extern char* yytext; /* Compartilhamento de string crua lida pelo Flex */
extern int yylex();  /* Assinatura para que o Bison invoque o motor de busca do Flex */
extern FILE *yyin;   /* Ponteiro do buffer de arquivo injetado */

AST* ast_raiz = NULL; /* Instância Matriz (Root) repassada ao Semântico e C-Gen ao fim */

void yyerror(const char *s); /* Assinatura protótipo pra avisos impiedosos de crashes Sintáticos */
%}

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 2 - DEFINIÇÕES DO BISON (TOKENS E METADADOS)
 * ============================================================================
 * O QUE É: O mapa tipográfico e estruturação de permissões de montagem do LALR.
 * 
 * PARA QUE SERVE: Lista todas as entidades que o Parser pode aceitar. É aqui
 * que ensinamos o C estrutural a diferenciar quais tipos de varíaveis (Tipagem)
 * pertencem aos Nós da Gramática (Terminais X Não-Terminais).
 * 
 * COMO FUNCIONA: 
 * - '%define parse.error verbose': Ativa depuração inteligente exigindo que o C 
 *   imprima no prompt *o que ele esperava ter lido* ao invés de um erro curto genérico.
 * - '%union': A mais complexa união de memória volátil C. Como nós e strings viajam
 *   em caminhos independentes, o union cria o objeto central 'yylval' com múltiplas opções
 *   permitindo que o Lexer mande textos ('str'), e o próprio Bison trafegue 
 *   Nodes complexos ('node').
 * - '%token': Folhas da árvore (Letras primárias e fixas injetadas do Flex).
 * - '%type': Partes Ramificadas criadas (Obriga as reduções do Bison a sempre
 *   armazenarem nós `AST node` na pilha estruturada, permitindo amarrações ascendentes).
 * ============================================================================
 */
%define parse.error verbose
%union {
    char* str;        /* Compartimento para capturas textuais soltas extraídas do Flex */
    struct AST* node; /* Compartimento alocado para os complexos Nós Árvore da RAM */
}

/* 
 * [EXPLICAÇÃO DA REGRA] TOKENS TERMINAIS 
 * Componentes básicos imutáveis retornados do Flex. Os marcados com "<str>" exigem 
 * uso prioritário de resgate com 'yylval.str'.
 */
%token PRINCIPAL INT CAR LEIA ESCREVA NOVALINHA SE ENTAO SENAO FIMSE ENQUANTO
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL
%token <str> IDENTIFICADOR INTCONST CADEIACARACTERES CARCONST

/* 
 * [EXPLICAÇÃO DA REGRA] TOKENS NÃO-TERMINAIS (NÓS CONSTRUTORES) 
 * Entidades lógicas abstratas gramaticais engatilhadores. A regra "<node>" barra falhas, 
 * exigindo sob erro fatal que eles emitam e se liguem à infraestrutura global da AST.
 */
%type <node> Programa DeclPrograma Bloco VarSection ListaDeclVar DeclVar Tipo ListaIds ListaComando Comando Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr

/* Aponta a Raiz mor primária a qual a gramática precisa reduzir até satisfazer 100% o PC */
%start Programa

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 3 - REGRAS DE PRODUÇÃO (COMO LER O CÓDIGO BISON)
 * ============================================================================
 * O QUE É: este bloco (entre os '%%') define a gramática da linguagem usando
 * a sintaxe do Bison. Aqui está literalmente "a linguagem" que o compilador aceita.
 *
 * ----------------------------------------------------------------------------
 * COMO LER A ESTRUTURA DO CÓDIGO:
 *
 * Tudo aqui segue um padrão fixo:
 *
 *     NomeRegra : produção1 { ação }
 *               | produção2 { ação }
 *               ;
 *
 * Ou seja:
 * - Antes dos dois pontos ':' → é o nome da regra (o "não-terminal")
 * - Depois dos ':' → são as possíveis formas válidas dessa regra
 * - O '|' → significa "ou" (alternativas)
 * - O ';' → encerra a definição da regra
 * - O bloco '{ ... }' → é código C executado quando aquela produção é reconhecida
 *
 * Pense assim: o lado esquerdo "vira" o lado direito quando a regra casa.
 *
 * ----------------------------------------------------------------------------
 * SOBRE OS SÍMBOLOS USADOS:
 *
 * -> NÃO-TERMINAIS:
 *    São nomes como `Programa`, `Bloco`, `Expr`, etc.
 *    Eles representam estruturas da linguagem (conceitos abstratos).
 *
 * -> TERMINAIS:
 *    São tokens vindos do Lexer, como:
 *    `IDENTIFICADOR`, `INT`, `'{'`, `'+'`, etc.
 *    Eles são os elementos "reais" do texto do programa.
 *
 * ----------------------------------------------------------------------------
 * SOBRE AS CHAVES `{ ... }`:
 *
 * As chaves NÃO fazem parte da gramática — elas delimitam código C.
 *
 * Esse código é executado no momento em que o parser reconhece aquela regra.
 * É aqui que você constrói a AST, faz validações, etc.
 *
 * Exemplo:
 *
 *     A : B C { $$ = createNode(..., $1, $2); }
 *
 * Significa:
 * - Quando B seguido de C for reconhecido,
 * - execute esse código C,
 * - e produza um resultado (`$$`).
 *
 * ----------------------------------------------------------------------------
 * SOBRE `$1`, `$2`, `$$`:
 *
 * Esses símbolos representam valores dentro da regra:
 *
 * - `$1` → primeiro item do lado direito
 * - `$2` → segundo item
 * - `$3` → terceiro...
 * - `$$` → resultado final da regra (lado esquerdo)
 *
 * Exemplo:
 *
 *     Expr : Expr '+' Expr
 *
 * Aqui:
 * - `$1` = Expr da esquerda
 * - `$2` = '+'
 * - `$3` = Expr da direita
 *
 * E normalmente:
 *
 *     $$ = createNode(..., $1, $3);
 *
 * ----------------------------------------------------------------------------
 * SOBRE O FLUXO (COMO O PARSER PENSA):
 *
 * O Bison usa o modelo shift-reduce (bottom-up):
 *
 * 1. Ele lê tokens do Lexer
 * 2. Empilha esses tokens
 * 3. Quando a sequência bate com alguma regra → ele reduz
 * 4. Executa o código `{ ... }`
 * 5. Substitui aquela sequência pelo resultado (`$$`)
 *
 * Isso continua até reduzir tudo para `Programa`.
 *
 * ----------------------------------------------------------------------------
 * COMO INTERPRETAR AS REGRAS DO ARQUIVO:
 *
 * -> `Programa`:
 *    É a regra inicial. Tudo precisa virar isso no final.
 *
 * -> `DeclPrograma`:
 *    Define o formato obrigatório do programa principal.
 *
 * -> `Bloco`:
 *    Sempre leia como:
 *        '{' ... '}'
 *    Ou seja, delimitador de escopo.
 *
 * -> `ListaDeclVar`:
 *    Quando você vê uma regra chamando ela mesma:
 *
 *        ListaDeclVar : ... ListaDeclVar
 *
 *    isso é recursão → significa "lista de tamanho arbitrário".
 *
 * -> `DeclVar` com epsilon:
 *
 *        | /* vazio *
 *
 *    Isso significa: "pode não ter nada".
 *    É o que permite parar listas.
 *
 * -> `Comando`:
 *    Cada linha com '|' é uma forma diferente de comando válido.
 *    Leia como várias opções possíveis.
 *
 * ----------------------------------------------------------------------------
 * SOBRE A PRECEDÊNCIA (Expr → PrimExpr):
 *
 * A ordem das regras define a precedência:
 *
 * - Quanto mais "embaixo" no arquivo, maior a prioridade
 * - Quanto mais "em cima", menor a prioridade
 *
 * Hierarquia prática:
 *
 * PrimExpr   → base (valores, identificadores, parênteses)
 * UnExpr     → unários (+, -, !)
 * MulExpr    → * /
 * AddExpr    → + -
 * DesigExpr  → < > <= >=
 * EqExpr     → == !=
 * AndExpr    → &&
 * OrExpr     → ||
 *
 * Isso funciona porque o parser reduz primeiro o que está mais "profundo".
 *
 * ----------------------------------------------------------------------------
 * RESUMO MENTAL PRA LER QUALQUER REGRA:
 *
 * Sempre leia assim:
 *
 * "Se eu encontrar [lado direito], então eu posso reduzir para [lado esquerdo],
 * executando [código C] e produzindo [$$]."
 *
 * ============================================================================
 */

%%

Programa : DeclPrograma { ast_raiz = $1; }
         ;

DeclPrograma : PRINCIPAL Bloco { $$ = createNode(NODE_PROGRAMA, "principal", yylineno, $2, NULL, NULL); }
             ;

Bloco : '{' ListaComando '}'             { $$ = createNode(NODE_BLOCO, "bloco", yylineno, NULL, $2, NULL); }
      | VarSection '{' ListaComando '}'  { $$ = createNode(NODE_BLOCO, "bloco_var", yylineno, $1, $3, NULL); }
      ;

VarSection : '{' ListaDeclVar '}' { $$ = $2; }
           ;

ListaDeclVar : IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar { 
                 AST* idNode = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                 AST* vars = createNode(NODE_DECL_VAR, "ids", yylineno, idNode, $2, $4);
                 $$ = createNode(NODE_DECL_VAR, "lista_var", yylineno, vars, $6, NULL);
             }
             | IDENTIFICADOR DeclVar ':' Tipo ';' {
                 AST* idNode = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                 $$ = createNode(NODE_DECL_VAR, "ids", yylineno, idNode, $2, $4);
             }
             ;

DeclVar : /* epsilon - engate vazio opcional permitindo cancelamento do laço */     { $$ = NULL; }
        | ',' IDENTIFICADOR DeclVar { 
             AST* idNode = createNode(NODE_IDENTIFICADOR, $2, yylineno, NULL, NULL, NULL);
             $$ = createNode(NODE_DECL_VAR, "ids", yylineno, idNode, $3, NULL);
        }
        ;

Tipo : INT { $$ = createNode(NODE_TIPO, "int", yylineno, NULL, NULL, NULL); }
     | CAR { $$ = createNode(NODE_TIPO, "car", yylineno, NULL, NULL, NULL); }
     ;

ListaIds : ListaIds ',' IDENTIFICADOR { AST* idNode = createNode(NODE_IDENTIFICADOR, $3, yylineno, NULL, NULL, NULL); $$ = createNode(NODE_DECL_VAR, "ids", yylineno, $1, idNode, NULL); }
         | IDENTIFICADOR              { $$ = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL); }
         ;

ListaComando : ListaComando Comando  { $$ = createNode(NODE_LISTA_COMANDO, "comandos", yylineno, $1, $2, NULL); }
             | Comando               { $$ = $1; }
             ;

Comando : LEIA IDENTIFICADOR ';' { 
            AST* idNode = createNode(NODE_IDENTIFICADOR, $2, yylineno, NULL, NULL, NULL);
            $$ = createNode(NODE_COMANDO, "leia", yylineno, idNode, NULL, NULL); 
        }
        | ESCREVA Expr ';' { $$ = createNode(NODE_COMANDO, "escreva", yylineno, $2, NULL, NULL); }
        | NOVALINHA ';' { $$ = createNode(NODE_NOVALINHA, "novalinha", yylineno, NULL, NULL, NULL); }
        | IDENTIFICADOR '=' Expr ';' { 
            AST* idNode = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
            $$ = createNode(NODE_COMANDO, "=", yylineno, idNode, $3, NULL); 
        }
        | SE '(' Expr ')' ENTAO ListaComando FIMSE { $$ = createNode(NODE_COMANDO, "se", yylineno, $3, $6, NULL); }
        | SE '(' Expr ')' ENTAO ListaComando SENAO ListaComando FIMSE { $$ = createNode(NODE_COMANDO, "se_senao", yylineno, $3, $6, $8); }
        | ENQUANTO '(' Expr ')' Comando { $$ = createNode(NODE_COMANDO, "enquanto", yylineno, $3, $5, NULL); }
        | Bloco { $$ = $1; } 
        | ';' { $$ = createNode(NODE_COMANDO, "vazio", yylineno, NULL, NULL, NULL); }
        ;

Expr : OrExpr { $$ = $1; }
     ;

OrExpr : OrExpr OU AndExpr { $$ = createNode(NODE_OP, "||", yylineno, $1, $3, NULL); }
       | AndExpr { $$ = $1; }
       ;

AndExpr : AndExpr E EqExpr { $$ = createNode(NODE_OP, "&", yylineno, $1, $3, NULL); }
        | EqExpr { $$ = $1; }
        ;
        
EqExpr : EqExpr IGUAL DesigExpr { $$ = createNode(NODE_OP, "==", yylineno, $1, $3, NULL); }
       | EqExpr DIFERENTE DesigExpr { $$ = createNode(NODE_OP, "!=", yylineno, $1, $3, NULL); }
       | DesigExpr { $$ = $1; }
       ; 
    
DesigExpr : DesigExpr '<' AddExpr { $$ = createNode(NODE_OP, "<", yylineno, $1, $3, NULL); }
          | DesigExpr '>' AddExpr { $$ = createNode(NODE_OP, ">", yylineno, $1, $3, NULL); }
          | DesigExpr MAIORIGUAL AddExpr { $$ = createNode(NODE_OP, ">=", yylineno, $1, $3, NULL); }
          | DesigExpr MENORIGUAL AddExpr { $$ = createNode(NODE_OP, "<=", yylineno, $1, $3, NULL); }
          | AddExpr   { $$ = $1; }
          ;

AddExpr : AddExpr '+' MulExpr { $$ = createNode(NODE_OP, "+", yylineno, $1, $3, NULL); }
        | AddExpr '-' MulExpr { $$ = createNode(NODE_OP, "-", yylineno, $1, $3, NULL); }
        | MulExpr             { $$ = $1; }
        ;

MulExpr : MulExpr '*' UnExpr  { $$ = createNode(NODE_OP, "*", yylineno, $1, $3, NULL); }
        | MulExpr '/' UnExpr  { $$ = createNode(NODE_OP, "/", yylineno, $1, $3, NULL); }
        | UnExpr              { $$ = $1; }
        ;

UnExpr : '+' PrimExpr         { $$ = createNode(NODE_OP, "+", yylineno, NULL, $2, NULL); }
       | '-' PrimExpr         { $$ = createNode(NODE_OP, "-", yylineno, NULL, $2, NULL); }
       | '!' PrimExpr         { $$ = createNode(NODE_OP, "!", yylineno, NULL, $2, NULL); }
       | PrimExpr             { $$ = $1; }
       ;

PrimExpr : IDENTIFICADOR      { $$ = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL); }
         | INTCONST           { $$ = createNode(NODE_INTCONST, $1, yylineno, NULL, NULL, NULL); }
         | CARCONST           { $$ = createNode(NODE_CARCONST, $1, yylineno, NULL, NULL, NULL); }
         | CADEIACARACTERES   { $$ = createNode(NODE_CARCONST, $1, yylineno, NULL, NULL, NULL); }
         | '(' Expr ')'       { $$ = $2; }
         ;


%%

/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 4 - MÓDULO EXECUTIVO ADICIONAL (MAIN E LOGS)
 * ============================================================================
 * O QUE É: As peças gerenciais em puro C atreladas as execuções de fluxo final do software.
 * 
 * PARA QUE SERVE: Engatilha o ponto de ignição (Startpoint) do Software Compilador e 
 * arquiteta e documenta as falhas detectadas caso elas surjam. Adicionalmente atua 
 * acoplando as três fases inteiras de pós-ast (Tabela Ram -> DFS Sym -> Geração em IO).
 * 
 * COMO FUNCIONA: O projeto começa em 'main()', abrindo o arquivo `.g` via IO do kernel ('fopen').
 * Logo engatilha o loop em 'yyparse()' que consome Lexer com o fluxo Shift-Reduce ativo.
 * Caso yyparse traga sucesso, a Main desperta e inicia o caminhante 'checkSemantics' 
 * repassando à ele a Tabela 'Stack' nova em folha. Com o atestado semântico em 'ok', a compilação
 * funde-se e flui no arquivo elétrico chamando e imprimindo o 'generateCode()'.
 * Se no meio da jornada do 'yyparse()' a gramática falhar irrecuperavelmente, 
 * o gatilho automático interno em 'yyerror' chuta no terminal e estilhaça a execução inteira do processo.
 * ============================================================================
 */
void yyerror(const char *s) {
    printf("ERRO: %s %d\n", s, yylineno);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r"); /* O Flex irá varrer este arquivo indicado pelo argv */
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    yyparse(); /* Inicia a montagem da Árvore AST em fluxo iterativo! */

    fclose(yyin);
    
    /* Verificamos se o Bison montou a Árvore Completa e entregou a Raiz imaculada */
    if (ast_raiz != NULL) {
        /* Inicializamos estaticamente o motor de Escopos central LIFO */
        Stack tabelaDeSimbolos;
        initStack(&tabelaDeSimbolos);
        
        printf("\n>>> Iniciando O CAMINHANTE SEMANTICO...\n");
        checkSemantics(ast_raiz, &tabelaDeSimbolos);
        
        printf("\n>>> SUCESSO! O G-V1 compilou '%s' e nao encontrou \nnenhum erro Semantico, Sintatico ou Lexico na fita!\n", argv[1]);
        
        /* --- INICIANDO ETAPA DE GERAÇÃO DE CÓDIGO FINAL (MIPS ASSEMBLY) --- */
        /* Fabricando o nome do arquivo de saída: removemos a extensão original (.g) e injetamos a nova (.s) */
        char out_nome[256];
        strncpy(out_nome, argv[1], sizeof(out_nome) - 3); /* Copia de forma segura com margem pra concatenação */
        out_nome[sizeof(out_nome) - 3] = '\0';
        
        /* Buscamos o último ponto '.' de trás pra frente no nome fornecido */
        char *ponto = strrchr(out_nome, '.');
        if (ponto != NULL) {
            *ponto = '\0'; /* Cortamos a string exatamente aqui ignorando o .g final */
        }
        
        /* Anexamos a extensão pura do Assembly MIPS e gravamos */
        strncat(out_nome, ".s", sizeof(out_nome) - strlen(out_nome) - 1);
        
        generateCode(ast_raiz, &tabelaDeSimbolos, out_nome);
    }

    return 0;
}
