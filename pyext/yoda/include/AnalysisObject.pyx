cdef class AnalysisObject:
    """Base object class"""
    cdef cAnalysisObject *thisptr

    @property
    def type(self):
        """The type of this analysis object as a string"""
        return self.type_()
