from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair
from libcpp cimport bool

cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass AnalysisObject:
        string type()

cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass Reader:
        vector[AnalysisObject *] read(string filename)

cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass Scatter2D(AnalysisObject):
        size_t numPoints()
        vector[Point2D] points()
        Scatter2D (Scatter2D &s)

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

cdef extern from "YODA/Dbn1D.h" namespace "YODA":
    cdef cppclass Dbn1D:
        pass

cdef extern from "YODA/HistoBin1D.h" namespace "YODA":
    cdef cppclass HistoBin1D:
        HistoBin1D (HistoBin1D &h)
        double area()
        double height()
        double areaErr()
        double heightErr()
        HistoBin1D operator + (HistoBin1D &, HistoBin1D &)
        HistoBin1D operator - (HistoBin1D &, HistoBin1D&)
        pass

cdef extern from "YODA/Histo1D.h" namespace "YODA":
    Histo1D add(Histo1D &, Histo1D &)
    Histo1D subtract(Histo1D &, Histo1D &)
    Scatter2D divide(Histo1D &, Histo1D &)

    cdef cppclass Histo1D(AnalysisObject):
        Histo1D(size_t nbins, double lower, double upper, string &path,
                string &title)
        Histo1D(vector[double] &binedges, string &path, string &title)
        Histo1D(vector[double] &binedges)
        Histo1D(Histo1D &h, string &path)
        Histo1D(Histo1D &h)
        void fill(double x, double weight)
        void reset()
        void scaleW(double scalefactor)
        void mergeBins(size_t a, size_t b)
        void rebin(int n)
        # Bin Accessors
        size_t numBins()
        double lowEdge()
        double highEdge()
        vector[HistoBin1D] &bins()
        Dbn1D &underflow()
        Dbn1D &overflow()
        void eraseBin(size_t index)
        # Statistical functions
        double integral(bool includeoverflows)
        double integral(size_t a, size_t b)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double variance(bool includeoverflows)
        double stdDev(bool includeoverflows)
