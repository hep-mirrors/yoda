cdef extern from "YODA/ProfileBin1D.h" namespace "YODA":

    cdef cppclass cProfileBin1D "YODA::ProfileBin1D":
        cProfileBin1D (cProfileBin1D &h)
        cProfileBin1D (double, double)
        double area()
        double height()
        double areaErr()
        double heightErr()
        void reset()

        # TODO
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
        double xRMS()
        double mean()
        double variance()
        double stdDev()
        double stdErr()
        double rms()
        double numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()
        double sumWY()
        double sumWY2()


#Ugly hack using shim header for Cython 0.13
cdef extern from "shims.h":
    cProfileBin1D add_ProfileBin1D (cProfileBin1D &, cProfileBin1D &)
    cProfileBin1D subtract_ProfileBin1D (cProfileBin1D &, cProfileBin1D &)


cdef class ProfileBin1D:
    """
    A 1D profile histogram bin, as stored inside Histo1D.

    Only one Python constructor:

    * ProfileBin1D(xlow, xhigh)
    """

    cdef cProfileBin1D* thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr


    # TODO: Why does Cython hate this?!?
    # def __init__(self, double xlow, double xhigh):
    #     self.setptr(new cHistoBin1D(xlow, xhigh), dealloc=True)


    cdef ProfileBin1D setptr(self, cProfileBin1D* ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cProfileBin1D* ptr(self):
        return self.thisptr


    ##############################
    # THIS IS ALL BIN1D STUFF: CAN WE INHERIT THE PY MAPPING?

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
    def xStdErr(self):
        """The standard error of the x-values that have filled the bin."""
        return self.ptr().xStdErr()

    @property
    def xRMS(self):
        """The RMS of the x-values that have filled the bin."""
        return self.ptr().xRMS()

    ####################################


    @property
    def mean(self):
        """The mean of the y-values that have filled the bin."""
        return self.ptr().mean()

    @property
    def variance(self):
        """The variance of the y-values that have filled the bin."""
        return self.ptr().variance()

    @property
    def stdDev(self):
        """The standard deviation of the y-values that have filled the bin."""
        return self.ptr().stdDev()

    @property
    def stdErr(self):
        """The standard error of the y-values that have filled the bin."""
        return self.ptr().stdErr()

    @property
    def rms(self):
        """The RMS of the y-values that have filled the bin."""
        return self.ptr().rms()

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
    def sumWY(self):
        """Sum of the products of y-values and their weights."""
        return self.ptr().sumWX()

    @property
    def sumWY2(self):
        """Sum of the products of y-values squared and their weights."""
        return self.ptr().sumWX2()


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
        return 'ProfileBin1D(%r)' % self.area


cdef ProfileBin1D ProfileBin1D_fromptr(cProfileBin1D *ptr, dealloc=False):
    # Construct a Python ProfileBin1D from a pointer to a cProfileBin1D,
    # without calling __init__ and excessive memory allocation
    cdef ProfileBin1D bin = ProfileBin1D.__new__(ProfileBin1D)
    return bin.setptr(ptr, dealloc)
