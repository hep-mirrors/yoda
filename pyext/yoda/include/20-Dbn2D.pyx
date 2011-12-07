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
    cdef cDbn2D *thisptr
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
        self.ptr().fill(x, y, weight)

    def reset(self):
        self.ptr().reset()

    def scaleW(self, scalefactor):
        self.ptr().scaleW(scalefactor)

    def scaleX(self, factor):
        self.ptr().scaleX(factor)

    def scaleY(self, factor):
        self.ptr().scaleY(factor)

    def xMean(self):
        return self.ptr().xMean()

    def yMean(self):
        return self.ptr().yMean()

    def xVariance(self):
        return self.ptr().xVariance()

    def yVariance(self):
        return self.ptr().yVariance()

    def xStdDev(self):
        return self.ptr().xStdDev()

    def yStdDev(self):
        return self.ptr().yStdDev()

    def xStdErr(self):
        return self.ptr().xStdDev()

    def yStdErr(self):
        return self.ptr().yStdDev()

    def xRMS(self):
        return self.ptr().xRMS()

    def yRMS(self):
        return self.ptr().yRMS()

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

    def sumWY(self):
        return self.ptr().sumWY()

    def sumWY2(self):
        return self.ptr().sumWY2()

    def sumWXY(self):
        return self.ptr().sumWXY()

    # TODO: map operator+ and operator-, both internal and external



cdef Dbn2D Dbn2D_fromptr(cDbn2D *ptr, dealloc=False):
    # Construct a Python Dbn2D from a pointer to a cDbn2D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn2D dbn = Dbn2D.__new__(Dbn2D)
    return dbn.setptr(ptr, dealloc)
