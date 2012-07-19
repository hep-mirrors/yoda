cdef extern from "YODA/Dbn2D.h" namespace "YODA":
    cdef cppclass cDbn2D "Dbn2D":
        cDbn2D()
        cDbn2D(unsigned int numEntries, double sumW, double sumW2, double sumWX, double sumWX2, double sumWY, double sumWY2, double sumWXY)
        void fill(double valX, double valY, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleX(double factor)
        void scaleY(double factor)
        void scaleXY(double factor)
        double xMean()
        double yMean()
        double xVariance()
        double yVariance()
        double xStdDev()
        double yStdDev()
        double xStdErr()
        double yStdErr()
        double yRMS()
        double xRMS()
        unsigned long numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()
        double sumWY()
        double sumWY2()
        double sumWXY()
        # TODO: map operator+ and operator-, both internal and external


cdef class Dbn2D:
    """
    A 2D distribution 'counter', used and exposed by 1D histograms and their bins.

    TODO: Allow no-arg construction?
    TODO: map operator+ and operator-, both internal and external
    """

    cdef cDbn2D* thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr


    def __init__(self, *args, **kwargs):
        cdef:
            unsigned int numEntries
            double sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWXY
        assert len(args) == 8
        numEntries, sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWXY = args
        self.setptr(new cDbn2D(numEntries, sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWXY), True)


    cdef Dbn2D setptr(self, cDbn2D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cDbn2D* ptr(self):
        return self.thisptr


    def fill(self, double x, double y, double weight=1.0):
        "Fill the distribution with the given weight at given x and y."
        self.ptr().fill(x, y, weight)

    def reset(self):
        "Reset the distribution counters to the unfilled state."
        self.ptr().reset()

    def scaleW(self, scalefactor):
        "Scale the fill weights by the given factor"
        self.ptr().scaleW(scalefactor)

    def scaleX(self, factor):
        "Scale the x variable by the given factor"
        self.ptr().scaleX(factor)

    def scaleY(self, factor):
        "Scale the y variable by the given factor"
        self.ptr().scaleY(factor)

    @property
    def xMean(self):
        "Get the weighted mean x"
        return self.ptr().xMean()

    @property
    def yMean(self):
        "Get the weighted mean y"
        return self.ptr().yMean()

    @property
    def xVariance(self):
        "Get the weighted variance of x"
        return self.ptr().xVariance()

    @property
    def yVariance(self):
        "Get the weighted variance of y"
        return self.ptr().yVariance()

    @property
    def xStdDev(self):
        "Get the weighted standard deviation of x"
        return self.ptr().xStdDev()

    @property
    def yStdDev(self):
        "Get the weighted standard deviation of y"
        return self.ptr().yStdDev()

    @property
    def xStdErr(self):
        "Get the weighted standard error on <x>"
        return self.ptr().xStdDev()

    @property
    def yStdErr(self):
        "Get the weighted standard error on <y>"
        return self.ptr().yStdDev()

    @property
    def xRMS(self):
        "Get the weighted RMS of x"
        return self.ptr().xRMS()

    @property
    def yRMS(self):
        "Get the weighted RMS of y"
        return self.ptr().yRMS()

    @property
    def numEntries(self):
        "Get the number of entries"
        return self.ptr().numEntries()

    @property
    def effNumEntries(self):
        "Get the effective number of entries (for weighted events)"
        return self.ptr().effNumEntries()

    @property
    def sumW(self):
        "Get the sum of weights"
        return self.ptr().sumW()

    @property
    def sumW2(self):
        "Get the sum of squared-weights"
        return self.ptr().sumW2()

    @property
    def sumWX(self):
        "Get the sum of weighted xs"
        return self.ptr().sumWX()

    @property
    def sumWX2(self):
        "Get the sum of weighted squared-xs"
        return self.ptr().sumWX2()

    @property
    def sumWY(self):
        "Get the sum of weighted ys"
        return self.ptr().sumWY()

    @property
    def sumWY2(self):
        "Get the sum of weighted squared-ys"
        return self.ptr().sumWY2()

    @property
    def sumWXY(self):
        "Get the sum of weighted (x*y)s"
        return self.ptr().sumWXY()



cdef Dbn2D Dbn2D_fromptr(cDbn2D *ptr, dealloc=False):
    # Construct a Python Dbn2D from a pointer to a cDbn2D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn2D dbn = Dbn2D.__new__(Dbn2D)
    return dbn.setptr(ptr, dealloc)
