from libcpp.vector cimport vector
from string cimport string
from libcpp.pair cimport pair
from libcpp cimport bool
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
cdef extern from "YODA/Dbn1D.h" namespace "YODA":
    cdef cppclass cDbn1D "Dbn1D":
        pass
cdef extern from "YODA/Point2D.h" namespace "YODA":
    cdef cppclass cPoint2D "YODA::Point2D":
        cPoint2D ()
        cPoint2D (cPoint2D &p)

        cPoint2D (double x, double y,
                  double exminus, double explus,
                 double eyminus, double eyplus)

        double x()
        double y()
        void setX(double x)
        void setY(double y)
        double xMin()
        double xMax()
        pair[double,double] xErrs()
        pair[double,double] yErrs()
        void setXErr(double minus, double plus)
        void setYErr(double minus, double plus)

cdef class Point2D:
    cdef cPoint2D* thisptr

    def __cinit__(self):
        self.thisptr = new cPoint2D()

    def __init__(self,  *args):
        if len(args) == 0:
            self.pos = 0, 0
        elif len(args) == 2 :
            self.pos = args
        elif len(args) == 4:
            self.pos = args[:2]
            self.xErrs, self.yErrs = args[2:]
        elif len(args) == 6:
            self.pos = args[:2]
            self.xErrs = args[2:4]
            self.yErrs = args[4:]
        else:
            raise ValueError(
                'Wrong number of values: can take 2, 4, or 6 parameters')
        
    def _x(self):
        return self.thisptr.x()

    def _y(self):
        return self.thisptr.y()

    def _setX(self, double x):
        self.thisptr.setX(x)

    def _setY(self, double y):
        self.thisptr.setY(y)

    x = property(_x, _setX)
    y = property(_y, _setY)

    def _pos(self):
        """(x, y) coordinates of this point"""
        return (self.x, self.y)

    def _setPos(self, pos):
        cdef double x, y
        x, y = pos
        self.thisptr.setX(x)
        self.thisptr.setY(y)

    pos = property(_pos, _setPos)

    def _xErrs(self):
        """The x-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] xErrs = self.thisptr.xErrs()
        return (xErrs.first, xErrs.second)

    def _setxErrs(self, arg):
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.thisptr.setXErr(low, high)

    xErrs = property(_xErrs, _setxErrs)

    def _yErrs(self):
        """The y-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] yErrs = self.thisptr.yErrs()
        return (yErrs.first, yErrs.second)

    def _setyErrs(self, arg):
        cdef double low, high
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.thisptr.setYErr(low, high)

    yErrs = property(_yErrs, _setyErrs)

    def __repr__(self):
        return 'Point2D({0},{1})'.format(self.x, self.y)

    def __dealloc__(self):
        del self.thisptr
cdef extern from "YODA/HistoBin1D.h" namespace "YODA":

    cdef cppclass cHistoBin1D "YODA::HistoBin1D":
        cHistoBin1D (cHistoBin1D &h)
        double area()
        double height()
        double areaErr()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
        # way to make Cython acknowledge these (it seems to be a typedef parsing
        # issue rather than a technical issue).

        void scaleW(double factor)
        void scaleX(double factor)
        
        double lowEdge()
        double highEdge()
        double width()
        double focus()
        double midpoint()
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()
        double numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()

# Disabled to work with Cython 0.13
#cdef extern from "YODA/HistoBin1D.h" namespace "YODA::HistoBin1D":
#    cHistoBin1D operator + (cHistoBin1D &, cHistoBin1D &)
#    cHistoBin1D operator - (cHistoBin1D &, cHistoBin1D &)"""

# TODO: re-enable these operators using a c++ shim like
#using namespace YODA
#Histo1D add__Histo1D(Histo1D a, Histo1D b) 
#{ 
#       return YODA::Histo1D::add(a, b); 
#} 


