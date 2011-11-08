cdef extern from "YODA/HistoBin2D.h" namespace "YODA":

    #cHistoBin2D operator + (cHistoBin2D &, cHistoBin2D &)
    #cHistoBin2D operator - (cHistoBin2D &, cHistoBin2D &)

    cdef cppclass cHistoBin2D "YODA::HistoBin2D":
        cHistoBin2D (cHistoBin2D &h)

        double volume()
        double volumeErr()
        double height()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
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

        double numEntries()
        # double effNumEntries()

        double sumW()
        double sumW2()

        double sumWX()
        double sumWY()
        double sumWXY()
        double sumWX2()
        double sumWY2()

cdef class HistoBin2D:
    cdef cHistoBin2D *thisptr

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef HistoBin2D setptr(self, cHistoBin2D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef set(self, cHistoBin2D ptr):
        self.thisptr = new cHistoBin2D(ptr)
        return self

    cdef cHistoBin2D* ptr(self):
        return self.thisptr

    @property
    def volume(self):
        return self.ptr().volume()

    @property
    def volumeErr(self):
        return self.ptr().volumeErr()

    @property
    def height(self):
        return self.ptr().height()

    @property
    def heightErr(self):
        return self.ptr().heightErr()

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

    def __repr__(self):
        return 'HistoBin2D(%r)' % self.volume

cdef HistoBin2D HistoBin2D_fromptr(cHistoBin2D *ptr, dealloc=False):
    # Construct a Python HistoBin2D from a pointer to a cHistoBin2D,
    # without calling __init__ and excessive memory allocation
    cdef HistoBin2D bin = HistoBin2D.__new__(HistoBin2D)
    return bin.setptr(ptr, dealloc)
