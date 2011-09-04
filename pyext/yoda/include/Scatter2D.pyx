cdef class Scatter2D(AnalysisObject):

    @property
    def numPoints(self):
        return self.ptr().numPoints()
    
    cdef cScatter2D * ptr(self):
        return <cScatter2D *> self.thisptr

    cdef setptr(self, cScatter2D *ptr):
        self.thisptr = ptr
        return self

    @property
    def points(self):
        cdef vector[cPoint2D] vec
        cdef int i
        cdef cPoint2D *p
        vec = self.ptr().points()
        out = []
        for i in range(vec.size()):
            pt = Point2D()
            p = new cPoint2D(vec[i])
            pt.thisptr = p
            out.append(pt)

        return out

    def __repr__(self):
        return 'Scatter2D%r' % self.points

