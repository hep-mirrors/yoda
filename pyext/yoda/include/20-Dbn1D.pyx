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
        self.ptr().fill(x, weight)

    def reset(self):
        self.ptr().reset()

    def scaleW(self, scalefactor):
        self.ptr().scaleW(scalefactor)

    def scaleX(self, factor):
        self.ptr().scaleX(factor)

    def mean(self):
        return self.ptr().mean()

    def xMean(self):
        return self.ptr().xMean()

    def variance(self):
        return self.ptr().variance()

    def xVariance(self):
        return self.ptr().xVariance()

    def stdDev(self):
        return self.ptr().stdDev()

    def xStdDev(self):
        return self.ptr().xStdDev()

    def stdErr(self):
        return self.ptr().stdDev()

    def xStdErr(self):
        return self.ptr().xStdDev()

    def rms(self):
        return self.ptr().rms()

    def xRMS(self):
        return self.ptr().xRMS()

    def numEntries(self):
        return self.ptr().numEntries()

    def effNumEntries(self):
        return self.ptr().effNumEntries()

    def sumW(self):
        return self.ptr().sumW()

    def sumW2(self):
        return self.ptr().sumW2()

    def sumWX(self):
        return self.ptr().sumWX()

    def sumWX2(self):
        return self.ptr().sumWX2()

    # TODO: map operator+ and operator-, both internal and external



cdef Dbn1D Dbn1D_fromptr(cDbn1D *ptr, dealloc=False):
    # Construct a Python Dbn1D from a pointer to a cDbn1D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn1D dbn = Dbn1D.__new__(Dbn1D)
    return dbn.setptr(ptr, dealloc)
