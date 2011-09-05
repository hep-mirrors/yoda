if ENABLE_PYEXT

SUBDIRS = yoda .
CYTHONFLAGS = @CYTHONFLAGS@

all-local: yoda
	$(PYTHON) setup.py build
	$(PYTHON) setup.py install --install-lib=build/

install-exec-local:
	$(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix)

uninstall-local:
	rm -rf $(DESTDIR)$(YODA_PYTHONPATH)/yoda

clean-local:
	$(PYTHON) setup.py clean --all
	@rm -f $(top_builddir)/*.pyc
	@rm -rf $(builddir)/build
	@rm -rf dist

distclean-local:
	@rm -rf yoda.egg-info

endif
