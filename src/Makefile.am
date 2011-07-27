SUBDIRS = tinyxml .

lib_LTLIBRARIES = libYODA.la

libYODA_la_SOURCES = \
    Dbn2D.cc \
    Bin2D.cc \
    HistoBin2D.cc \
    Histo2D.cc \
    Scatter3D.cc

libYODA_la_LIBADD = tinyxml/libtinyxml.la
libYODA_la_CPPFLAGS = $(AM_CPPFLAGS) -DTIXML_USE_STL
