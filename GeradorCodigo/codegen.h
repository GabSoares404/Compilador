#ifndef CODEGEN_H
#define CODEGEN_H

#include "../AST/ast.h"
#include "../SymbolTable/symbol_table.h"
#include <stdio.h>

// Função principal que o Bison chamará se o Semântico der sucesso
void generateCode(AST* root, Stack* scopes, const char* out_filename);

void cgenEx(AST* expr, FILE* out, Stack* scopes);
void cgenCmd(AST* cmd, FILE* out, Stack* scopes);
void cgenIf(AST* left, AST* right, AST* extra, FILE* out, Stack* scopes);
void cgenWhile(AST* left, AST* right, FILE* out, Stack* scopes);

#endif
