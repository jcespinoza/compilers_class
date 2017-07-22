TARGET=sample1
.PHONY : clean

$(TARGET): $(TARGET).c tokens.h
	gcc -o $@ $<

sample1.c: sample1.l
	flex -o $@ $^

run: $(TARGET)
	./$(TARGET) < input.txt

clean:
	rm -f sample1.c
	rm -f sample1
