if ENABLE_PYEXT

SUBDIRS = .
CYTHONFLAGS = @CYTHONFLAGS@

all-local: yoda.cc
	$(PYTHON) setup.py build
	$(PYTHON) setup.py install --install-lib=build/
yoda.cc:
	$(PYTHON) -m cython --cplus yoda.pyx

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
