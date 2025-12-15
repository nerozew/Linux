lib_LTLIBRARIES = libgreeting.la
libgreeting_la_SOURCES = libgreeting.c libgreeting.h
libgreeting_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

bin_PROGRAMS = greeting
greeting_SOURCES = main.c
greeting_LDADD = libgreeting.la
greeting_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
