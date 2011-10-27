cdef extern from "YODA/Histo1D.h" namespace "YODA":
    # TODO: We can use these in place of the workaround when Cython 0.15 is available
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
