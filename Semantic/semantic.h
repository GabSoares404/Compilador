#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../AST/ast.h"
#include "../SymbolTable/symbol_table.h"

void checkSemantics(AST* node, Stack* scopes);

int inferirTipo(AST* expr, Stack* scopes);

void extrairVariaveis(AST* nodeIds, Stack* scopes, int tipoHerdado);

#endif
