AC_INIT([Greeting App], [1.0], [bug@example.com], [greeting])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# 初始化 Libtool 以支持动态库
LT_INIT
AC_PROG_CC
AM_PROG_AR   dnl
# 国际化支持
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19])

# Doxygen 检查
AC_CHECK_PROGS([DOXYGEN], [doxygen])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
    po/Makefile.in
    Doxyfile
])
AC_OUTPUT
