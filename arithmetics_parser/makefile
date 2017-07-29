TARGET=sample1
C_SRCFILES=$(TARGET).c main.c parser.c
OBJ_FILES=${C_SRCFILES:.c=.o}
.PHONY : clean

$(TARGET): $(OBJ_FILES)
	gcc -o $@ $^

sample1.c: sample1.l
	flex -o $@ $^

%.o: %.c
	gcc -c -o $@ $<
run: $(TARGET)
	./$(TARGET) < input.txt

clean:
	rm -f sample1.c
	rm -f sample1
	rm -f *.o
