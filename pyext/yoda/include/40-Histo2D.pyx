cdef extern from "YODA/Histo2D.h" namespace "YODA":
    #cHisto2D operator + (cHisto2D &, cHisto2D &)
    #cHisto2D operator - (cHisto2D &, cHisto2D &)

    cdef cppclass cHisto2D "YODA::Histo2D"(cAnalysisObject):
        cHisto2D(size_t nbinsX, double lowerX, double upperX,
                 size_t nbinsY, double lowerY, double upperY,
                 string &path, string &title)
        
        cHisto1D(cHisto1D &h, string &path)
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

