cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass cScatter2D "YODA::Scatter2D" (cAnalysisObject):
        size_t numPoints()
        vector[cPoint2D] points()
        cScatter2D (cScatter2D &s)

