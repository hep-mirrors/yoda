lib_LTLIBRARIES = libYODA.la

libYODA_la_SOURCES = \
    AnalysisObject.cc \
    Dbn1D.cc \
    Bin.cc \
    HistoBin.cc \
    Histo1D.cc \
    ProfileBin.cc \
    Profile1D.cc \
    Writer.cc \
    WriterAIDA.cc \
    WriterYODA.cc
# Error.cc
# Point.cc
# Scatter.cc

libYODA_la_CPPFLAGS = -I$(srcdir)/yaml-cpp $(AM_CPPFLAGS)

## NB. Using $(top_builddir)/src rather than $(builddir) because of SL4 automake bug
## TODO: Remove this workaround when SL4 is unsupported
if WITH_YAML_CPP
libYODA_la_LDFLAGS = $(AM_LDFLAGS) -L$(YAML_CPPLIBPATH)
libYODA_la_LIBADD = -lyaml-cpp
else
SUBDIRS = yaml-cpp
libYODA_la_LIBADD = $(top_builddir)/src/yaml-cpp/libyaml-cpp.la
endif
