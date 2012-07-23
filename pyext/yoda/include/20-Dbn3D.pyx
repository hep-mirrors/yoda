cdef extern from "YODA/Dbn3D.h" namespace "YODA":
    cdef cppclass cDbn3D "Dbn3D":
        cDbn3D()
        cDbn3D(unsigned long numEntries, double sumW, double sumW2,
               double sumWX, double sumWX2, double sumWY, double sumWY2, double sumWZ, double sumWZ2,
               double sumWXY, double sumWXZ, double sumWYZ, double sumWXYZ)
        void fill(double valX, double valY, double valZ, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleX(double factor)
        void scaleY(double factor)
        void scaleZ(double factor)
        void scaleXY(double xfactor, double yfactor)
        void scaleXZ(double xfactor, double zfactor)
        void scaleYZ(double yfactor, double zfactor)
        void scaleXYZ(double xfactor, double yfactor, double zfactor)
        double xMean()
        double yMean()
        double zMean()
        double xVariance()
        double yVariance()
        double zVariance()
        double xStdDev()
        double yStdDev()
        double zStdDev()
        double xStdErr()
        double yStdErr()
        double zStdErr()
        double xRMS()
        double yRMS()
        double zRMS()
        unsigned long numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()
        double sumWY()
        double sumWY2()
        double sumWZ()
        double sumWZ2()
        double sumWXY()
        double sumWXZ()
        double sumWYZ()
        double sumWXYZ()
        # TODO: map operator+ and operator-, both internal and external


cdef class Dbn3D:
    """
    A 2D distribution 'counter', used and exposed by 1D histograms and their bins.

    TODO: Allow no-arg construction?
    TODO: map operator+ and operator-, both internal and external
    """

    cdef cDbn3D* thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr


    def __init__(self, *args, **kwargs):
        cdef:
            unsigned int numEntries
            double sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWZ, sumWZ2, sumWXY, sumWXZ, sumWYZ, sumWXYZ
        assert len(args) == 13
        numEntries, sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWZ, sumWZ2, sumWXY, sumWXZ, sumWYZ, sumWXYZ = args
        self.setptr(new cDbn3D(numEntries, sumW, sumW2, sumWX, sumWX2, sumWY, sumWY2, sumWZ, sumWZ2, sumWXY, sumWXZ, sumWYZ, sumWXYZ), True)


    cdef Dbn3D setptr(self, cDbn3D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cDbn3D* ptr(self):
        return self.thisptr


    def fill(self, double x, double y, double z, double weight=1.0):
        "Fill the distribution with the given z and weight at given x and y."
        self.ptr().fill(x, y, z, weight)

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

    def scaleZ(self, factor):
        "Scale the z variable by the given factor"
        self.ptr().scaleZ(factor)

    # TODO: map scaleXY etc. -- Pythonically, with list/tuple args?

    @property
    def xMean(self):
        "Get the weighted mean x"
        return self.ptr().xMean()

    @property
    def yMean(self):
        "Get the weighted mean y"
        return self.ptr().yMean()

    @property
    def zMean(self):
        "Get the weighted mean z"
        return self.ptr().zMean()

    @property
    def xVariance(self):
        "Get the weighted variance of x"
        return self.ptr().xVariance()

    @property
    def yVariance(self):
        "Get the weighted variance of y"
        return self.ptr().yVariance()

    @property
    def zVariance(self):
        "Get the weighted variance of z"
        return self.ptr().zVariance()

    @property
    def xStdDev(self):
        "Get the weighted standard deviation of x"
        return self.ptr().xStdDev()

    @property
    def yStdDev(self):
        "Get the weighted standard deviation of y"
        return self.ptr().yStdDev()

    @property
    def zStdDev(self):
        "Get the weighted standard deviation of z"
        return self.ptr().zStdDev()

    @property
    def xStdErr(self):
        "Get the weighted standard error on <x>"
        return self.ptr().xStdDev()

    @property
    def yStdErr(self):
        "Get the weighted standard error on <y>"
        return self.ptr().yStdDev()

    @property
    def zStdErr(self):
        "Get the weighted standard error on <z>"
        return self.ptr().zStdDev()

    @property
    def xRMS(self):
        "Get the weighted RMS of x"
        return self.ptr().xRMS()

    @property
    def yRMS(self):
        "Get the weighted RMS of y"
        return self.ptr().yRMS()

    @property
    def zRMS(self):
        "Get the weighted RMS of z"
        return self.ptr().zRMS()

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
    def sumWZ(self):
        "Get the sum of weighted zs"
        return self.ptr().sumWZ()

    @property
    def sumWZ2(self):
        "Get the sum of weighted squared-zs"
        return self.ptr().sumWZ2()

    @property
    def sumWXY(self):
        "Get the sum of weighted (x*y)s"
        return self.ptr().sumWXY()

    @property
    def sumWXZ(self):
        "Get the sum of weighted (x*z)s"
        return self.ptr().sumWXZ()

    @property
    def sumWYZ(self):
        "Get the sum of weighted (y*z)s"
        return self.ptr().sumWYZ()

    @property
    def sumWXYZ(self):
        "Get the sum of weighted (x*y*z)s"
        return self.ptr().sumWXYZ()


cdef Dbn3D Dbn3D_fromptr(cDbn3D *ptr, dealloc=False):
    # Construct a Python Dbn3D from a pointer to a cDbn3D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn3D dbn = Dbn3D.__new__(Dbn3D)
    return dbn.setptr(ptr, dealloc)
