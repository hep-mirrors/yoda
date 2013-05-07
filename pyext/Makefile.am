SUBDIRS = yoda
EXTRA_DIST = template.py

if ENABLE_PYEXT
PYEXT_ENV = CC="$(CC)" CXX="$(CXX)"

all-local:
	$(PYEXT_ENV) $(PYTHON) setup.py install --install-lib=build/

install-exec-local:
	$(PYEXT_ENV) $(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix)

uninstall-local:
	rm -rf $(DESTDIR)$(YODA_PYTHONPATH)/yoda

clean-local:
	$(PYEXT_ENV) $(PYTHON) setup.py clean --all
	@rm -f $(builddir)/*.pyc
	@rm -rf $(builddir)/build
	@rm -rf dist

distclean-local:
	@rm -rf yoda.egg-info

endif
