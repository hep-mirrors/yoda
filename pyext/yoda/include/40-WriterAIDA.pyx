cdef extern from "shims.h":
    void WriterAIDA_write (string&, vector[AOptr]&) except +

def __write_AIDA(char* filename, aos):
    """Takes a file, returns a list of AnalysisObjects"""
    cdef vector[AOptr] vec = vector[AOptr]()
    cdef AnalysisObject ana

    for ao in aos:
        ana = <AnalysisObject> ao
        vec.push_back(ana.thisptr)

    WriterAIDA_write(string(filename), vec)

class WriterAIDA:
    """Read AIDA files"""
    write = staticmethod(__write_AIDA)
