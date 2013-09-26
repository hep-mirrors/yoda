SUBDIRS = yoda
EXTRA_DIST = template.py

if WITH_CYTHON
lhapdf.cpp: lhapdf.pyx clhapdf.pxd
	cython lhapdf.pyx --cplus
else
lhapdf.cpp:
	@echo "Not (re)generating lhapdf.cpp since Cython is not installed"
endif

if ENABLE_PYEXT
PYEXT_ENV = CC="$(CC)" CXX="$(CXX)" ARCHFLAGS=""
if ENABLE_ROOT
  PYEXT_ENV += BUILD_ROOTCOMPAT="yes"
endif

## always force setup.py, it's not good at guessing what needs to rebuild
all-local:
	$(PYEXT_ENV) $(PYTHON) setup.py install --install-lib=build/ --force

install-exec-local:
	$(PYEXT_ENV) $(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix) --force

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
