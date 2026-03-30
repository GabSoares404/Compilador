%{
#include "g-v1.tab.h"
#include <string.h>
%}

%option yylineno
%option noyywrap

%%

"principal"     { return PRINCIPAL; }
"se"            { return SE; }
"entao"         { return ENTAO; }
"senao"         { return SENAO; }
"fimse"         { return FIMSE; }
"enquanto"      { return ENQUANTO; }
"int"           { return INT; }
"car"           { return CAR; }
"leia"          { return LEIA; }
"escreva"       { return ESCREVA; }
"novalinha"     { return NOVALINHA; }

"=="            { return IGUAL; }
"!="            { return DIFERENTE; }
">="            { return MAIORIGUAL; }
"<="            { return MENORIGUAL; }

"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
";"             { return ';'; }
":"             { return ':'; }
","             { return ','; }
"="             { return '='; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"!"             { return '!'; }
">"             { return '>'; }
"<"             { return '<'; }

[a-zA-Z_][a-zA-Z0-9_]* { return IDENTIFICADOR; }
[0-9]+                 { return INTCONST; }

[ \t\r\n]+      { /* ignora brancos */ }

.               { printf("ERRO: CARACTERE INVALIDO %s na linha %d\n", yytext, yylineno); }

%%