cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass cReader "YODA::Reader":
        vector[cAnalysisObject *] read(string filename)
