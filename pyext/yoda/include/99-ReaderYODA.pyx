cdef extern from "YODA/ReaderYODA.h" namespace "YODA::ReaderYODA":
    cReader createReaderYODA "YODA::ReaderYODA::create" () 


def __read_YODA(char *filename):
    """Takes a file, returns a list of AnalysisObjects"""
    cdef vector[AOptr] vec = vector[AOptr]()
    cdef size_t i
    cdef AnalysisObject ana, ana_

    createReaderYODA().read(string(filename), vec)
    out = []

    ana_ = AnalysisObject()

    for i in range(vec.size()):
        ana_.thisptr = vec[i]

        if ana_.type == 'Histo1D':
            out.append(Histo1D_fromptr(<cHisto1D*>vec[i], True))
        elif ana_.type == 'Profile1D':
            out.append(Profile1D_fromptr(<cProfile1D*>vec[i], True))
        elif ana_.type == 'Scatter2D':
            out.append(Scatter2D_fromptr(<cScatter2D*>vec[i], True))

    return out


cdef class ReaderYODA:
    """Read YODA files"""
    read = staticmethod(__read_YODA)
