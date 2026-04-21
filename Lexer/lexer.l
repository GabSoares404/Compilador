%{
/* ============================================================================
 * [EXPLICAÇÃO DO BLOCO] PARTE 1 - DEFINIÇÕES GLOBAIS EM C
 * ============================================================================
 * O QUE É: Este bloco cercado por '%{' e '%}' é uma área de injeção direta de código C.
 * Tudo o que é escrito aqui é copiado integralmente pelo Flex para o topo
 * do arquivo final 'lexer.c' sem sofrer alterações sintáticas visuais.
 * 
 * PARA QUE SERVE: Serve para importar bibliotecas padrão do sistema (como stdio e stdlib),
 * incluir os cabeçalhos exportados pela fase seguinte (o nosso parser) e declarar funções
 * ou variáveis essenciais para que as nossas interações léxicas possam compilar e funcionar.
 * 
 * COMO FUNCIONA: O Flex simplesmente copia isso para o topo do seu arquivo gerado.
 * Notavelmente, incluímos "../Parser/parser.h", que foi gerado automaticamente pelo Bison.
 * É graças a esse arquivo que temos acesso oficial aos IDs (números) dos Tokens que o 
 * analisador Sintático espera receber (como as TAGS: PRINCIPAL, INT, CAR), bem como o 
 * fundamental acesso à união em RAM inter-pastas chamada 'yylval'.
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "../Parser/parser.h"
#include <string.h>
%}

 /* ============================================================================
  * [EXPLICAÇÃO DO BLOCO] PARTE 2 - DEFINIÇÕES E CONFIGURAÇÕES DO FLEX
  * ============================================================================
  * O QUE É: Área de configuração das opções operacionais subjacentes da própria ferramenta Flex.
  * 
  * PARA QUE SERVE: Controlar o comportamento padrão comportamental e declarar chaves de estados nativos (como o estado de estar dentro de um comentário multi-linha).
  * 
  * COMO FUNCIONA: 
  * - 'yylineno': Configura o Flex a manter automaticamente uma variável global 
  *   chamada yylineno, incrementando-a silenciosamente a cada quebra de linha lida (\n).
  * - 'noyywrap': Informa ao Flex de antemão que nós vamos processar apenas 1 único arquivo,
  *   então ele não precisará engatilhar a função yywrap() pra tentar procurar novos arquivos na pasta.
  * - '%x COMMENT': Cria um "Contexto ou Estado Exclusivo" na máquina Lexer chamado COMMENT. 
  *   Este estado em exclusividade desabilita lógicas padrões vitais para permitirmos que o sistema ignore chaves limitrofanas e operações matemáticas perigosas se elas estiverem escritas dentro de comentários literais "/*".
  * ============================================================================
  */
%option yylineno
%option noyywrap
%x COMMENT

%%

 /* ============================================================================
  * [EXPLICAÇÃO DO BLOCO] PARTE 3 - REGRAS LÉXICAS E EXPRESSÕES REGULARES
  * ============================================================================
  * O QUE É: O coração operacional do Lexer. Uma lista formalizada de padrões que testamos exaustivamente no texto bruto processado no exato instante.
  * 
  * PARA QUE SERVE: Dita exatamente e cirurgicamente como cada token universal será moldado e extraído na fonte natural da escrita. 
  * 
  * COMO FUNCIONA: O projeto em C gerado pelo Flex lê caracteres de cima para baixo testando com a lista abaixo simultaneamente. Se houver duplo "match" nas regras, ele é projetado para sempre dar preferência a regra de maior extensão de caracteres de preenchimento (Grep guloso), ou então à regra atirada mais acima primeiro.
  * Cada linha segue a anatomia: [EXPRESSÃO_REGULAR_A_TER_MATCH] { CÓDIGO_C_ENGATILHADO_A_EXECUTAR }
  * Todo tag disparada no código abaixo a partir de 'return' será mandada instantaneamente sem delay para avaliação quem engatilhou o lexer no Parser_Loop.
  * ============================================================================
  */

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.1 Tratamento Condicional de Comentários em Bloco 
  * Entramos no estado exclusivo COMMENT quando achamos os caracteres puros de abertura de comentario em C (barra-asterisco). Estando na tag de estado COMMENT,
  * ignoramos qualquer caractere que venha (.) através da função vazia de skip, exceto a finalização de fechamento asterisco-barra imperativa que devolve intencionalmente a Máquina ao seu normal (INITIAL). Se o compilador alcançar um EOF text (fim de arquivo abrupto) atrelados no modo comentário acionado, disparamos falha fatal.
  */
"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>\n         { /* faz nada ignorando loop infinito, e nosso option yylineno incrementará localmente a linha sozinho */ }
<COMMENT>.          { /* consome o lixo dentro do comentário e recicla memória na ram ativamente */ }
<COMMENT><<EOF>>    { printf("ERRO: COMENTÁRIO NAO TERMINA %d\n", yylineno);
                      exit(1); }

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.2 Descarte Grosseiro do Espaço Físico Limpo
  * A regra com sinalização '+', devora agressivamente os espaços seguidamente ativados, engolindo silenciosamente os \t e pulos puros de arquivo \n como descarte descartado, para não interferir na geração linear em gramática.
  */
[ \t\r\n]+          { /* Ignora espacos e quebras de linha ativas */ }

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.3 Palavras Reservadas da Linguagem (Keywords Nativas G-V1) 
  * Mapeamentos rígidos de formatações hardcoded atrelados. Mapeamos os literais estáticos exigidos pela folha do trabalho e devolvemos a porta de quem solicitou `yylex()` a etiqueta formativa gerada puramente originada nas declarações C (BISON).
  */
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

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.4 Símbolos Estruturais Matemático/Lógicos Multicaractere Formativo
  * Para que eles não disparessem a identificação de duplo "=" do Igual base ou duplo And em '&'. 
  */
"||"                { return OU; }
"&"                 { return E; }
"=="                { return IGUAL; }
"!="                { return DIFERENTE; }
">="                { return MAIORIGUAL; }
"<="                { return MENORIGUAL; }

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.5 Símbolos Estruturantes Individuais (Single Tokens Base)
  * Retorna O PRÓPRIO Caractere puro cru ASCII resgatando-o no index inicial `0` instanciado livremente da área alocativa de `yytext`. 
  * O sistema do Bison já foi devidamente formalizado como apto com a capacidade de interpretar diretrizes tokens textuais crus como '{' puro.
  */
"{" |
"}" |
"(" |
")" |
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

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.6 Expressões Regulares Customizadas de Valor Agregável Váriavel 
  * Servem para captar padrões onde o preenchimento irá influenciar no significado final. Precisam de acoplagem relacional na cópia extração String C!
  */

 /* 
  * [EXPLICAÇÃO EXTRA] Identificadores Padrões (Nomes de Variáveis em C/C++)
  * Inicia regradamente atrelado por letras baseadas/[_], seguido sequencialmente infinito por numéricos alfa-letras base/[_]. 
  * Importantíssimo processo: O código efetua `strdup()` forçando a alocação crua duradoura na ram e anexa o acesso em `yylval.str` viabilizando extração pura nas pastas sucessoras de Tabela SymTable e a própria Base C da AST. 
  */
[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext);
    return IDENTIFICADOR; 
}

 /* 
  * [EXPLICAÇÃO EXTRA] Números Matemáticos Formato Inteiros Base10 
  * Processa e requer 1 ou incontabilizando quantia numeral sequencial restritos matemáticos em (+). 
  */
