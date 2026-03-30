# Nome do executável final
TARGET = g-v1

# Arquivos gerados pelas ferramentas
LEX_OUT = lex.yy.c
YACC_OUT = g-v1.tab.c
YACC_HDR = g-v1.tab.h

all: $(TARGET)

$(TARGET): $(LEX_OUT) $(YACC_OUT)
	gcc -o $(TARGET) $(LEX_OUT) $(YACC_OUT) -lfl

$(LEX_OUT): g-v1.l $(YACC_HDR)
	flex g-v1.l

$(YACC_OUT) $(YACC_HDR): g-v1.y
	bison -d g-v1.y

clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT) $(YACC_HDR)