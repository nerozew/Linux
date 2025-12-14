CC = gcc
CFLAGS = -O0 -g

all: range

range: range.c
	$(CC) $(CFLAGS) range.c -o range

test: range
	@echo "Running Test 1..."
	# 运行 gdb 脚本，过滤出我们要的调试信息（以 @@@ 开头的行）
	gdb -q -x test1.gdb ./range | grep "@@@" > result1.txt
	@echo "Test 1 Done. Output saved to result1.txt"
	
	@echo "Running Test 2..."
	gdb -q -x test2.gdb ./range | grep "@@@" > result2.txt
	@echo "Test 2 Done. Output saved to result2.txt"
	
	@echo "Displaying results:"
	@cat result1.txt
	@echo "----------------"
	@cat result2.txt

clean:
	rm -f range result1.txt result2.txt
