SUBDIRS = tinyxml .

lib_LTLIBRARIES = libYODA.la

## Compile ReaderYODA first: since it takes longest, this speeds up -jN compilation!
libYODA_la_SOURCES = \
    ReaderYODA.cc \
    ReaderAIDA.cc \
    Writer.cc \
    WriterYODA.cc \
    WriterAIDA.cc \
    WriterFLAT.cc \
    Dbn1D.cc \
    Histo1D.cc \
    Histo2D.cc \
    Profile1D.cc \
    Profile2D.cc \
    Scatter1D.cc \
    Scatter2D.cc \
    Scatter3D.cc

libYODA_la_LIBADD = tinyxml/libtinyxml.la
libYODA_la_CPPFLAGS = $(AM_CPPFLAGS) -DTIXML_USE_STL
