CC = gcc
CFLAGS = -Wall -O2

all: esub

esub: esub.c
	$(CC) $(CFLAGS) esub.c -o esub

test: esub
	@echo "--- Running Tests ---"
	@./esub "world" "universe" "Hello world" > out.txt
	@echo "Hello world" | sed -E 's/world/universe/' > expected.txt
	@diff out.txt expected.txt && echo "[PASS] Simple replacement"
	@rm -f out.txt expected.txt
	@echo "--- Tests Finished ---"

clean:
	rm -f esub out.txt expected.txt
