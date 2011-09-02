cimport cyoda
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair

cdef class AnalysisObject:
    """Base object class"""
    cdef cyoda.AnalysisObject *thisptr

    def type(self):
        return self.thisptr.type().c_str()

cdef class Point2D:
    cdef cyoda.Point2D *thisptr

    @property
    def x(self):
        return self.thisptr.x()

    @property
    def y(self):
        return self.thisptr.y()

    @property
    def xErrs(self):
        cdef pair[double, double] xErrs = self.thisptr.xErrs()
        return (xErrs.first, xErrs.second)

    @property
    def yErrs(self):
        cdef pair[double, double] yErrs = self.thisptr.yErrs()
        return (yErrs.first, yErrs.second)

    def __repr__(self):
        return 'Point2D({0},{1})'.format(self.x, self.y)


cdef class Scatter2D(AnalysisObject):

    @property
    def numPoints(self):
        return self.ptr().numPoints()
    
    cdef cyoda.Scatter2D * ptr(self):
        return <cyoda.Scatter2D *> self.thisptr

    @property
    def points(self):
        cdef vector[cyoda.Point2D] vec
        cdef int i
        cdef cyoda.Point2D *p
        vec = self.ptr().points()
        out = []
        for i in range(vec.size()):
            pt = Point2D()
            p = new cyoda.Point2D(vec[i])
            pt.thisptr = p
            out.append(pt)

        return out

cdef class ReaderAIDA:
    def read(self, char *filename):
        cdef vector[cyoda.AnalysisObject *] vec
        cdef int i
        vec = cyoda.create().read(string(filename))
        out = []
        for i in range(vec.size()):
            if str(vec[i].type().c_str()) == 'Scatter2D':
                print (<cyoda.Scatter2D *> vec[i]).points().at(0).x(),\
                      (<cyoda.Scatter2D *> vec[i]).points().at(0).y()
                ana = Scatter2D()
            else:
                ana = AnalysisObject()

            ana.thisptr = vec[i]

            out.append(ana)

        return out
