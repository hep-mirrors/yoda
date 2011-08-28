SUBDIRS = tinyxml .

lib_LTLIBRARIES = libYODA.la

libYODA_la_SOURCES = \
    Dbn1D.cc \
    Histo1D.cc \
    Histo2D.cc \
    Profile1D.cc \
    Scatter2D.cc \
    Scatter3D.cc\
    Writer.cc \
    WriterAIDA.cc \
    WriterYODA.cc \
    Reader.cc \
    ReaderAIDA.cc

libYODA_la_LIBADD = tinyxml/libtinyxml.la
libYODA_la_CPPFLAGS = $(AM_CPPFLAGS) -DTIXML_USE_STL
