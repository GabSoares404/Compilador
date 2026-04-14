# Variáveis de atalho 
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall

# A regra padrão apontará para criar o 'g-v1'
all: g-v1

# REGRA 1: Para criar 'g-v1', eu exijo ter 'lexer.c', 'parser.c', 'ast.c' e 'symbol_table.c'.
g-v1: lexer.c parser.c ast.c symbol_table.c
	$(CC) $(CFLAGS) -o g-v1 parser.c lexer.c ast.c symbol_table.c

# REGRA 2: Para criar 'parser.c' e também seu parceiro 'parser.h' (usando o sinal -d), uso 'parser.y'
parser.c parser.h: parser.y
	$(BISON) -d -o parser.c parser.y

# REGRA 3: Para criar 'lexer.c', uso o 'lexer.l'. (Ele exige o parser.h gerado acima para puxar os tokens!)
lexer.c: lexer.l parser.h
	$(FLEX) -o lexer.c lexer.l

# REGRA 4: Limpeza! Se você rodar 'make clean', ele deletará as sobras pra começar fresco.
clean:
	rm -f g-v1 lexer.c parser.c parser.h
