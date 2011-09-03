cimport cyoda
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair
from libcpp cimport bool

cdef class AnalysisObject:
    """Base object class"""
    cdef cyoda.AnalysisObject *thisptr

    @property
    def type(self):
        return self.type_()

    cdef str type_(self):
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

    cdef setptr(self, cyoda.Scatter2D *ptr):
        self.thisptr = <cyoda.AnalysisObject *> ptr
        return self

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

    def __repr__(self):
        return 'Scatter2D%r' % self.points

cdef class ReaderAIDA:
    def read(self, char *filename):
        cdef vector[cyoda.AnalysisObject *] vec
        cdef int i
        vec = cyoda.create().read(string(filename))
        out = []
        for i in range(vec.size()):
            if str(vec[i].type().c_str()) == 'Scatter2D':
                ana = Scatter2D()
            else:
                ana = AnalysisObject()

            ana.thisptr = vec[i]

            out.append(ana)

        return out


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
                new cyoda.Histo1D(nbins, lower, upper, string(path), string(title))
            )

    cdef cyoda.Histo1D* ptr(self):
        return <cyoda.Histo1D *> self.thisptr

    cdef setptr(self, cyoda.Histo1D *ptr):
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
        
        cdef vector[cyoda.HistoBin1D] bins = self.ptr().bins()

        cdef cyoda.HistoBin1D *b

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

    def __add__(Histo1D a, Histo1D b):
        cdef cyoda.Histo1D *res
        res = new cyoda.Histo1D(cyoda.add(a.ptr()[0], b.ptr()[0]))
        return Histo1D().setptr(res)

    def __sub__(Histo1D a, Histo1D b):
        cdef cyoda.Histo1D *res
        res = new cyoda.Histo1D(cyoda.subtract(a.ptr()[0], b.ptr()[0]))
        return Histo1D().setptr(res)

    def __mul__(x, y):
        cdef cyoda.Histo1D *res
        tx, ty = type(x), type(y)
        if (tx is int or tx is float) and ty is Histo1D:
            histo = <Histo1D> y; factor = <Histo1D> x
        elif tx is Histo1D and (ty is int or ty is float):
            histo = <Histo1D> x; factor = <Histo1D> y
        else:
            raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

        print histo, factor
        res = new cyoda.Histo1D(histo.ptr()[0])
        res.scaleW(factor)
        return Histo1D().setptr(res)

    def _div_scalar(Histo1D x, double y):
        cdef cyoda.Histo1D *res = new cyoda.Histo1D(x.ptr()[0])
        if y == 0:
            raise ArithmeticError('Histo1D: Divide by zero scalar')
        res.scaleW(1.0 / y)
        return Histo1D().setptr(res)

    def _div_histo(Histo1D x, Histo1D y):
        cdef cyoda.Scatter2D *res

        res = new cyoda.Scatter2D(cyoda.divide(x.ptr()[0], y.ptr()[0]))
        return Scatter2D().setptr(res)

    def __div__(x, y):
        tx = type(x); ty = type(y)
        if tx is Histo1D:
            if ty is int or ty is float:
                return x._div_scalar(y)
            elif ty is Histo1D:
                return x._div_histo(y)
        
        raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))
    
    def __repr__(self):
        return 'Histo1D%r' % self.bins


cdef class Dbn1D:
    pass

cdef class Bin1D:
    pass
         
cdef class HistoBin1D(Bin1D):
    cdef cyoda.HistoBin1D *thisptr

    cdef setptr(self, cyoda.HistoBin1D *ptr):
        return self

    cdef set(self, cyoda.HistoBin1D ptr):
        self.thisptr = new cyoda.HistoBin1D(ptr)
        return self

    cdef cyoda.HistoBin1D* ptr(self):
        return self.thisptr

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

    def __repr__(self):
        return 'HistoBin1D(%r)' % self.area
