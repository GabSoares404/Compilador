%{

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

%define parse.error verbose
%union {
    char* str;        /* Compartimento para capturas textuais soltas extraídas do Flex */
    struct AST* node; /* Compartimento alocado para os complexos Nós Árvore da RAM */
}


%token PRINCIPAL INT CAR LEIA ESCREVA NOVALINHA SE ENTAO SENAO FIMSE ENQUANTO
%token GLOBAL FUNCAO RETORNE
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL
%token <str> IDENTIFICADOR INTCONST CADEIACARACTERES CARCONST


%type <node> Programa DeclPrograma Bloco VarSection ListaDeclVar DeclVar Tipo ListaIds ListaComando Comando Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr DeclVarGlobais DeclFunc ListaFuncoes ListaParametros ListaParametrosTail DeclVarItem ListaArgs ListaArgsTail

%start Programa


%%

Programa : DeclVarGlobais DeclFunc DeclPrograma {
             /* AST Raiz agora abraça Globais, Funções e o Principal */
             ast_raiz = createNode(NODE_PROGRAMA, "programa", yylineno, $1, $2, $3);
         }
         ;

DeclVarGlobais : GLOBAL VarSection { $$ = createNode(NODE_DECL_GLOBAL, "global", yylineno, $2, NULL, NULL); }
               | /* vazio */       { $$ = NULL; }
               ;

DeclFunc : FUNCAO '[' ListaFuncoes ']' { $$ = $3; }
         | /* vazio */                 { $$ = NULL; }
         ;

ListaFuncoes : IDENTIFICADOR '(' ListaParametros ')' ':' Tipo Bloco ListaFuncoes {
                 AST* funcNode = createFuncNode(NODE_DECL_FUNC, $1, yylineno, $3, $6, $7, NULL);
                 $$ = createNode(NODE_LISTA_FUNC, "funcoes", yylineno, funcNode, $8, NULL);
             }
             | /* vazio */ { $$ = NULL; }
             ;

ListaParametros : ListaParametrosTail { $$ = $1; }
                | /* vazio */         { $$ = NULL; }
                ;

ListaParametrosTail : IDENTIFICADOR ':' Tipo {
                        AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                        $$ = createNode(NODE_PARAM, "param", yylineno, id, $3, NULL);
                    }
                    | IDENTIFICADOR '[' ']' ':' Tipo {
                        /* G-V2: Parâmetro vetor passado por referência */
                        AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                        AST* param = createNode(NODE_PARAM, "param_vetor", yylineno, id, $5, NULL);
                        $$ = param;
                    }
                    | IDENTIFICADOR ':' Tipo ',' ListaParametrosTail {
                        AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                        AST* p = createNode(NODE_PARAM, "param", yylineno, id, $3, NULL);
                        $$ = createNode(NODE_PARAM, "lista_param", yylineno, p, $5, NULL);
                    }
                    | IDENTIFICADOR '[' ']' ':' Tipo ',' ListaParametrosTail {
                        AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                        AST* p = createNode(NODE_PARAM, "param_vetor", yylineno, id, $5, NULL);
                        $$ = createNode(NODE_PARAM, "lista_param", yylineno, p, $7, NULL);
                    }
                    ;

DeclPrograma : PRINCIPAL Bloco { $$ = createNode(NODE_PROGRAMA, "principal", yylineno, $2, NULL, NULL); }
             ;

Bloco : '{' ListaComando '}'             { $$ = createNode(NODE_BLOCO, "bloco", yylineno, NULL, $2, NULL); }
      | VarSection '{' ListaComando '}'  { $$ = createNode(NODE_BLOCO, "bloco_var", yylineno, $1, $3, NULL); }
      ;

VarSection : '[' ListaDeclVar ']' { $$ = $2; }
           ;

DeclVarItem : IDENTIFICADOR { $$ = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL); }
            | IDENTIFICADOR '[' INTCONST ']' {
                AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
                AST* tam = createNode(NODE_INTCONST, $3, yylineno, NULL, NULL, NULL);
                $$ = createNode(NODE_VETOR_DECL, "vetor_decl", yylineno, id, tam, NULL);
            }
            ;

ListaDeclVar : DeclVarItem DeclVar ':' Tipo ';' ListaDeclVar {
                 AST* itemNode = $1;
                 AST* vars = createNode(NODE_DECL_VAR, "ids", itemNode->linha, itemNode, $2, $4);
                 $$ = createNode(NODE_DECL_VAR, "lista_var", itemNode->linha, vars, $6, NULL);
             }
             | DeclVarItem DeclVar ':' Tipo ';' {
                 AST* itemNode = $1;
                 $$ = createNode(NODE_DECL_VAR, "ids", itemNode->linha, itemNode, $2, $4);
             }
             ;

