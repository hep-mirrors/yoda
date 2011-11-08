cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass cAnalysisObject "YODA::AnalysisObject":
        string type()

ctypedef cAnalysisObject* AOptr 

cdef class AnalysisObject:
    """Base object class"""
    cdef cAnalysisObject *thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    @property
    def type(self):
        """The type of this analysis object as a string"""
        return self.thisptr.type().c_str()

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef AnalysisObject setptr(self, AOptr ptr, dealloc=False):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self
