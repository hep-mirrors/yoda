check_PROGRAMS = \
  testweights	\
  testhisto1Da testhisto1Db \
  testprofile1Da \
  testhisto2De \
  testpoint3D  \
  testscatter3D\
  testindexedset testsortedvector\
  testhisto1Dcreate \
  testhisto1Dfill\
  testhisto1Dmodify \
  testprofile1Dcreate\
  testprofile1Dfill \
  testprofile1Dmodify\
  testhisto2Dcreate \
  testhisto2Dfill\
  testhisto2Dmodify\
  testscatter2Dcreate\
  testscatter2Dmodify\
  testscatter3Dcreate\
  testscatter3Dmodify

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_builddir)/src -lYODA

testweights_SOURCES = TestWeights.cc
testhisto1Da_SOURCES = TestHisto1Da.cc
testhisto1Db_SOURCES = TestHisto1Db.cc
testprofile1Da_SOURCES = TestProfile1Da.cc
testindexedset_SOURCES = TestIndexedSet.cc
testsortedvector_SOURCES = TestSortedVector.cc
testhisto2De_SOURCES = TestHisto2Derase.cc
testpoint3D_SOURCES = TestPoint3D.cc
testscatter3D_SOURCES = TestScatter3D.cc
testhisto1Dcreate_SOURCES = Histo1D/H1DCreate.cc
testhisto1Dfill_SOURCES = Histo1D/H1DFill.cc
testhisto1Dmodify_SOURCES = Histo1D/H1DModify.cc
testprofile1Dcreate_SOURCES = Profile1D/P1DCreate.cc
testprofile1Dfill_SOURCES = Profile1D/P1DFill.cc
testprofile1Dmodify_SOURCES = Profile1D/P1DModify.cc
testhisto2Dcreate_SOURCES = Histo2D/H2DCreate.cc
testhisto2Dfill_SOURCES = Histo2D/H2DFill.cc
testhisto2Dmodify_SOURCES = Histo2D/H2DModify.cc
testscatter2Dcreate_SOURCES = Scatter2D/S2DCreate.cc
testscatter2Dmodify_SOURCES = Scatter2D/S2DModify.cc
testscatter3Dcreate_SOURCES = Scatter3D/S3DCreate.cc
testscatter3Dmodify_SOURCES = Scatter3D/S3DModify.cc


# TODO: Search paths are still not complete for the Python tests: need to add the lib.linux-i686-2.7 part to PYTHONPATH
TESTS_ENVIRONMENT = \
  LD_LIBRARY_PATH=$(top_builddir)/src:$(top_builddir)/src/.libs:$(LD_LIBRARY_PATH) \
  PYTHONPATH=$(top_builddir)/pyext/build:$(PYTHONPATH) \
  PATH=$(top_builddir)/bin:$(PATH)

TESTS = \
  testweights \
  testhisto1Da testhisto1Db \
  testprofile1Da \
  testhisto2De \
  testpoint3D    \
  testscatter3D  \
  testindexedset testsortedvector \
  testhisto1Dcreate \
  testhisto1Dfill \
  testhisto1Dmodify \
  testprofile1Dcreate \
  testprofile1Dfill \
  testprofile1Dmodify\
  testhisto2Dcreate \
  testhisto2Dfill\
  testhisto2Dmodify\
  testscatter2Dcreate\
  testscatter2Dmodify\
  testscatter3Dcreate\
  testscatter3Dmodify\
  test-yoda-1.py test-yoda-2.py
