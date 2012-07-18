cdef extern from "shims.h":
    void WriterYODA_write (string&, vector[AOptr]&) except +

def __write_YODA(char* filename, aos):
    """Takes a file, returns a list of AnalysisObjects"""
    cdef vector[AOptr] vec = vector[AOptr]()
    cdef AnalysisObject ana

    for ao in aos:
        ana = <AnalysisObject> ao
        vec.push_back(ana.thisptr)

    WriterYODA_write(string(filename), vec)

class WriterYODA:
    """Read YODA files"""
    write = staticmethod(__write_YODA)
