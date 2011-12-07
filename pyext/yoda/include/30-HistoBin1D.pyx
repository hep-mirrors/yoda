cdef extern from "YODA/HistoBin1D.h" namespace "YODA":

    cdef cppclass cHistoBin1D "YODA::HistoBin1D":
        cHistoBin1D (cHistoBin1D &h)
        cHistoBin1D (double, double)
        double area()
        double height()
        double areaErr()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
        # way to make Cython acknowledge these (it seems to be a typedef parsing
        # issue rather than a technical issue).

        void scaleW(double factor)
        void scaleX(double factor)

        double lowEdge()
        double highEdge()
        double width()
        double focus()
        double midpoint()
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()
        double numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()


#Ugly hack using shim header for Cython 0.13
cdef extern from "shims.h":
    cHistoBin1D add_HistoBin1D (cHistoBin1D &, cHistoBin1D &)
    cHistoBin1D subtract_HistoBin1D (cHistoBin1D &, cHistoBin1D &)


cdef class HistoBin1D:
    cdef cHistoBin1D *thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef HistoBin1D setptr(self, cHistoBin1D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cHistoBin1D* ptr(self):
        return self.thisptr

    @property
    def lowEdge(self):
        """The lower of the two bin edges."""
        return self.ptr().lowEdge()

    xMin = lowEdge

    @property
    def highEdge(self):
        """The higher of the two bin edges."""
        return self.ptr().highEdge()

    xMax = highEdge

    @property
    def width(self):
        """The width of the bin."""
        return self.ptr().width()

    @property
    def focus(self):
        """The focus of the bin."""
        return self.ptr().focus()

    @property
    def midpoint(self):
        """The point half-way between the bin edges."""
        return self.ptr().midpoint()

    @property
    def xMean(self):
        """The mean of the x-values that have filled the bin."""
        return self.ptr().xMean()

    @property
    def xVariance(self):
        """The variance of the x-values that have filled the bin."""
        return self.ptr().xVariance()

    @property
    def xStdDev(self):
        """The standard deviation of the x-values that have filled the bin."""
        return self.ptr().xStdDev()

    @property
    def numEntries(self):
        """The number of entries in the bin."""
        return self.ptr().numEntries()

    @property
    def effNumEntries(self):
        """The effective number of entries in the bin. This is
        (s.sumW)**2 / s.sumW2"""
        return self.ptr().effNumEntries()

    @property
    def sumW(self):
        """Sum of weights."""
        return self.ptr().sumW()

    @property
    def sumW2(self):
        """Sum of weights squared."""
        return self.ptr().sumW2()

    @property
    def sumWX(self):
        """Sum of the products of x-values and their weights."""
        return self.ptr().sumWX()

    @property
    def sumWX2(self):
        """Sum of the products of x-values squared and their weights."""
        return self.ptr().sumWX2()

    @property
    def area(self):
        """
        b.area <==> b.sumW

        The area of the bin is the sum of weights of the bin; it is
        independent of width.

        """
        return self.ptr().area()

    @property
    def height(self):
        """
        b.height <==> b.area / b.width

        The height of the bin is defined as the area divided by the
        width.

        """
        return self.ptr().height()

    @property
    def areaErr(self):
        """
        Error computed using binomial statistics on squared sum of bin weights,
        i.e. s.areaErr = sqrt(s.sumW2)

        """
        return self.ptr().areaErr()

    @property
    def heightErr(self):
        """
        Height error - scales the s.areaError by the reciprocal of the
        bin width.

        """
        return self.ptr().heightErr()


    def scaleX(self, double factor):
        """
        bin.scaleX(factor) -> bin

        Scale the x-axis of `bin` in-place by `factor`.

        """
        self.ptr().scaleX(factor)

    def scaleW(self, double factor):
        """
        bin.scaleW(factor) -> bin

        Scale `bin` in-place as if all weights were scaled by given `factor`

        """
        self.ptr().scaleX(factor)


    def __repr__(self):
        return 'HistoBin1D(%r)' % self.area


cdef HistoBin1D HistoBin1D_fromptr(cHistoBin1D *ptr, dealloc=False):
    # Construct a Python HistoBin1D from a pointer to a cHistoBin1D,
    # without calling __init__ and excessive memory allocation
    cdef HistoBin1D bin = HistoBin1D.__new__(HistoBin1D)
    return bin.setptr(ptr, dealloc)
