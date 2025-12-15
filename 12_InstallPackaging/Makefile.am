SUBDIRS = src po tests
ACLOCAL_AMFLAGS = -I m4

man_MANS = man/greeting.1

if HAVE_DOXYGEN
doc: Doxyfile
	$(DOXYGEN) Doxyfile
all-local: doc
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(docdir)/html
	cp -r doc/html/* $(DESTDIR)$(docdir)/html
endif

