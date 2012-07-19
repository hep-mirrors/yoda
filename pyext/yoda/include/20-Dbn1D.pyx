cdef extern from "YODA/Dbn1D.h" namespace "YODA":
    cdef cppclass cDbn1D "Dbn1D":
        cDbn1D()
        cDbn1D(unsigned int numEntries, double sumW, double sumW2, double sumWX, double sumWX2)
        void fill(double val, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleX(double factor)
        double mean()
        double xMean()
        double variance()
        double xVariance()
        double stdDev()
        double xStdDev()
        double stdErr()
        double xStdErr()
        double rms()
        double xRMS()
        unsigned long numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()
        # TODO: map operator+ and operator-, both internal and external


cdef class Dbn1D:
    """
    A 1D distribution 'counter', used and exposed by 1D histograms and their bins.

    TODO: Allow no-arg construction?
    TODO: map operator+ and operator-, both internal and external
    """

    cdef cDbn1D *thisptr
    cdef bool _dealloc


    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr


    def __init__(self, *args, **kwargs):
        cdef:
            unsigned int numEntries
            double sumW, sumW2, sumWX, sumWX2
        assert len(args) == 5
        numEntries, sumW, sumW2, sumWX, sumWX2 = args
        self.setptr(new cDbn1D(numEntries, sumW, sumW2, sumWX, sumWX2), True)


    cdef Dbn1D setptr(self, cDbn1D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cDbn1D* ptr(self):
        return self.thisptr


    def fill(self, double x, double weight=1.0):
        "Fill the distribution with the given weight at given x."
        self.ptr().fill(x, weight)

    def reset(self):
        "Reset the distribution counters to the unfilled state."
        self.ptr().reset()

    def scaleW(self, scalefactor):
        "Scale the fill weights by the given factor"
        self.ptr().scaleW(scalefactor)

    def scaleX(self, factor):
        "Scale the x variable by the given factor"
        self.ptr().scaleX(factor)

    @property
    def mean(self):
        "Get the weighted mean x (synonym for xMean)"
        return self.ptr().mean()

    @property
    def xMean(self):
        "Get the weighted mean x"
        return self.ptr().xMean()

    @property
    def variance(self):
        "Get the weighted variance of x (synonym for xVariance)"
        return self.ptr().variance()

    @property
    def xVariance(self):
        "Get the weighted variance of x"
        return self.ptr().xVariance()

    @property
    def stdDev(self):
        "Get the weighted standard deviation of x (synonym for xStdDev)"
        return self.ptr().stdDev()

    @property
    def xStdDev(self):
        "Get the weighted standard deviation of x"
        return self.ptr().xStdDev()

    @property
    def stdErr(self):
        "Get the weighted standard error on <x> (synonym for xStdErr)"
        return self.ptr().stdDev()

    @property
    def xStdErr(self):
        "Get the weighted standard error on <x>"
        return self.ptr().xStdDev()

    @property
    def rms(self):
        "Get the weighted RMS of x (synonym for xRMS)"
        return self.ptr().rms()

    @property
    def xRMS(self):
        "Get the weighted RMS of x"
        return self.ptr().xRMS()

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



cdef Dbn1D Dbn1D_fromptr(cDbn1D *ptr, dealloc=False):
    # Construct a Python Dbn1D from a pointer to a cDbn1D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn1D dbn = Dbn1D.__new__(Dbn1D)
    return dbn.setptr(ptr, dealloc)