cdef class HistoBin1D:
    cdef cHistoBin1D *thisptr

    cdef setptr(self, cHistoBin1D *ptr):
        return self

    cdef set(self, cHistoBin1D ptr):
        self.thisptr = new cHistoBin1D(ptr)
        return self

    cdef cHistoBin1D* ptr(self):
        return self.thisptr

    @property
    def lowEdge(self):
        return self.ptr().lowEdge()
    
    xMin = lowEdge

    @property
    def highEdge(self):
        return self.ptr().highEdge()

    xMax = highEdge

    @property
    def width(self):
        return self.ptr().width()

    @property
    def focus(self):
        return self.ptr().focus()

    @property
    def midpoint(self):
        return self.ptr().midpoint()

    @property
    def xMean(self):
        return self.ptr().xMean()

    @property
    def xVariance(self):
        return self.ptr().xVariance()

    @property
    def xStdDev(self):
        return self.ptr().xStdDev()

    @property
    def numEntries(self):
        return self.ptr().numEntries()

    @property
    def effNumEntries(self):
        return self.ptr().effNumEntries()

    @property
    def sumW(self):
        return self.ptr().sumW()

    @property
    def sumW2(self):
        return self.ptr().sumW2()

    @property
    def sumWX(self):
        return self.ptr().sumWX()

    @property
    def sumWX2(self):
        return self.ptr().sumWX2()

    @property
    def area(self):
        return self.ptr().area()

    @property
    def height(self):
        return self.ptr().height()

    @property
    def heightErr(self):
        return self.ptr().heightErr()

    @property
    def areaErr(self):
        return self.ptr().areaErr()

    def scaleX(self, double factor):
        self.ptr().scaleX(factor)

    def __repr__(self):
        return 'HistoBin1D(%r)' % self.area
cdef extern from "YODA/HistoBin2D.h" namespace "YODA":

    #cHistoBin2D operator + (cHistoBin2D &, cHistoBin2D &)
    #cHistoBin2D operator - (cHistoBin2D &, cHistoBin2D &)

    cdef cppclass cHistoBin2D "YODA::HistoBin2D":
        cHistoBin2D (cHistoBin2D &h)

        double volume()
        double volumeErr()
        double height()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
        # way to make Cython acknowledge these (it seems to be a typedef parsing
        # issue rather than a technical issue).

        void scaleW(double factor)
        
        double lowEdgeX()
        double lowEdgeY()
        double highEdgeX()
        double highEdgeY()
        double widthX()
        double widthY()
        pair[double,double] focus()
        pair[double,double] midpoint()
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()

        double yStdErr()
        double yMean()
        double yVariance()
        double yStdDev()

        double numEntries()
        # double effNumEntries()

        double sumW()
        double sumW2()

        double sumWX()
        double sumWY()
        double sumWXY()
        double sumWX2()
        double sumWY2()
cdef class HistoBin2D:
    cdef cHistoBin2D *thisptr

    cdef setptr(self, cHistoBin2D *ptr):
        return self

    cdef set(self, cHistoBin2D ptr):
        self.thisptr = new cHistoBin2D(ptr)
        return self

    cdef cHistoBin2D* ptr(self):
        return self.thisptr

    @property
    def volume(self):
        return self.ptr().volume()

    @property
    def volumeErr(self):
        return self.ptr().volumeErr()

    @property
    def height(self):
        return self.ptr().height()

    @property
    def heightErr(self):
        return self.ptr().heightErr()

    def scaleW(self, double factor):
        self.ptr().scaleW(factor)


    @property
    def lowEdgeX(self):
        return self.ptr().lowEdgeX()

    @property
    def highEdgeX(self):
        return self.ptr().highEdgeX()


    @property
    def lowEdgeY(self):
        return self.ptr().lowEdgeY()
    
    @property
    def highEdgeY(self):
        return self.ptr().highEdgeY()

    xMin = lowEdgeX
    xMax = highEdgeX
    yMin = lowEdgeY
    yMax = highEdgeY

    @property
    def widthX(self):
        return self.ptr().widthX()

    @property
    def widthY(self):
        return self.ptr().widthY()

    @property
    def focus(self):
        cdef pair[double, double] p
        p = self.ptr().focus()
        return (p.first, p.second)

    @property
    def midpoint(self):
        cdef pair[double, double] p
        p = self.ptr().midpoint()
        return (p.first, p.second)

    @property
    def xMean(self):
        return self.ptr().xMean()

    @property
    def xVariance(self):
        return self.ptr().xVariance()

    @property
    def xStdDev(self):
        return self.ptr().xStdDev()

    @property
    def xStdErr(self):
        return self.ptr().xStdDev()

    @property
    def yMean(self):
        return self.ptr().yMean()

    @property
    def yVariance(self):
        return self.ptr().yVariance()

    @property
    def yStdDev(self):
        return self.ptr().yStdDev()

    @property
    def yStdErr(self):
        return self.ptr().yStdDev()

    @property
    def numEntries(self):
        return self.ptr().numEntries()

    """@property
    def effNumEntries(self):
        return self.ptr().effNumEntries()"""

    @property
    def sumW(self):
        return self.ptr().sumW()

    @property
    def sumW2(self):
        return self.ptr().sumW2()

    @property
    def sumWX(self):
        return self.ptr().sumWX()

    @property
    def sumWY(self):
        return self.ptr().sumWY()

    @property
    def sumWXY(self):
        return self.ptr().sumWXY()

    @property
    def sumWX2(self):
        return self.ptr().sumWX2()

    @property
    def sumWY2(self):
        return self.ptr().sumWY2()

    def __repr__(self):
        return 'HistoBin2D(%r)' % self.volume
cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass cReader "YODA::Reader":
        void read(string filename, vector[AOptr]&) except + 
cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass cScatter2D "YODA::Scatter2D" (cAnalysisObject):
        size_t numPoints()
        vector[cPoint2D] points()
        cScatter2D (cScatter2D &s)

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
            pt.thisptr[0] = vec[i]
            out.append(pt)

        return out

    def __repr__(self):
        return 'Scatter2D%r' % self.points

    def __dealloc__(self):
        del self.thisptr
cdef extern from "YODA/Histo1D.h" namespace "YODA":
    #cHisto1D operator + (cHisto1D &, cHisto1D &)
    #cHisto1D operator - (cHisto1D &, cHisto1D &)
    #cScatter2D operator / (cHisto1D &, cHisto1D &)"""

    cdef cppclass cHisto1D "YODA::Histo1D"(cAnalysisObject):
        cHisto1D(size_t nbins, double lower, double upper, string &path,
                string &title)
        cHisto1D(vector[double] &binedges, string &path, string &title)
        cHisto1D(vector[double] &binedges)
        cHisto1D(cHisto1D &h, string &path)
        cHisto1D(cHisto1D &h)

        void fill(double x, double weight)
        void reset()
        void scaleW(double scalefactor)
        void mergeBins(size_t a, size_t b)
        void rebin(int n)

        # Bin Accessors
        size_t numBins()
        double lowEdge()
        double highEdge()
        vector[cHistoBin1D] &bins()
        cDbn1D &underflow()
        cDbn1D &overflow()
        void eraseBin(size_t index)

        # Statistical functions
        double integral(bool includeoverflows)
        double integral(size_t a, size_t b)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double variance(bool includeoverflows)
        double stdDev(bool includeoverflows)

