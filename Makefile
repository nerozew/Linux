CC =	 gcc
CFLAGS =	 -Wall -Wextra -std=c11
LIBS =	 -lncursesw

Show: Show.c
	gcc $(CFLAGS) -o Show Show.c $(LIBS)

clean:
	rm -f Show *~

.PHONY: clean
