cdef extern from "YODA/Histo1D.h" namespace "YODA":
    #cHisto1D operator + (cHisto1D &, cHisto1D &)
    #cHisto1D operator - (cHisto1D &, cHisto1D &)
    #cScatter2D operator / (cHisto1D &, cHisto1D &)"""

    cdef cppclass cHisto1D "YODA::Histo1D"(cAnalysisObject):
        cHisto1D()
        cHisto1D(size_t nbins, double lower, double upper, string &path, string &title)
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
        cHistoBin1D & bin "bin"(size_t i)
        cDbn1D &totalDbn()
        cDbn1D &underflow()
        cDbn1D &overflow()
        void addBin(double low, double high)
        void addBins(vector[double] &binedges)
        void eraseBin(size_t index)

        # Statistical functions
        double integral(bool includeoverflows)
        double integral(size_t a, size_t b)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double mean(bool includeoverflows)
        double variance(bool includeoverflows)
        double stdDev(bool includeoverflows)
        double stdErr(bool includeoverflows)


cdef extern from "shims.h":
    cHisto1D add_Histo1D (cHisto1D &, cHisto1D &)
    cHisto1D subtract_Histo1D (cHisto1D &, cHisto1D &)
    cScatter2D divide_Histo1D (cHisto1D &, cHisto1D &)
    cScatter2D Scatter2D_mkScatter(cHisto1D &)

from cython.operator cimport dereference as deref


