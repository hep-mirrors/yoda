cdef class ProfileBin1D(Bin1D_Dbn2D):
    """
    A 1D profile bin, as stored inside Profile1D.

    Only one constructor:

    * ProfileBin1D(xlow, xhigh)
    """

    def __init__(self, double a, double b):
        util.set_owned_ptr(
            self, new c.ProfileBin1D(a, b))

    def fill(self, x, y, weight=1.0):
        """
        (x, y, weight=1.0) -> None. Fill this bin with given values and weight.

        """
        self._ProfileBin1D().fill(x, y, weight)

    def fillBin(self, y, weight=1.0):
        """
        (y, weight=1.0) -> None. Fill this bin with given y-value and weight.

        """
        self._ProfileBin1D().fillBin(y, weight)

    def scale(self, x=1.0, w=1.0):
        """
        (x=1.0, w=1.0) -> None
        
        Scale values by given coefficients.
        """
        if x != 1.0:
            self._ProfileBin1D().scaleX(x)
        if w != 1.0:
            self._ProfileBin1D().scaleW(w)

    @property
    def mean(self):
        """The mean of the y-values that have filled the bin."""
        return self._ProfileBin1D().mean()

    @property
    def variance(self):
        """The variance of the y-values that have filled the bin."""
        return self._ProfileBin1D().variance()

    @property
    def stdDev(self):
        """The standard deviation of the y-values that have filled the bin."""
        return self._ProfileBin1D().stdDev()

    @property
    def stdErr(self):
        """The standard error of the y-values that have filled the bin."""
        return self._ProfileBin1D().stdErr()

    @property
    def rms(self):
        """The RMS of the y-values that have filled the bin."""
        return self._ProfileBin1D().rms()

    @property
    def sumWY(self):
        """sum(weights * ys)"""
        return self._ProfileBin1D().sumWY()

    @property
    def sumWY2(self):
        """sum(weights * ys * ys)"""
        return self._ProfileBin1D().sumWY2()

    def __add__(ProfileBin1D a, ProfileBin1D b):
        return util.new_owned_cls(
            ProfileBin1D,
            new c.ProfileBin1D(
                deref(a._ProfileBin1D()) +
                deref(b._ProfileBin1D())))

    def __iadd__(ProfileBin1D self, ProfileBin1D other):
        c.ProfileBin1D_iadd_ProfileBin1D(self._ProfileBin1D(), other._ProfileBin1D())
        return self
    def __isub__(ProfileBin1D self, ProfileBin1D other):
        c.ProfileBin1D_isub_ProfileBin1D(self._ProfileBin1D(), other._ProfileBin1D())
        return self

    def __sub__(ProfileBin1D a, ProfileBin1D b):
        return util.new_owned_cls(
            ProfileBin1D,
            new c.ProfileBin1D(
                deref(a._ProfileBin1D()) -
                deref(b._ProfileBin1D())))

    cdef inline c.ProfileBin1D *_ProfileBin1D(self) except NULL:
        return <c.ProfileBin1D *> self.ptr()

