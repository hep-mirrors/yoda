SUBDIRS = yoda
EXTRA_DIST = template.py

if ENABLE_PYEXT

all-local:
	CC=$(CC) CXX=$(CXX) $(PYTHON) setup.py install --install-lib=build/

install-exec-local:
	CC=$(CC) CXX=$(CXX) $(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix)

uninstall-local:
	rm -rf $(DESTDIR)$(YODA_PYTHONPATH)/yoda

clean-local:
	CC=$(CC) CXX=$(CXX) $(PYTHON) setup.py clean --all
	@rm -f $(top_builddir)/*.pyc
	@rm -rf $(builddir)/build
	@rm -rf dist

distclean-local:
	@rm -rf yoda.egg-info

endif
