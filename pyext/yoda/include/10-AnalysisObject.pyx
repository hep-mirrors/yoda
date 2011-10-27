cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass cAnalysisObject "YODA::AnalysisObject":
        string type()

ctypedef cAnalysisObject* AOptr

cdef class AnalysisObject:
    """Base object class"""
    cdef cAnalysisObject *thisptr

    @property
    def type(self):
        """The type of this analysis object as a string"""
        return self.thisptr.type().c_str()

    # TODO: Map annotations, etc.
