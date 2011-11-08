cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass cScatter2D "YODA::Scatter2D" (cAnalysisObject):
        size_t numPoints()
        vector[cPoint2D] points()
        cPoint2D& point(size_t i)
        cScatter2D (cScatter2D &s)
        cScatter2D (vector[cPoint2D]&, string, string)
        cScatter2D ()

cdef class Scatter2D(AnalysisObject):
    cdef tuple _points

    def __init__(self, points, char *path="", char *title=""):
        points = tuple(points)

        cdef size_t N = len(points)
        cdef vector[cPoint2D] point_vector = vector[cPoint2D](N)
        cdef Point2D item
        cdef cScatter2D *scatter
        cdef int i
        for i in range(N):
            item = points[i]
            point_vector[i] = item.ptr()[0]
        
        scatter = new cScatter2D(point_vector, string(path), string(title))
        self.setptr(scatter, True)

        self._points = points

    @property
    def numPoints(self):
        return self.ptr().numPoints()
    
    cdef cScatter2D * ptr(self):
        return <cScatter2D *> self.thisptr

    def __getitem__(self, args):
        return self.points.__getitem__(args)

    def copy(self):
        return Scatter2D_fromptr(new cScatter2D(self.ptr()[0]), True)

    @property
    def points(self):
        cdef size_t i
        cdef Point2D pt
        if self._points:
            return self._points
        else:
            out = []

            for i in range(self.ptr().numPoints()):
                pt = Point2D_fromptr(& self.ptr().point(i))
                out.append(pt)

            out = tuple(out)
            self._points = out
            return out

    def __repr__(self):
        return '<Scatter2D>'

        
# dealloc decides whether or not the python object is responsible for freeing
# used memory. Most times, it's not - we're just wrapping a C++ instance.
# However, the same classes should be used for both wrapping and creating.

# It is important that we do free allocated C++ memory, because otherwise we can
# quickly end up with memory leaks.

cdef Scatter2D Scatter2D_fromptr(cScatter2D* ptr, dealloc = False):
    cdef Scatter2D scatter = Scatter2D.__new__(Scatter2D)
    return scatter.setptr(ptr, False)
