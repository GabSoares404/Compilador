%{
/* 
 * PARTE 1 - DEFINIÇÕES C
 * Variáveis globais contendo a linha do flex, o motor e bibliotecas. 
 */
#include <stdio.h>
#include <stdlib.h>
#include "AST/ast.h"
#include "Semantic/semantic.h"

extern int yylineno; /* Puxa a contagem de linhas do Flex */
extern char* yytext; /* Puxa o texto lido pelo Flex */
extern int yylex();  /* Invoca o motor do Flex */
extern FILE *yyin;

AST* ast_raiz = NULL; /* Onde guardaremos a árvore para repassar ao Analisador Semântico */

void yyerror(const char *s);
%}

/*
 * PARTE 2 - DEFINIÇÕES DO BISON E TOKENS
 * Aqui declaramos configurações e listamos todos os tokens gerados pelo Lexer.
 */
%define parse.error verbose
%union {
    char* str;    /* Compartimento para textos soltos do Flex */
    struct AST* node; /* Compartimento para os nossos Nós Árvore */
}
/* AQUI DECLARAREMOS NOSSOS TOKENS */

%token PRINCIPAL INT CAR LEIA ESCREVA NOVALINHA SE ENTAO SENAO FIMSE ENQUANTO
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL
%token <str> IDENTIFICADOR INTCONST CADEIACARACTERES CARCONST

/* Obriga as suas regras da gramática inteira a usarem as caixas do compartimento de nós */
%type <node> Programa DeclPrograma Bloco VarSection ListaDeclVar DeclVar Tipo ListaIds ListaComando Comando Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr

%start Programa

%%

/* 
 * PARTE 3 - REGRAS DE PRODUÇÃO (GRAMÁTICA)
 * AQUI VIRÃO AS REGRAS DA GRAMÁTICA G-V1 DA NOSSA LINGUAGEM!
 */
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

DeclVar : /* epsilon - vazio */     { $$ = NULL; }
        | ',' IDENTIFICADOR DeclVar { 
             AST* idNode = createNode(NODE_IDENTIFICADOR, $2, yylineno, NULL, NULL, NULL);
             $$ = createNode(NODE_DECL_VAR, "ids", yylineno, idNode, $3, NULL);
        }
        ;

/* Dica: Tipos não precisam de nós soltos na AST compacta, a tabela de simbolos fará isso. Passamos NULL para focar nas variáveis apenas */
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
        | ENQUANTO '(' Expr ')' ListaComando { $$ = createNode(NODE_COMANDO, "enquanto", yylineno, $3, $5, NULL); }
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

/* 
 * PARTE 4 - CÓDIGO C ADICIONAL
 * Crianção da função main() para iniciar o arquivo e a função yyerror()
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

    yyparse(); /* Inicia a montagem da Árvore AST! */

    fclose(yyin);
    
    // Verificamos se o g-v1 montou a Árvore Completa e obteve a Raiz sem erros
    if (ast_raiz != NULL) {
        // Inicializamos o motor de Escopos central
        Stack tabelaDeSimbolos;
        initStack(&tabelaDeSimbolos);
        
        printf("\n>>> Iniciando O CAMINHANTE SEMANTICO...\n");
        checkSemantics(ast_raiz, &tabelaDeSimbolos);
        
        printf("\n>>> SUCESSO! O G-V1 compilou '%s' e nao encontrou \nnenhum erro Semantico, Sintatico ou Lexico na fita!\n", argv[1]);
    }

    return 0;
}
