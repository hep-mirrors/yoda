SUBDIRS = yoda
EXTRA_DIST = template.py

ENV = CC=$(CC) CXX=$(CXX)

if ENABLE_PYEXT

all-local:
	$(ENV) $(PYTHON) setup.py install --install-lib=build/

install-exec-local:
	$(ENV) $(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix)

uninstall-local:
	rm -rf $(DESTDIR)$(YODA_PYTHONPATH)/yoda

clean-local:
	$(ENV) $(PYTHON) setup.py clean --all
	@rm -f $(builddir)/*.pyc
	@rm -rf $(builddir)/build
	@rm -rf dist

distclean-local:
	@rm -rf yoda.egg-info

endif
