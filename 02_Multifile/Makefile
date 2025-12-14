.PHONY: all clean test

# 编译器和标志
CC = cc
# 为了方便，我们让所有 .o 都加上 -fPIC，这样既可以用于动态库也可以用于静态库
CFLAGS = -Wall -fPIC

# 定义生成目标
LIBS = liboutput_static.a liboutput.so
PROGS = prog prog-a prog-so
# 定义测试生成的临时文件
TEST_OUTS = test_prog.out test_prog_a.out test_prog_so.out

# 默认目标：编译所有库和程序
all: $(LIBS) $(PROGS)

# 通用规则：从 .c 生成 .o
%.o: %.c outlib.h
	$(CC) $(CFLAGS) -c $< -o $@

# 1. 静态库 liboutput_static.a
liboutput_static.a: fun.o const.o
	ar -rcs $@ $^

# 2. 动态库 liboutput.so
liboutput.so: fun.o const.o
	$(CC) -shared -o $@ $^

# 3. 三个二进制文件

# 3.1 prog: 直接从 .o 文件链接
prog: prog.o fun.o const.o
	$(CC) $(CFLAGS) -o $@ $^

# 3.2 prog-a: 链接静态库
# 注意：库文件通常放在源文件后面
prog-a: prog.o liboutput_static.a
	$(CC) $(CFLAGS) -o $@ prog.o -L. -loutput_static

# 3.3 prog-so: 链接动态库
# 需要指定运行时路径 -Wl,-rpath,. 或者运行时候指定 LD_LIBRARY_PATH
# 题目暗示运行时候处理，这里只管链接
prog-so: prog.o liboutput.so
	$(CC) $(CFLAGS) -o $@ prog.o -L. -loutput

# 4. 测试目标
test: $(PROGS)
	@echo "--- Testing prog ---"
	./prog > test_prog.out 2>&1 || true
	./prog arg1 >> test_prog.out 2>&1
	./prog arg1 arg2 arg3 >> test_prog.out 2>&1

	@echo "--- Testing prog-a ---"
	./prog-a > test_prog_a.out 2>&1 || true
	./prog-a arg1 >> test_prog_a.out 2>&1
	./prog-a arg1 arg2 arg3 >> test_prog_a.out 2>&1

	@echo "--- Testing prog-so ---"
	LD_LIBRARY_PATH=`pwd` ./prog-so > test_prog_so.out 2>&1 || true
	LD_LIBRARY_PATH=`pwd` ./prog-so arg1 >> test_prog_so.out 2>&1
	LD_LIBRARY_PATH=`pwd` ./prog-so arg1 arg2 arg3 >> test_prog_so.out 2>&1

	@echo "--- Comparing results ---"
	# 比较 prog 和 prog-a 的输出
	# 注意：因为 argv[0] (程序名) 不同，直接 cmp 会失败
	# 题目提示 argv[0] 不同，所以 cmp 可能需要忽略差异或者我们需要 sed 处理
	# 但题目要求 "比较相应启动的输出...如果不相同则返回非零"
	# 这里有个陷阱：usage() 函数打印了 prog name。
	# 我们用 sed 把程序名替换成统一的字符串再比较
	
	sed 's/prog[-a-so]*//g' test_prog.out > test_prog.sed
	sed 's/prog[-a-so]*//g' test_prog_a.out > test_prog_a.sed
	sed 's/prog[-a-so]*//g' test_prog_so.out > test_prog_so.sed
	
	cmp test_prog.sed test_prog_a.sed
	cmp test_prog.sed test_prog_so.sed
	
	@echo "--- ALL TESTS PASSED ---"

# 清理目标
clean:
	rm -f *.o $(LIBS) $(PROGS) $(TEST_OUTS) *.sed
