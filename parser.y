%{
/* 
 * PARTE 1 - DEFINIÇÕES C
 * Variáveis globais contendo a linha do flex, o motor e bibliotecas. 
 */
#include <stdio.h>
#include <stdlib.h>

extern int yylineno; /* Puxa a contagem de linhas do Flex */
extern char* yytext; /* Puxa o texto lido pelo Flex */
extern int yylex();  /* Invoca o motor do Flex */
extern FILE *yyin;

void yyerror(const char *s);
%}

/*
 * PARTE 2 - DEFINIÇÕES DO BISON E TOKENS
 * Aqui declaramos configurações e listamos todos os tokens gerados pelo Lexer.
 */
%define parse.error verbose

/* AQUI DECLARAREMOS NOSSOS TOKENS */

%token PRINCIPAL INT CAR LEIA ESCREVA NOVALINHA SE ENTAO SENAO FIMSE ENQUANTO
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL
%token IDENTIFICADOR INTCONST CADEIACARACTERES CARCONST

%start Programa

%%

/* 
 * PARTE 3 - REGRAS DE PRODUÇÃO (GRAMÁTICA)
 * AQUI VIRÃO AS REGRAS DA GRAMÁTICA G-V1 DA NOSSA LINGUAGEM!
 */
Programa : DeclPrograma
         ;

DeclPrograma : PRINCIPAL Bloco
             ;

Bloco : '{' VarSection ListaComando '}'
      | '{' ListaComando '}'
      | '{' VarSection '}'
      | '{' '}'
      ;


Tipo : INT
     | CAR
     ;

DeclVar : Tipo ListaIds ';'
        ;

ListaIds : ListaIds ',' IDENTIFICADOR
         | IDENTIFICADOR
         ;

VarSection : ListaDeclVar
           ;

ListaDeclVar : ListaDeclVar DeclVar
             | DeclVar
             ;

ListaComando : ListaComando Comando
             | Comando
             ;

Comando : LEIA IDENTIFICADOR ';'
        | NOVALINHA ';'
        | IDENTIFICADOR '=' Expr ';'
        | ESCREVA Expr ';'
        | SE '(' Expr ')' ENTAO ListaComando FIMSE
        | SE '(' Expr ')' ENTAO ListaComando SENAO ListaComando FIMSE
        | ENQUANTO '(' Expr ')' Comando
        | Bloco
        | ';'
        ;

Expr : OrExpr
     ;

OrExpr : OrExpr OU AndExpr
       | AndExpr
       ;

AndExpr : AndExpr E EqExpr
        | EqExpr
        ;
        
EqExpr : EqExpr IGUAL DesigExpr
       | EqExpr DIFERENTE DesigExpr
       | DesigExpr
       ; 
    
DesigExpr : DesigExpr '<' AddExpr
          | DesigExpr '>' AddExpr
          | DesigExpr MAIORIGUAL AddExpr
          | DesigExpr MENORIGUAL AddExpr
          | AddExpr    
          ;

AddExpr : AddExpr '+' MulExpr
        | AddExpr '-' MulExpr
        | MulExpr
        ;

MulExpr : MulExpr '*' UnExpr
        | MulExpr '/' UnExpr
        | UnExpr
        ;

UnExpr : '+' PrimExpr
       | '-' PrimExpr
       | '!' PrimExpr
       | PrimExpr
       ;

PrimExpr : IDENTIFICADOR
         | INTCONST
         | CARCONST
         | CADEIACARACTERES
         | '(' Expr ')'
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

    yyparse(); /* Inicia a verificação de gramática! */

    fclose(yyin);
    return 0;
}

