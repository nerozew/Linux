AC_PREREQ([2.69])
AC_INIT([rhasher], [0.1], [your-email@example.com])
AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

# 检查编译器
AC_PROG_CC

# 1. 检查必需的库: librhash
# 检查 rhash_library_init 函数是否存在于 librhash 中
AC_CHECK_LIB([rhash], [rhash_library_init], [], [AC_MSG_ERROR([librhash is required but not found.])])
# 检查头文件
AC_CHECK_HEADERS([rhash.h], [], [AC_MSG_ERROR([rhash.h is required.])])

# 2. 检查可选的库: libreadline
# 添加 --disable-readline 选项
AC_ARG_ENABLE([readline],
    AS_HELP_STRING([--disable-readline], [Disable readline support]),
    [enable_readline=$enableval],
    [enable_readline=yes])

if test "x$enable_readline" = "xyes"; then
    AC_CHECK_LIB([readline], [readline], 
        [
            AC_DEFINE([HAVE_LIBREADLINE], [1], [Define if you have libreadline])
            LIBS="-lreadline $LIBS"
        ], 
        [AC_MSG_WARN([libreadline not found, falling back to getline])])
fi

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
