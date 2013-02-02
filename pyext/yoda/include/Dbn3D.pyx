cdef class Dbn3D(util.Base):
    """
    A 2D distribution 'counter', used and exposed by 2D histograms and
    1D profiles and their bins.

    """
    def __init__(self):
        util.set_owned_ptr(self, new c.Dbn3D())

    def copy(self):
        return util.new_owned_cls(Dbn3D, new c.Dbn3D(deref(self._Dbn3D())))

    def fill(self, x, y, z, weight=1.0):
        """
        (x, y, z, weight=1.0) -> None

        Fills the distribution with the given weight at given (x, y).

        """
        self._Dbn3D().fill(x, y, z, weight)

    def reset(self):
        """
        () -> None

        Reset the distribution counters to the unfilled state."""
        self._Dbn3D().reset()

    def scale(self, x=1.0, y=1.0, z=1.0, w=1.0):
        """
        (x=1.0, y=1.0, z=1.0, w=1.0) -> None

        Scale Dbn3D's x, y, z and/or weight parameters.

        """
        if x != 1.0 and y != 1.0 and z != 1.0:
            self._Dbn3D().scaleXYZ(x, y, z)
        else:
            if x != 1.0:
                self._Dbn3D().scaleX(x)
            if y != 1.0:
                self._Dbn3D().scaleY(x)
            if z != 1.0:
                self._Dbn3D().scaleZ(x)

        if w != 1.0:
            self._Dbn3D().scaleW(w)

    @property
    def mean(self):
        """Weighted mean of x"""
        return util.XYZ(self._Dbn3D().xMean(),
                        self._Dbn3D().yMean(),
                        self._Dbn3D().zMean())

    @property
    def variance(self):
        """Weighted variance of x"""
        return util.XYZ(self._Dbn3D().xVariance(),
                        self._Dbn3D().yVariance(),
                        self._Dbn3D().zVariance())

    @property
    def stdDev(self):
        """Weighted standard deviation of x"""
        return util.XYZ(self._Dbn3D().xStdDev(),
                        self._Dbn3D().yStdDev(),
                        self._Dbn3D().zStdDev())

    @property
    def stdErr(self):
        """Weighted standard error on <x>"""
        return util.XYZ(self._Dbn3D().xStdErr(),
                        self._Dbn3D().yStdErr(),
                        self._Dbn3D().zStdErr())

    @property
    def rms(self):
        """Weighted root mean squared (RMS) of x"""
        return util.XYZ(self._Dbn3D().xRMS(),
                        self._Dbn3D().yRMS(),
                        self._Dbn3D().zRMS())

    @property
    def numEntries(self):
        """The number of entries"""
        return self._Dbn3D().numEntries()

    @property
    def effNumEntries(self):
        """Effective number of entries (for weighted events)"""
        return self._Dbn3D().effNumEntries()

    @property
    def sumW(self):
        """sum(weights)"""
        return self._Dbn3D().sumW()

    @property
    def sumW2(self):
        """sum(weights * weights)"""
        return self._Dbn3D().sumW2()

    @property
    def sumWX(self):
        """sum(weights * xs)"""
        return self._Dbn3D().sumWX()

    @property
    def sumWY(self):
        """sum(weights * ys)"""
        return self._Dbn3D().sumWY()

    @property
    def sumWZ(self):
        """sum(weights * zs)"""
        return self._Dbn3D().sumWZ()

    @property
    def sumWX2(self):
        """sum(weights * xs * xs)"""
        return self._Dbn3D().sumWX2()

    @property
    def sumWY2(self):
        """sum(weights * ys * ys)"""
        return self._Dbn3D().sumWY2()

    @property
    def sumWZ2(self):
        """sum(weights * zs * zs")"""
        return self._Dbn3D().sumWZ2()

    @property
    def sumWXY(self):
        """sum(weights * xs * ys)"""
        return self._Dbn3D().sumWXY()

    @property
    def sumWXZ(self):
        """sum(weights * xs * zs)"""
        return self._Dbn3D().sumWXZ()

    @property
    def sumWYZ(self):
        """sum(weights * ys * zs)"""
        return self._Dbn3D().sumWYZ()

    def __add__(Dbn3D self, Dbn3D other):
        return util.new_owned_cls(Dbn3D, new c.Dbn3D(
            deref(self._Dbn3D()) + deref(other._Dbn3D())))

    def __sub__(Dbn3D self, Dbn3D other):
        return util.new_owned_cls(Dbn3D, new c.Dbn3D(
            deref(self._Dbn3D()) - deref(other._Dbn3D())))

    def __repr__(self):
        return 'Dbn3D(mean=(%g, %g), stdDev=(%g, %g))' % (self.mean + self.std_dev)

    # Magic stuff
    cdef c.Dbn3D *_Dbn3D(self) except NULL:
        return <c.Dbn3D *> self.ptr()

    def __dealloc__(self):
        cdef c.Dbn3D *p = self._Dbn3D()
        if self._deallocate:
            del p

