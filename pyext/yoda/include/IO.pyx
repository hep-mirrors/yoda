# Readers and writers
# TODO: (low priority) refactor to improve readability.

# The basic idea here is to provide Python IO semantics by using Python to do
# the IO. Otherwise we get C++ IO semantics in Python. It also means we can use
# dummy files, e.g. anything with read/write attirbutes. Generally a much better
# idea than just "give this a filename", and well worth the inefficiencies and
# potential memory limits.

cdef list aobjects_to_list(vector[c.AnalysisObject*] *aobjects):
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
cdef void make_iss(c.istringstream &iss, char *s):
    iss.str(string(s))

##
## Readers
##

def readYODA(file_or_filename):
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects

    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    else:
        with open(file_or_filename) as f:
            s = f.read()

    make_iss(iss, s)
     
    c.ReaderYODA_create().read(iss, aobjects)

    # Not as expensive as it looks!
    return aobjects_to_list(&aobjects)

def readAIDA(file_or_filename):
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects

    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    else:
        with open(file_or_filename) as f:
            s = f.read()

    make_iss(iss, s)
     
    c.ReaderAIDA_create().read(iss, aobjects)

    # Not as expensive as it looks!
    return aobjects_to_list(&aobjects)

##
## Writers
##

def writeYODA(ana_objs, file_or_filename):
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    for a in ana_objs:
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    # Perhaps speak with andy re: huge files
    c.WriterYODA_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        with open(file_or_filename, 'w') as f:
            f.write(oss.str().c_str())


def writeFLAT(ana_objs, file_or_filename):
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    for a in ana_objs:
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    # Perhaps speak with andy re: huge files
    c.WriterFLAT_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        with open(file_or_filename, 'w') as f:
            f.write(oss.str().c_str())


def writeAIDA(ana_objs, file_or_filename):
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    for a in ana_objs:
        vec.push_back(a._AnalysisObject())

    # Most of the time we just won't care about memory
    # Perhaps speak with andy re: huge files
    c.WriterAIDA_create().write(oss, vec)

    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(oss.str().c_str())
    else:
        with open(file_or_filename, 'w') as f:
            f.write(oss.str().c_str())
