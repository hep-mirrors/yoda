cdef extern from "YODA/ProfileBin2D.h" namespace "YODA":

    #cProfileBin2D operator + (cProfileBin2D &, cProfileBin2D &)
    #cProfileBin2D operator - (cProfileBin2D &, cProfileBin2D &)

    cdef cppclass cProfileBin2D "YODA::ProfileBin2D":
        cProfileBin2D (cProfileBin2D &h)

        void reset()

        # These are inherited methods from Bin2D... I can't seem to find a nice
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

        double stdErr()
        double mean()
        double variance()
        double stdDev()

        double numEntries()
        # double effNumEntries()

        double sumW()
        double sumW2()

        double sumWX()
        double sumWY()
        double sumWXY()
        double sumWX2()
        double sumWY2()

        double sumWZ()
        double sumWZ2()


cdef class ProfileBin2D:
    cdef cProfileBin2D *thisptr

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef ProfileBin2D setptr(self, cProfileBin2D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef set(self, cProfileBin2D ptr):
        self.thisptr = new cProfileBin2D(ptr)
        return self

    cdef cProfileBin2D* ptr(self):
        return self.thisptr


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

    @property
    def sumWXY(self):
        return self.ptr().sumWXY()

    @property
    def sumWZ(self):
        return self.ptr().sumWZ()

    @property
    def sumWZ2(self):
        return self.ptr().sumWZ2()


    @property
    def mean(self):
        return self.ptr().mean()

    @property
    def variance(self):
        return self.ptr().variance()

    @property
    def stdDev(self):
        return self.ptr().stdDev()

    @property
    def stdErr(self):
        return self.ptr().stdErr()


    def __repr__(self):
        return 'ProfileBin2D(%r)' % self.volume