[0-9]+      { 
    yylval.str = strdup(yytext);
    return INTCONST; 
}

 /* 
  * [EXPLICAÇÃO EXTRA] Cadeia Extensiva String Textual em Limitação Unifásica ("texto restrito")
  * Exige Iniciar-se na detecção aspas puríssima (\"), devorando a seguir livremente qualquer continuação que (NÃO seja \n quebra) e acoberta obrigue-se a estagnar em (\").
  */
\"[^\"\n]*\"        { 
    yylval.str = strdup(yytext);
    return CADEIACARACTERES; 
}

 /* 
  * [EXPLICAÇÃO EXTRA] Detecção De Fuga Linear - Falsa Cadeia Malformada sem limitante
  * O Lexer detecta se as aspas originais cruzam indevidas uma quebra de limitação (`\n`) disparando barreira impeditiva e matando localmente a tradução Lexer do motor C.
  */
\"[^\"\n]*\n        { printf("ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n", yylineno - 1); 
                      exit(1); }

 /* 
  * [EXPLICAÇÃO EXTRA] Caractere Primitivo Físico Formato ('t')
  * Requer isoladamente aspas simples textuais, suporta enclausuramentos especiais restritos como o `\n` interno isolados ou corpo de `1` byte estrito (`.`), findando sob a opressiva simples ('`). 
  */
\'([^'\n\\]|\\.)\'  { 
    yylval.str = strdup(yytext);
    return CARCONST; 
}

 /* 
  * [EXPLICAÇÃO DA REGRA] 3.7 A Sentinela Lixeira Externa Customizada em Ponto Final ('.') Lexo
  * A premissa central final do LEX. Se o caractere da fita não cruze positivamente (dê "Match") de fato com a totalidade sequencial prévia testada referencial acima dele, ele forçosamente cairá submetido aqui por regra temporal do Flex (Último recurso wildcard dot '.').
  * Isso desmascara instantaneamente no prompt que um caractere lixo ou proibido (como um `$` ou o `@`) em G-V1 foi lido. Traz o `exit(1)` punitivo logando com a linha final.
  */
.                   { printf("ERRO: CARACTERE INVÁLIDO %d\n", yylineno);
                      exit(1); }


%%

 /* ============================================================================
  * [EXPLICAÇÃO DO BLOCO] PARTE 4 - CÓDIGO C ADICIONAL LIVRE DA FERRAMENTA EXPRESSÃO REGULAR
  * ============================================================================
  * Tudo contido estruturalmente abaixo visual do "%%" nunca sofrerá análises de pré
  * processamento ou injeções pela ferramenta construtora do Autômato Determinístico do C (Flex). 
  * Geralmente trata-se puramente como espaço propício logado para criação de métodos e 
  * construtores lógicos suplementares a engrenar na Lex (yyerror de Lex, yyprints em dumps.. etc), 
  * todavia o Bison é nosso MasterCaller que agirá abrigando as execuções, mantendo a limpeza de arquivo.
  * ============================================================================
  */
