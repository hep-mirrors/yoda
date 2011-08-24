check_PROGRAMS = \
  testweights	\
  testhisto1Da testhisto1Db \
  testprofile1Da \
  testhisto2D  \
  testhisto2Da  \
  testhisto2De \
  testpoint3D  \
  testscatter3D\
  testindexedset testsortedvector\
  testhisto1Dconstructor \
  testhisto1Dfill\
  testhisto1Dmodify

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_builddir)/src -lYODA

testweights_SOURCES = TestWeights.cc
testhisto1Da_SOURCES = TestHisto1Da.cc
testhisto1Db_SOURCES = TestHisto1Db.cc
testprofile1Da_SOURCES = TestProfile1Da.cc
testindexedset_SOURCES = TestIndexedSet.cc
testsortedvector_SOURCES = TestSortedVector.cc
testhisto2D_SOURCES = TestHisto2D.cc
testhisto2Da_SOURCES = TestHisto2Da.cc
testhisto2De_SOURCES = TestHisto2Derase.cc
testpoint3D_SOURCES = TestPoint3D.cc
testscatter3D_SOURCES = TestScatter3D.cc
testhisto1Dconstructor_SOURCES = Histo1D/Constructors.cc
testhisto1Dfill_SOURCES = Histo1D/Fill.cc
testhisto1Dmodify_SOURCES = Histo1D/Modify.cc

# TODO: Search paths are still not complete for the Python tests: need to add the lib.linux-i686-2.7 part to PYTHONPATH
TESTS_ENVIRONMENT = \
  LD_LIBRARY_PATH=$(top_builddir)/src:$(top_builddir)/src/.libs:$(LD_LIBRARY_PATH) \
  PYTHONPATH=$(top_builddir)/pyext/build:$(PYTHONPATH) \
  PATH=$(top_builddir)/bin:$(PATH)

TESTS = \
  testweights \
  testhisto1Da testhisto1Db \
  testprofile1Da \
  testhisto2D    \
  testhisto2Da    \
  testhisto2De \
  testpoint3D    \
  testscatter3D  \
  testindexedset testsortedvector \
  testhisto1Dconstructor \
  testhisto1Dfill \
  testhisto1Dmodify \
  test-yoda-1.py test-yoda-2.py
