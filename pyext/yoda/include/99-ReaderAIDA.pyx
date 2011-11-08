cdef extern from "YODA/ReaderAIDA.h" namespace "YODA::ReaderAIDA":
    cReader createReaderAIDA "YODA::ReaderAIDA::create" () 

def __read_AIDA(char *filename):
    """Takes a file, returns a list of AnalysisObjects"""
    cdef vector[AOptr] vec = vector[AOptr]()
    cdef size_t i
    cdef AnalysisObject ana, ana_

    createReaderAIDA().read(string(filename), vec)
    out = []

    ana_ = AnalysisObject()

    for i in range(vec.size()):
        ana_.thisptr = vec[i]

        if ana_.type == 'Scatter2D':
            out.append(Scatter2D_fromptr(<cScatter2D*>vec[i], True))
        elif ana_.type == 'Histo1D':
            out.append(Scatter2D_fromptr(<cScatter2D*>vec[i], True))

    return out

cdef class ReaderAIDA:
    """Read AIDA files"""
    read = staticmethod(__read_AIDA)