cdef class Histo1D(AnalysisObject):
    def __cinit__(self, *args, **kwargs):
        cdef:
            size_t nbins
            double lower
            double upper
            char* path = '/'
            char* title = ''

        if len(args) == 3:
            nbins, lower, upper = args[0], args[1], args[2]

            self.setptr(
                new cHisto1D(nbins, lower, upper, string(path), string(title))
            )
        else:
            raise ValueError('Histo1D: Expected 3 arguments')

    cdef cHisto1D* ptr(self):
        return <cHisto1D *> self.thisptr

    cdef setptr(self, cHisto1D *ptr):
        self.thisptr = ptr
        return self
    
    def fill(self, double x, double weight=1.0):
        self.ptr().fill(x, weight)

    def reset(self):
        """Reset the histogram but leave the bin structure"""
        self.ptr().reset()

    def scaleW(self, double factor):
        """Scale the histogram and its statistics by given factor"""
        self.ptr().scaleW(factor)

    def mergeBins(self, size_t a, size_t b):
        self.ptr().mergeBins(a, b)

    def rebin(self, int n):
        self.ptr().rebin(n)

    @property
    def bins(self):
        cdef size_t numbins = self.ptr().numBins()
        cdef size_t i
        
        cdef vector[cHistoBin1D] bins = self.ptr().bins()

        cdef cHistoBin1D *b

        out = []

        for i in range(numbins):
            out.append(HistoBin1D().set(bins[i]))

        return out
    
    @property
    def lowEdge(self):
        return self.ptr().lowEdge()

    @property
    def highEdge(self):
        return self.ptr().highEdge()


    def underflow(self):
        pass

    def __delitem__(self, size_t ix):
        self.ptr().eraseBin(ix)

    def __getitem__(self, size_t ix):
        return HistoBin1D().set(self.ptr().bins()[ix])

    def integral(self, bool overflows=True):
        return self.ptr().integral(overflows)

    def sumW(self, bool overflows=True):
        return self.ptr().sumW(overflows)

    def sumW2(self, bool overflows=True):
        return self.ptr().sumW2(overflows)

    def variance(self, bool overflows=True):
        return self.ptr().variance(overflows)

    def stdDev(self, bool overflows=True):
        return self.ptr().stdDev(overflows)


    """def __add__(Histo1D a, Histo1D b):
        cdef cHisto1D *res
        res = new cHisto1D(a.ptr()[0] + b.ptr()[0])
        return Histo1D().setptr(res)

    def __sub__(Histo1D a, Histo1D b):
        cdef cHisto1D *res
        res = new cHisto1D(a.ptr()[0] - b.ptr()[0])
        return Histo1D().setptr(res)

    def __mul__(x, y):
        cdef cHisto1D *res
        tx, ty = type(x), type(y)
        if (tx is int or tx is float) and ty is Histo1D:
            histo = <Histo1D> y; factor = <Histo1D> x
        elif tx is Histo1D and (ty is int or ty is float):
            histo = <Histo1D> x; factor = <Histo1D> y
        else:
            raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

        res = new cHisto1D(histo.ptr()[0])
        res.scaleW(factor)
        return Histo1D().setptr(res)


    def _div_scalar(Histo1D x, double y):
        cdef cHisto1D *res = new cHisto1D(x.ptr()[0])
        if y == 0:
            raise ArithmeticError('Histo1D: Divide by zero scalar')
        res.scaleW(1.0 / y)
        return Histo1D().setptr(res)

    def _div_histo(Histo1D x, Histo1D y):
        cdef cScatter2D *res

        res = new cScatter2D(x.ptr()[0] / y.ptr()[0])
        return Scatter2D().setptr(res)

    def __div__(x, y):
        tx = type(x); ty = type(y)
        if tx is Histo1D:
            if ty is int or ty is float:
                return x._div_scalar(y)
            elif ty is Histo1D:
                return x._div_histo(y)
        
        raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))"""
    
    def __repr__(self):
        return 'Histo1D%r' % self.bins