DeclVar : /* epsilon - engate vazio opcional permitindo cancelamento do laço */     { $$ = NULL; }
        | ',' DeclVarItem DeclVar {
             AST* itemNode = $2;
             $$ = createNode(NODE_DECL_VAR, "ids", itemNode->linha, itemNode, $3, NULL);
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

Comando : IDENTIFICADOR '=' Expr ';' {
            AST* idNode = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
            $$ = createNode(NODE_COMANDO, "=", yylineno, idNode, $3, NULL);
        }
        | IDENTIFICADOR '[' Expr ']' '=' Expr ';' {
            AST* idNode = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
            AST* vetorNode = createNode(NODE_VETOR_ACESSO, "vetor_acesso", yylineno, idNode, $3, NULL);
            $$ = createNode(NODE_COMANDO, "=", yylineno, vetorNode, $6, NULL);
        }
        | RETORNE Expr ';' { $$ = createNode(NODE_RETORNE, "retorne", yylineno, $2, NULL, NULL); }
        | SE '(' Expr ')' ENTAO ListaComando FIMSE { $$ = createNode(NODE_COMANDO, "se", yylineno, $3, $6, NULL); }
        | SE '(' Expr ')' ENTAO ListaComando SENAO ListaComando FIMSE { $$ = createNode(NODE_COMANDO, "se", yylineno, $3, $6, $8); }
        | ENQUANTO '(' Expr ')' Comando { $$ = createNode(NODE_COMANDO, "enquanto", yylineno, $3, $5, NULL); }
        | LEIA IDENTIFICADOR ';' {
            AST* idNode = createNode(NODE_IDENTIFICADOR, $2, yylineno, NULL, NULL, NULL);
            $$ = createNode(NODE_COMANDO, "leia", yylineno, idNode, NULL, NULL);
        }
        | LEIA IDENTIFICADOR '[' Expr ']' ';' {
            AST* idNode = createNode(NODE_IDENTIFICADOR, $2, yylineno, NULL, NULL, NULL);
            AST* vetorNode = createNode(NODE_VETOR_ACESSO, "vetor_acesso", yylineno, idNode, $4, NULL);
            $$ = createNode(NODE_COMANDO, "leia", yylineno, vetorNode, NULL, NULL);
        }
        | ESCREVA Expr ';' { $$ = createNode(NODE_COMANDO, "escreva", yylineno, $2, NULL, NULL); }
        | IDENTIFICADOR '(' ListaArgs ')' ';' {
             AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
             $$ = createNode(NODE_CHAMADA_FUNC, "chamada", yylineno, id, $3, NULL);
        }
        | NOVALINHA ';'    { $$ = createNode(NODE_NOVALINHA, "novalinha", yylineno, NULL, NULL, NULL); }
        | Bloco            { $$ = $1; }
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

PrimExpr : IDENTIFICADOR { $$ = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL); }
         | IDENTIFICADOR '[' Expr ']' {
             AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
             $$ = createNode(NODE_VETOR_ACESSO, "vetor_acesso", yylineno, id, $3, NULL);
         }
         | IDENTIFICADOR '(' ListaArgs ')' {
             AST* id = createNode(NODE_IDENTIFICADOR, $1, yylineno, NULL, NULL, NULL);
             $$ = createNode(NODE_CHAMADA_FUNC, "chamada", yylineno, id, $3, NULL);
         }
         | INTCONST        { $$ = createNode(NODE_INTCONST, $1, yylineno, NULL, NULL, NULL); }
         | CADEIACARACTERES{ $$ = createNode(NODE_CARCONST, $1, yylineno, NULL, NULL, NULL); }
         | CARCONST        { $$ = createNode(NODE_CARCONST, $1, yylineno, NULL, NULL, NULL); }
         | '(' Expr ')'    { $$ = $2; }
         ;

ListaArgs : ListaArgsTail { $$ = $1; }
          | /* vazio */   { $$ = NULL; }
          ;

ListaArgsTail : Expr { $$ = createNode(NODE_PARAM, "arg", yylineno, $1, NULL, NULL); }
              | Expr ',' ListaArgsTail {
                  AST* p = createNode(NODE_PARAM, "arg", yylineno, $1, NULL, NULL);
                  $$ = createNode(NODE_PARAM, "lista_arg", yylineno, p, $3, NULL);
              }
              ;


%%

void yyerror(const char *s) {
    if (strstr(s, "expecting '{'")) {
        printf("ERRO: ABRE CHAVES ESPERADO %d\n", yylineno - 1);
    } else if (strstr(s, "unexpected ':', expecting '='")) {
        printf("ERRO: DECLARACAO NAO ENVOLVIDA EM CHAVES %d\n", yylineno);
    } else if (strstr(s, "unexpected SENAO")) {
        printf("ERRO: FECHA CHAVES ESPERADO %d\n", yylineno - 1);
    } else if (strstr(s, "unexpected '}'") && !strstr(s, "expecting")) {
        printf("ERRO: FIMSE FALTANDO %d\n", yylineno - 1);
    } else if (strstr(s, "expecting ';'")) {
        printf("ERRO: PONTO E VIRGULA FALTANDO %d\n", yylineno - 1);
    } else {
        printf("ERRO: %s %d\n", s, yylineno);
    }
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

    if (ast_raiz != NULL) {
        Stack tabelaDeSimbolos;
        initStack(&tabelaDeSimbolos);

        printf("\n>>> Iniciando O CAMINHANTE SEMANTICO...\n");
        checkSemantics(ast_raiz, &tabelaDeSimbolos);

        printf("\n>>> SUCESSO! O G-V1 compilou '%s' e nao encontrou \nnenhum erro Semantico, Sintatico ou Lexico na fita!\n", argv[1]);

       
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
