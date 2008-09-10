if ENABLE_PYEXT

BUILT_SOURCES = yoda_wrap.cc
SWIG_SOURCES  = yoda.i
python_PYTHON = yoda.py

pyexec_LTLIBRARIES = _yoda.la
_yoda_la_SOURCES = yoda_wrap.cc $(SWIG_SOURCES)
_yoda_la_CPPFLAGS = $(AM_CPPFLAGS) $(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include
_yoda_la_CXXFLAGS = $(AM_CXXFLAGS) -Wno-long-long -fno-strict-aliasing
_yoda_la_LDFLAGS = -module -L$(top_builddir)/src
_yoda_la_LIBADD = -lYODA

incdir = $(top_srcdir)/include
yoda_wrap.cc : $(SWIG_SOURCES) $(incdir)/YODA/Histo1D.h $(incdir)/YODA/Profile1D.h
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(incdir) -o yoda_wrap.cc $(SWIG_SOURCES)

EXTRA_DIST = PyTest.py
TESTS = PyTest.py
TESTS_ENVIRONMENT = \
  PYTHONPATH=$(PYTHONPATH):$(top_builddir)/pyext:$(top_builddir)/pyext/.libs \
  python

else

EXTRA_DIST = yoda.i PyTest.py

endif
