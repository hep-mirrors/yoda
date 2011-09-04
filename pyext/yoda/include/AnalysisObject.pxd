cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass cAnalysisObject "YODA::AnalysisObject":
        string type()
