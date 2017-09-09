TARGET = tinypython
CXXFLAGS=-std=c++11

all: ${TARGET}

${TARGET}: ast.o ${TARGET}_parser.o ${TARGET}_lexer.o main.o
	g++ -o $@ $^

ast.o: ast.cpp ast.h
	g++ $(CXXFLAGS) -c -o $@ $<

main.o: main.cpp ast.h
	g++ $(CXXFLAGS) -c -o $@ $<

${TARGET}_lexer.o: ${TARGET}_lexer.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

${TARGET}_lexer.cpp: ${TARGET}.l ast.h
	flex -i -o $@ $<

${TARGET}_parser.cpp: ${TARGET}.y ast.h
	bison -Werror --report=all --defines=tokens.h -o $@ $<

${TARGET}_parse.o: ${TARGET}_parser.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

run1: ${TARGET}
	./${TARGET} ../samples/HelloWorld.py -s

run2: ${TARGET}
	./${TARGET} ../samples/ArithOperators.py -s

run3: ${TARGET}
	./${TARGET} ../samples/ForTest.py -s

run4: ${TARGET}
	./${TARGET} ../samples/IfTest.py -s

run5: ${TARGET}
	./${TARGET} ../samples/RelOperators.py -s

run6: ${TARGET}
	./${TARGET} ../samples/WhileTest.py -s

clean:
	rm -f *.o
	rm -f ${TARGET}_lexer.cpp
	rm -f ${TARGET}_parser.cpp
	rm -f tokens.h
	rm -f ${TARGET}
	rm -f ${TARGET}_parser.output
