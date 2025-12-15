SUBDIRS = src po

if HAVE_DOXYGEN
doc: Doxyfile
	$(DOXYGEN) Doxyfile
	
# 自动生成文档的目标
all-local: doc
endif

clean-local:
	rm -rf doc

ACLOCAL_AMFLAGS = -I m4