cdef extern from "YODA/Histo2D.h" namespace "YODA":
    #cHisto2D operator + (cHisto2D &, cHisto2D &)
    #cHisto2D operator - (cHisto2D &, cHisto2D &)

    cdef cppclass cHisto2D "YODA::Histo2D"(cAnalysisObject):
        cHisto2D(size_t nbinsX, double lowerX, double upperX,
                 size_t nbinsY, double lowerY, double upperY,
                 string &path, string &title)
        
        cHisto2D(cHisto2D &h)

        void fill(double x, double y, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleXY(double scaleX, double scaleY)
        void mergeBins(size_t a, size_t b)
        void rebin(int a, int b)

        # Bin Accessors
        size_t numBins()
        double lowEdgeX()
        double lowEdgeY()
        double highEdgeX()
        double highEdgeY()

        vector[cHistoBin2D] &bins()
        cHistoBin2D& binByCoord(double x, double y)

        void eraseBin(size_t index)

        # Statistical functions
        double integral(bool includeoverflows)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double xMean(bool includeoverflows)
        double yMean(bool includeoverflows)

        double xVariance(bool includeoverflows)
        double yVariance(bool includeoverflows)
        
        double xStdDev(bool includeoverflows)
        double yStdDev(bool includeoverflows)

cdef class Histo2D(AnalysisObject):
    def __cinit__(self, *args, **kwargs):
        cdef:
            size_t nbinsX, nbinsY
            double lowX, highX, lowY, highY
            char* path = '/'
            char* title = ''

        if len(args) == 6:
            nbinsX, lowX, highX, nbinsY, lowY, highY = args

            self.setptr(
                new cHisto2D(nbinsX, lowX, highX, nbinsY, lowY, highY,
                             string(path), string(title))
            )

    cdef cHisto2D* ptr(self):
        return <cHisto2D *> self.thisptr

    cdef setptr(self, cHisto2D *ptr):
        self.thisptr = ptr
        return self
    
    def fill(self, double x, double y, double weight=1.0):
        self.ptr().fill(x, y, weight)

    def reset(self):
        """Reset the histogram but leave the bin structure"""
        self.ptr().reset()

    def scaleW(self, double factor):
        """Scale the histogram and its statistics by given factor"""
        self.ptr().scaleW(factor)

    def mergeBins(self, size_t a, size_t b):
        self.ptr().mergeBins(a, b)

    def rebin(self, int a, int b):
        self.ptr().rebin(a, b)

    @property
    def bins(self):
        cdef size_t numbins = self.ptr().numBins()
        cdef size_t i
        
        cdef vector[cHistoBin2D] bins = self.ptr().bins()

        cdef cHistoBin2D *b

        out = []

        for i in range(numbins):
            out.append(HistoBin2D().set(bins[i]))

        return out
    
    @property
    def lowEdgeX(self):
        return self.ptr().lowEdgeX()

    @property
    def lowEdgeY(self):
        return self.ptr().lowEdgeY()

    @property
    def highEdgeX(self):
        return self.ptr().highEdgeX()

    @property
    def highEdgeY(self):
        return self.ptr().highEdgeY()

    def underflow(self):
        pass

    def __delitem__(self, size_t ix):
        self.ptr().eraseBin(ix)

    def __getitem__(self, size_t ix):
        return HistoBin2D().set(self.ptr().bins()[ix])

    def integral(self, bool overflows=True):
        return self.ptr().integral(overflows)

    def sumW(self, bool overflows=True):
        return self.ptr().sumW(overflows)

    def sumW2(self, bool overflows=True):
        return self.ptr().sumW2(overflows)

    def variance(self, bool overflows=True):
        cdef cHisto2D *s = self.ptr()
        return (s.xVariance(overflows), s.yVariance(overflows))

    def stdDev(self, bool overflows=True):
        cdef cHisto2D *s = self.ptr()
        return (s.xStdDev(overflows), s.yStdDev(overflows))

    """def __add__(Histo2D a, Histo2D b):
        cdef cHisto2D *res
        res = new cHisto2D(a.ptr()[0] + b.ptr()[0])
        return Histo2D().setptr(res)

    def __sub__(Histo2D a, Histo2D b):
        cdef cHisto2D *res
        res = new cHisto2D(a.ptr()[0] - b.ptr()[0])
        return Histo2D().setptr(res)

    def __mul__(x, y):
        cdef cHisto2D *res
        tx, ty = type(x), type(y)
        if (tx is int or tx is float) and ty is Histo2D:
            histo = <Histo2D> y; factor = <Histo2D> x
        elif tx is Histo2D and (ty is int or ty is float):
            histo = <Histo2D> x; factor = <Histo2D> y
        else:
            raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

        res = new cHisto2D(histo.ptr()[0])
        res.scaleW(factor)
        return Histo2D().setptr(res)"""

    
    def __repr__(self):
        return 'Histo2D%r' % self.bins

cdef extern from "YODA/ReaderAIDA.h" namespace "YODA::ReaderAIDA":
    cReader createReaderAIDA "YODA::ReaderAIDA::create" ()

class ReaderAIDA:
    """Read AIDA files"""

    def read(self, char *filename):
        """Takes a file, returns a list of AnalysisObjects"""
        cdef vector[AOptr] vec = vector[AOptr]()
        cdef size_t i
        cdef AnalysisObject ana, ana_

        createReaderAIDA().read(string(filename), vec)
        out = []

        ana_ = AnalysisObject()

        for i in range(vec.size()):
            ana_.thisptr = vec[i]
            if ana_.type == 'Scatter2D':
                ana = Scatter2D()
            else:
                ana = AnalysisObject()
            ana.thisptr = vec[i]
            out.append(ana)
        
        #print str(vec[0].type().c_str())

        return out
