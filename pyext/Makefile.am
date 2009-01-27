EXTRA_DIST = yoda.i ez_setup.py

## Bundling SWIG-built sources with the tarball for SL4 compat
EXTRA_DIST += yoda_wrap.cc yoda.py
yoda_wrap.cc yoda.py: yoda.i
	swig -c++ -python -I$(top_srcdir)/include -o yoda_wrap.cc $<


if ENABLE_PYEXT

all-local: yoda_wrap.cc
	$(PYTHON) setup.py build

install-exec-local:
	$(PYTHON) setup.py install --prefix=$(prefix)

uninstall-local:
	rm -rf $(libdir)/python*/site-packages/yoda*

clean-local:
	$(PYTHON) setup.py clean --all
	@rm -f $(top_builddir)/*.pyc

distclean-local:
	@rm -rf yoda.egg-info
	@rm -rf dist
	@rm -f yoda.py
	@rm -f yoda_wrap.cc

endif
