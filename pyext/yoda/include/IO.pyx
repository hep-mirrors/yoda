"""Readers and writers

The basic idea here is to provide Python IO semantics by using Python to do
the IO. Otherwise we get C++ IO semantics in Python. It also means we can use
dummy files, e.g. anything with read/write attributes. Generally a much better
idea than just "give this a filename", and well worth the inefficiencies and
potential memory limits.
"""

import sys

cdef list _aobjects_to_list(vector[c.AnalysisObject*] *aobjects):
    cdef list out = []
    cdef c.AnalysisObject *ao
    cdef size_t i
    for i in range(aobjects.size()):
        ao = deref(aobjects)[i]
        out.append(
            util.new_owned_cls(
                globals()[ao.type().c_str()], ao))
    return out

# Set a istringstream's string from a C/Python string
cdef void _make_iss(c.istringstream &iss, char *s):
    iss.str(string(s))


##
## Readers
##

def read(filename):
    """
    Read data objects from the provided filename,
    auto-determining the format from the file extension.
    Returns a list of analysis objects
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    f = open(filename)
    s = f.read()
    f.close()
    _make_iss(iss, s)
    c.Reader_create(filename).read(iss, aobjects)
    # Not as expensive as it looks!
    return _aobjects_to_list(&aobjects)


def readYODA(file_or_filename):
    """
    Read data objects from the provided YODA-format file.
    Returns a list of analysis objects
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects

    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    else:
        f = open(file_or_filename)
        s = f.read()
        f.close()

    _make_iss(iss, s)
    c.ReaderYODA_create().read(iss, aobjects)

    # Not as expensive as it looks!
    return _aobjects_to_list(&aobjects)


def readFLAT(file_or_filename):
    """
    Read data objects from the provided FLAT-format file.
    Returns a list of analysis objects
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects

    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    else:
        f = open(file_or_filename)
        s = f.read()
        f.close()

    _make_iss(iss, s)
    c.ReaderFLAT_create().read(iss, aobjects)

    # Not as expensive as it looks!
    return _aobjects_to_list(&aobjects)


def readAIDA(file_or_filename):
    """
    Read data objects from the provided AIDA-format file.
    Returns a list of analysis objects
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects

    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    else:
        f = open(file_or_filename)
        s = f.read()
        f.close()

    _make_iss(iss, s)
    c.ReaderAIDA_create().read(iss, aobjects)

    # Not as expensive as it looks!
    return _aobjects_to_list(&aobjects)


##
## Writers
##

def write(ana_objs, filename):
    """
    Write data objects to the provided filename,
    auto-determining the format from the file extension.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    try:
        for a in ana_objs:
            vec.push_back(a._AnalysisObject())
    except:
        a = ana_objs
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    c.Writer_create(filename).write(oss, vec)

    if filename == "-":
        sys.stdout.write(oss.str().c_str())
    else:
        f = open(filename, "w")
        s = f.write(oss.str().c_str())
        f.close()


def writeYODA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in YODA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    try:
        for a in ana_objs:
            vec.push_back(a._AnalysisObject())
    except:
        a = ana_objs
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    c.WriterYODA_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        fname = file_or_filename
        if fname == "-":
            sys.stdout.write(oss.str().c_str())
        else:
            f = open(fname, "w")
            s = f.write(oss.str().c_str())
            f.close()


def writeFLAT(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in FLAT format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    try:
        for a in ana_objs:
            vec.push_back(a._AnalysisObject())
    except:
        a = ana_objs
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    c.WriterFLAT_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        fname = file_or_filename
        if fname == "-":
            sys.stdout.write(oss.str().c_str())
        else:
            f = open(fname, "w")
            s = f.write(oss.str().c_str())
            f.close()


def writeAIDA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in AIDA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    try:
        for a in ana_objs:
            vec.push_back(a._AnalysisObject())
    except:
        a = ana_objs
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    c.WriterAIDA_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        fname = file_or_filename
        if fname == "-":
            sys.stdout.write(oss.str().c_str())
        else:
            f = open(fname, "w")
            s = f.write(oss.str().c_str())
            f.close()
