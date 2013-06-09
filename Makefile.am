# Makefile.am for Idesk 0.7.2 

SUBDIRS = src
MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in stamp-h.in src/default.h
ACLOCAL_AMFLAGS = -I .
EXTRA_DIST = \
acinclude.m4 \
AUTHORS \
COPYING \
INSTALL \
README \
TODO \
ChangeLog \
NEWS 

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/examples; then \
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir); \
	$(INSTALL_DATA) $(srcdir)/README $(DESTDIR)$(pkgdatadir); \
	  for example in $(srcdir)/examples/*; do \
	    if test -f $$example; then \
	      $(INSTALL_DATA) $$example $(DESTDIR)$(pkgdatadir); \
 	    fi \
	  done \
	fi 

dist-hook:
	  if test -d examples; then \
	  mkdir $(distdir); \
	    for d in examples/*; do \
	       if test -f $$d; then \
	         cp -ar $$d $(distdir); \
	       fi \
	    done \
	 fi

uninstall-local:
	rm -rf $(pkgdatadir)/*
	rmdir $(pkgdatadir)
distclean-local:
	rm -f *\~

source-doc:
	doxygen Doxyfile
