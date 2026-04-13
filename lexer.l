%{
/* 
 * PARTE 1 - DEFINIÇÕES C
 * Aqui importamos bibliotecas e declaramos variáveis globais 
 * que o nosso código em C precisará usar. 
 */
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>
%}

/*
 * PARTE 2 - DEFINIÇÕES DO FLEX 
 * Aqui nós dizemos ao Flex como lidar com linhas 
 * e definimos alguns estados e atalhos.
 */
%option yylineno
%option noyywrap
%x COMMENT

%%

 /* 
  * PARTE 3 - REGRAS LÉXICAS 
  * Aqui virão as nossas Expressões Regulares que transformarão
  * os textos que estão no nosso arquivo em "Tokens"
  */


"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>\n         { /* faz nada, mas nosso option yylineno incrementará a linha */ }
<COMMENT>.          { /* consome o lixo dentro do comentario */ }
<COMMENT><<EOF>>    { printf("ERRO: COMENTÁRIO NAO TERMINA %d\n", yylineno);
                      exit(1); }


[ \t\r\n]+          { /* Ignora espacos e quebras de linha */ }

"principal"         { return PRINCIPAL; }
"int"               { return INT; }
"car"               { return CAR; }
"leia"              { return LEIA; }
"escreva"           { return ESCREVA; }
"novalinha"         { return NOVALINHA; }
"se"                { return SE; }
"entao"             { return ENTAO; }
"senao"             { return SENAO; }
"fimse"             { return FIMSE; }
"enquanto"          { return ENQUANTO; }
"||"                { return OU; }
"&"                 { return E; }
"=="                { return IGUAL; }
"!="                { return DIFERENTE; }
">="                { return MAIORIGUAL; }
"<="                { return MENORIGUAL; }

"{" |
"}" |
"(" |
")" |
";" |
"," |
"=" |
"+" |
"-" |
"*" |
"/" |
"<" |
">" |
"!"                 { return yytext[0]; }

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext);
    return IDENTIFICADOR; 
    }

[0-9]+      { 
    yylval.str = strdup(yytext);
    return INTCONST; 
    }

\"[^\"\n]*\"        { 
    yylval.str = strdup(yytext);
    return CADEIACARACTERES; 
    }

\"[^\"\n]*\n        { printf("ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n", yylineno - 1); 
                      exit(1); }

\'([^'\n\\]|\\.)\'  { 
    yylval.str = strdup(yytext);
    return CARCONST; 
    }

.                   { printf("ERRO: CARACTERE INVÁLIDO %d\n", yylineno);
                      exit(1); }


%%

/* 
 * PARTE 4 - CÓDIGO C ADICIONAL (Opcional)
 * (Geralmente deixamos em branco, pois a função main ficará no Bison)
 */
