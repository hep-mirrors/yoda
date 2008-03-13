ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src include 
#pyext data bin

## Deal with the Doxygen stuff
if WITH_DOXYGEN
.PHONY = dox
dox:
	$(DOXYGEN) $(top_srcdir)/Doxyfile
all-local: dox
	@true
install-data-local:
	$(install_sh_DATA) dox/html/* $(DESTDIR)$(pkgdatadir)/doc/html
mostlyclean-local:
	rm -rf $(top_builddir)/ccwrap/dox
uninstall-local:
	rm -rf $(DESTDIR)$(pkgdatadir)/doc; \
	test ! -d $(DESTDIR)$(pkgdatadir) || rmdir --ignore-fail-on-non-empty $(DESTDIR)$(pkgdatadir)
endif