cdef class Histo1D(AnalysisObject):
    def __init__(self, *args, **kwargs):
        self._dealloc = True
        cdef:
            size_t nbins
            double lower
            double upper
            char* path = '/'
            char* title = ''


        # TODO: Handle more parameters (esp. path and title), and handle list[double] arguments

        if len(args) == 3:
            nbins, lower, upper = args[0], args[1], args[2]

            self.setptr(
                new cHisto1D(nbins, lower, upper, string(path), string(title))
            )
        else:
            raise ValueError('Histo1D: Expected 3 arguments')


    cdef cHisto1D* ptr(self):
        return <cHisto1D *> self.thisptr


    def asScatter(self):
        """
        h.asScatter() -> Scatter2D

        Return a 2D scatter data object from the histogram's bins and heights

        """
        cdef cScatter2D *s = new cScatter2D()
        s[0] = Scatter2D_mkScatter(self.ptr()[0])
        return Scatter2D_fromptr(s, True)


    def fill(self, double x, double weight=1.0):
        """
        h.fill(x[, weight=1.0]) -> self

        Fill the given histogram with value x and optional weighting

        """
        self.ptr().fill(x, weight)
        return self


    def reset(self):
        """
        h.reset() -> self

        Reset the histogram but leave the bin structure

        """
        self.ptr().reset()
        return self


    def scaleW(self, double factor):
        """
        s.scaleW(factor) -> self

        Scale the histogram and its statistics by given factor

        """
        self.ptr().scaleW(factor)
        return self


    def mergeBins(self, size_t a, size_t b):
        """
        s.mergeBins(from, to) -> self

        Merge bins between indexes from `from` to `to`

        """
        self.ptr().mergeBins(a, b)


    def rebin(self, int n):
        """
        s.rebin(n) -> self

        Merge every nth bin in the current histogram.

        """
        self.ptr().rebin(n)


    @property
    def bins(self):
        """
        h.bins -> tuple(HistoBin1D)

        Access the bin objects of this histogram. Bin objects are mutable and
        changes to the bin objects will be propagated back to the histogram
        unless the copy() method is called on a bin.

        """
        cdef size_t numbins = self.ptr().numBins()
        cdef size_t i
        cdef HistoBin1D bin

        out = []
        for i in xrange(numbins):
            bin = HistoBin1D_fromptr(& self.ptr().bins()[i])
            out.append(bin)
        # TODO: Why was this here?
        # self.ptr().bins()

        return out


    @property
    def lowEdge(self):
        """
        h.lowEdge -> float

        The x-value of the lowest edge of the lowest bin of the histogram.

        """
        return self.ptr().lowEdge()


    @property
    def highEdge(self):
        """
        h.highEdge -> float

        The x-value of the highest edge of the highest bin of the histogram.

        """
        return self.ptr().highEdge()


    @property
    def totalDbn(self):
        """
        h.totalDbn -> Distribution1D

        Return the Distribution1D object representing the total distribution.

        """
        return Dbn1D_fromptr(&self.ptr().totalDbn())


    @property
    def underflow(self):
        """
        h.underflow -> Distribution1D

        Return the Distribution1D object representing the underflow.

        """
        return Dbn1D_fromptr(&self.ptr().underflow())


    @property
    def overflow(self):
        """
        h.overflow -> Distribution1D

        Return the Distribution1D object representing the overflow.

        """
        return Dbn1D_fromptr(&self.ptr().overflow())


    def addBin(self, low, high):
        self.ptr().addBin(low, high)


    def addBins(self, binedges):
        # TODO: How to map the Python list to the C++ vector<double>?
        #void addBins(vector[double] &binedges)
        pass


    def __delitem__(self, size_t ix):
        self.ptr().eraseBin(ix)


    def __getitem__(self, size_t ix):
        return HistoBin1D_fromptr(& self.ptr().bin(ix))


    def integral(self, bool overflows=True):
        """
        s.integral([overflows]) -> float

        Return the total area of the histogram. If overflows is False, ignore
        over-and underflow bins and any bin gaps.

        """
        return self.ptr().integral(overflows)


    def sumW(self, bool overflows=True):
        """
        s.sumW([overflows]) -> float

        Return the sum of weights of the histogram. If overflows is False,
        ignore over-and underflow bins and any bin gaps.

        """

        return self.ptr().sumW(overflows)


    def sumW2(self, bool overflows=True):
        """
        s.sumW2([overflows]) -> float

        Return the sum of weights squared. If overflows is False, ignore
        over-and underflow bins and any bin gaps.

        """
        return self.ptr().sumW2(overflows)


    def mean(self, bool overflows=True):
        """
        s.mean([overflows]) -> float

        Return the mean. If overflows is False, ignore the over- and underflow
        bins and any bin gaps.

        """
        return self.ptr().mean(overflows)


    def variance(self, bool overflows=True):
        """
        s.variance([overflows]) -> float

        Return the variance. If overflows is False, ignore the over- and
        underflow bins and any bin gaps.

        """
        return self.ptr().variance(overflows)


    def stdDev(self, bool overflows=True):
        """
        s.stdDev([overflows]) -> float

        Return the standard deviation. If overflows is False, ignore over- and
        underflow bins and any bin gaps.

        """
        return self.ptr().stdDev(overflows)


    def stdErr(self, bool overflows=True):
        """
        s.stdErr([overflows]) -> float

        Return the standard error. If overflows is False, ignore over- and
        underflow bins and any bin gaps.

        """
        return self.ptr().stdErr(overflows)




    def __add__(Histo1D a, Histo1D b):
        cdef cHisto1D *res = new cHisto1D(add_Histo1D(a.ptr()[0], b.ptr()[0]))
        return Histo1D_fromptr(res, True)


    def __sub__(Histo1D a, Histo1D b):
        cdef cHisto1D *res = new cHisto1D(subtract_Histo1D(a.ptr()[0], b.ptr()[0]))
        return Histo1D_fromptr(res, True)


    def __mul__(x, y):
        """
        Scalar multiplication. Equivalent to scaleW acting on a copy.

        """
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
        return Histo1D_fromptr(res, True)


    def _div_scalar(Histo1D x, double y):
        if y == 0:
            raise ArithmeticError('Histo1D: Divide by zero scalar')

        cdef cHisto1D *res = new cHisto1D(x.ptr()[0])

        res.scaleW(1.0 / y)
        return Histo1D_fromptr(res, True)


    def _div_histo(Histo1D x, Histo1D y):
        cdef cScatter2D s = divide_Histo1D(x.ptr()[0], y.ptr()[0])
        return Scatter2D_fromptr(&s)


    def __div__(x, y):
        """
        Division by scalar (i.e. multiplication by reciprocal) or another
        1D histogram.

        """
        tx = type(x); ty = type(y)
        if tx is Histo1D:
            if ty is int or ty is float:
                return x._div_scalar(y)
            elif ty is Histo1D:
                return x._div_histo(y)

        raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))


    def __repr__(self):
        return 'Histo1D%r' % self.bins


cdef Histo1D Histo1D_fromptr(cHisto1D *ptr, bool dealloc=False):
    cdef Histo1D histo = Histo1D.__new__(Histo1D)
    return histo.setptr(ptr, dealloc)
