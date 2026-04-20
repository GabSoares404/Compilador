#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../AST/ast.h"
#include "../SymbolTable/symbol_table.h"

// Função Central do nosso Analisador Semântico. 
// Ela receberá o NÓ RAIZ (Programa Principal) gerado no Final do Bison, e a Pilha inicial de Escopos que construímos ontem!
void checkSemantics(AST* node, Stack* scopes);
int inferirTipo(AST* expr, Stack* scopes);
void extrairVariaveis(AST* nodeIds, Stack* scopes, int tipoHerdado);

#endif
