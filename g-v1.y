%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;

void yyerror(const char *s);
%}

/* Definição dos Tokens baseados na Tabela 1 do PDF */
%token PRINCIPAL SE ENTAO SENAO FIMSE ENQUANTO
%token INT CAR LEIA ESCREVA NOVALINHA
%token IDENTIFICADOR INTCONST CARCONST CADEIACARACTERES
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL

%%

/* Regra inicial simplificada para teste */
Programa: PRINCIPAL Bloco { printf("PROGRAMA RECONHECIDO COM SUCESSO!\n"); };

Bloco: '{' '}' ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "ERRO: %s na linha %d (proximo a '%s')\n", s, yylineno, yytext);
}

int main(int argc, char** argv) {
    extern FILE *yyin;
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Erro ao abrir arquivo");
            return 1;
        }
    } else {
        printf("Uso: ./g-v1 arquivo.g\n");
        return 1;
    }

    yyparse();
    return 0;
}