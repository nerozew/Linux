CC = gcc
CFLAGS = -Wall -O2
LIB_CFLAGS = -shared -fPIC

all: move protect.so

move: move.c
	$(CC) $(CFLAGS) move.c -o move

protect.so: protect.c
	$(CC) $(LIB_CFLAGS) protect.c -o protect.so -ldl

test: all
	chmod +x test_script.sh
	./test_script.sh

clean:
	rm -f move protect.so in.txt out.txt in_PROTECT.txt out_PROTECT.txt
