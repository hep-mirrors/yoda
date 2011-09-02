from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair

cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass AnalysisObject:
        string type()

cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass Reader:
        vector[AnalysisObject *] read(string filename)

cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass Scatter2D:
        size_t numPoints()
        vector[Point2D] points()

cdef extern from "YODA/Point2D.h" namespace "YODA":
    cdef cppclass Point2D:
        Point2D(Point2D &p)
        double x()
        double y()
        double xMin()
        double xMax()
        pair[double,double] xErrs()
        pair[double,double] yErrs()

cdef extern from "YODA/ReaderAIDA.h" namespace "YODA::ReaderAIDA":
    Reader& create()
