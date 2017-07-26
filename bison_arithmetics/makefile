TARGET=sample2
C_SRCFILES=main.c expr_parser.c expr_lexer.c
OBJ_FILES=${C_SRCFILES:.c=.o}
.PHONY : clean

$(TARGET): $(OBJ_FILES)
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<

expr_lexer.c: expr.l
	flex -o $@ $^

expr_parser.c: expr.y
	bison --defines=tokens.h -o $@ $<

run: $(TARGET)
	./$(TARGET) < input.txt

clean:
	rm -f expr_lexer.c expr_parser.c
	rm -f *.o
	rm -f ./$(TARGET)
