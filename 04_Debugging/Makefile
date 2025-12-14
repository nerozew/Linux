CC = cc
CFLAGS = -O0 -g
TARGET = range

$(TARGET): range.c
	$(CC) $(CFLAGS) range.c -o $(TARGET)

test:
	gdb --batch --quiet -x test1.gdb ./range | grep '^@@@'
	gdb --batch --quiet -x test2.gdb ./range | grep '^@@@'

clean:
	rm -f $(TARGET)

.PHONY: test clean
