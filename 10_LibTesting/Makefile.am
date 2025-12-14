## Process this file with automake to produce Makefile.in

# 1. 设置 C 语言标准和通用的编译选项
AM_CFLAGS = -Wall -Wextra -std=c99 -g

# 2. 定义我们要构建的库 (Libtool 库)
lib_LTLIBRARIES = libbuf.la

# 库的源代码文件
libbuf_la_SOURCES = buf.c buf.h

# 库特定的编译选项
libbuf_la_CFLAGS = $(AM_CFLAGS)

# 库的链接选项
libbuf_la_LDFLAGS = -version-info 0:0:0

# 3. 定义测试套件
TESTS = check_buf
check_PROGRAMS = check_buf
check_buf_SOURCES = tests.c
check_buf_CFLAGS = @CHECK_CFLAGS@ $(AM_CFLAGS) -I.
check_buf_LDADD = libbuf.la @CHECK_LIBS@

# 4. 自定义 gcov 覆盖率报告目标
# 注意：这里移除了规则内部带缩进的注释，避免 unportable 错误
gcov_report: check
	@echo ""
	@echo "===== Generating Gcov Report ====="
	@echo ""
	gcov -o .libs/libbuf_la-buf.o buf.c
	@echo ""
	@echo "===== Coverage Summary ====="
	@echo ""
	@grep "Lines executed" buf.c.gcov -B 1 || echo "No coverage data found"
	@echo ""
	@echo "Report generated in 'buf.c.gcov'."

# 清理规则
CLEANFILES = *.gcda *.gcno *.gcov

