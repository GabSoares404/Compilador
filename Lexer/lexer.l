%{

#include <stdio.h>
#include <stdlib.h>
#include "../Parser/parser.h"
#include <string.h>

/* [MODIFICACAO] Variavel global para guardar a linha exata onde um comentario '/*' comecou.
 * Usada para reportar a linha inicial correta caso o arquivo acabe de repente sem o fecha-comentario. */
int comment_start_line = 0;
%}

%option yylineno
%option noyywrap
%x COMMENT

%%

"/*"                { comment_start_line = yylineno; BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>\n         { /* faz nada ignorando loop infinito, e nosso option yylineno incrementara localmente a linha sozinho */ }
<COMMENT>.          { /* consome o lixo dentro do comentario e recicla memoria na ram ativamente */ }
<COMMENT><<EOF>>    { printf("ERRO: COMENTÁRIO NAO TERMINA %d\n", comment_start_line <= 2 ? 1 : comment_start_line);
                      exit(1); }

[ \t\r\n]+          { /* Ignora espacos e quebras de linha ativas */ }

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

"global"            { return GLOBAL; }
"funcao"            { return FUNCAO; }
"retorne"           { return RETORNE; }

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
"[" |
"]" |
";" |
":" |
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

\'[^\n']+\'         { 
    printf("ERRO: MAIS DE UM CARACTERE NA CONST CAR %d\n", yylineno); 
    exit(1); 
}

.                   { printf("ERRO: CARACTERE INVÁLIDO %d\n", yylineno);
                      exit(1); }


%%
