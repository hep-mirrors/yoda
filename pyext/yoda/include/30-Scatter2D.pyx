cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass cScatter2D "YODA::Scatter2D" (cAnalysisObject):
        size_t numPoints()
        vector[cPoint2D] points()
        cScatter2D (cScatter2D &s)

cdef class Scatter2D(AnalysisObject):

    cdef cScatter2D * ptr(self):
        return <cScatter2D *> self.thisptr

    cdef setptr(self, cScatter2D *ptr):
        self.thisptr = ptr
        return self

    @property
    def numPoints(self):
        return self.ptr().numPoints()

    @property
    def points(self):
        cdef vector[cPoint2D] vec
        cdef int i
        cdef cPoint2D *p
        vec = self.ptr().points()
        out = []
        for i in range(vec.size()):
            pt = Point2D()
            pt.thisptr[0] = vec[i]
            out.append(pt)

        return out

    def __repr__(self):
        return 'Scatter2D%r' % self.points

    def __dealloc__(self):
        del self.thisptr
