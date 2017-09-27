SUBDIRS = yoda

EXTRA_DIST = yoda1/__init__.py

if ENABLE_PYEXT
PYEXT_ENV = CC="$(CXX)" CXX="$(CXX)" CXXFLAGS="$(PYEXT_CXXFLAGS)" ARCHFLAGS=""

if ENABLE_ROOT
  PYEXT_ENV += BUILD_ROOTCOMPAT="yes"
endif

## Always force setup.py, it's not good at guessing what needs to rebuild
all-local:
	cd $(srcdir); \
	$(PYEXT_ENV) YODA_LOCAL=1 $(PYTHON) \
	  $(abs_builddir)/setup.py build \
	    --build-platlib $(abs_builddir)/build/lib \
	    --build-temp $(abs_builddir)/build/tmp \
        --build-base $(abs_builddir)/build \
        --verbose \
        --force

install-exec-local:
	cd $(srcdir); \
	$(PYEXT_ENV) $(PYTHON) \
	  $(abs_builddir)/setup.py install \
	    --prefix $(DESTDIR)$(prefix) \
	    --verbose \
	    --force 

#	$(PYEXT_ENV) $(PYTHON) \
#	  $(abs_builddir)/setup.py install_lib \
#	    --build-dir $(abs_builddir)/build \
#	    --install-dir $(DESTDIR)$(prefix)/lib/python2.7/site-packages/lib \
#	    --verbose \
#	    --force 


uninstall-local:
	cat $(DESTDIR)$(pkgpythondir)/install_files.txt | xargs rm -f
	rm -rf $(DESTDIR)$(pkgpythondir)

clean-local:
	rm -rf $(builddir)/build
##	rm -f $(builddir)/*.pyc
##	rm -rf dist

#distclean-local:
#	rm -f $(builddir)/yoda.egg-info
#	rm -rf $(builddir)/yoda
#	rm -rf $(builddir)/yoda1

endif
