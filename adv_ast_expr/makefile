TARGET=expr_printer
EXPR_PARSER_SRC=expr_parser.cpp
EXPR_LEXER_SRC=expr_lexer.cpp
C_SRCFILES=$(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC) ast.cpp main.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
.PHONY : clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES)

%.o: %.cpp tokens.h
	g++ -c -o $@ $<

$(EXPR_LEXER_SRC): expr.l
	flex -o $@ $^

$(EXPR_PARSER_SRC): expr.y ast.h ast.cpp
	bison --defines=tokens.h -o $@ $<

run: $(TARGET)
	./$(TARGET) input.txt

clean:
	rm -f $(EXPR_LEXER_SRC) $(EXPR_PARSER_SRC)
	rm -f *.o
	rm -f ./$(TARGET)
	rm -f tokens.h
