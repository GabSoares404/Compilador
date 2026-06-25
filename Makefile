# Variáveis de atalho 
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall

# A regra padrão apontará para criar o 'g-v1'
all: g-v2

# REGRA 1: Para criar 'g-v1', eu exijo ter 'lexer.c', 'parser.c', 'AST/ast.c', 'SymbolTable/symbol_table.c', 'Semantic/semantic.c' e 'codegen.c'.
g-v2: Lexer/lexer.c Parser/parser.c AST/ast.c SymbolTable/symbol_table.c Semantic/semantic.c GeradorCodigo/codegen.c
	$(CC) $(CFLAGS) -o g-v2 Parser/parser.c Lexer/lexer.c AST/ast.c SymbolTable/symbol_table.c Semantic/semantic.c GeradorCodigo/codegen.c

# REGRA 2: Para criar 'parser.c' e também seu parceiro 'parser.h' (usando o sinal -d), uso 'parser.y'
Parser/parser.c Parser/parser.h: Parser/parser.y
	$(BISON) -d -o Parser/parser.c Parser/parser.y

# REGRA 3: Para criar 'lexer.c', uso o 'lexer.l'. (Ele exige o parser.h gerado acima para puxar os tokens!)
Lexer/lexer.c: Lexer/lexer.l Parser/parser.h
	$(FLEX) -o Lexer/lexer.c Lexer/lexer.l

# REGRA 4: Limpeza! Se você rodar 'make clean', ele deletará as sobras pra começar fresco.
clean:
	rm -f g-v2 Lexer/lexer.c Parser/parser.c Parser/parser.h
