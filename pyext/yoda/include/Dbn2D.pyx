
cdef class Dbn2D(util.Base):
    """
    A 2D distribution 'counter', used and exposed by 2D histograms and
    1D profiles and their bins.

    """

    def __init__(self):
        util.set_owned_ptr(self, new c.Dbn2D())

    def copy(self):
        return util.new_owned_cls(Dbn2D, new c.Dbn2D(deref(self._Dbn2D())))

    def fill(self, x, y, weight=1.0):
        """
        (x, y, weight=1.0) -> None

        Fills the distribution with the given weight at given (x, y).
        
        """
        self._Dbn2D().fill(x, y, weight)

    def fill_many(self, xs, ys, ws=repeat(1)):
        """
        (xs, ys, ws=repeat(1.0)) -> None

        Fills the distribution from iterables xs, ys and ws.

        """
        cdef c.Dbn2D *ptr = self._Dbn2D()
        itx = iter(xs)
        ity = iter(ys)
        itw = iter(ws)
        while True:
            x = next(xs, None)
            y = next(ys, None)
            w = next(ws, None)
            if x is None or y is None or w is None:
                break
            ptr.fill(x, y, w)

    def reset(self):
        """
        () -> None

        Reset the distribution counters to the unfilled state."""
        self._Dbn2D().reset()

    def scale(self, x=1.0, y=1.0, w=1.0):
        """
        (x=1.0, y=1.0, w=1.0) -> None

        Scale Dbn2D's parameters

        """
        if x != 1.0 and y != 1.0:
            self._Dbn2D().scaleXY(x, y)
        elif x != 1.0:
            self._Dbn2D().scaleX(x)
        elif y != 1.0:
            self._Dbn2D().scaleY(y)
        if w != 1.0:
            self._Dbn2D().scaleW(w)

    @property
    def mean(self):
        """Weighted mean of x"""
        return util.XY(self._Dbn2D().xMean(), self._Dbn2D().yMean())

    @property
    def variance(self):
        """Weighted variance of x"""
        return util.XY(self._Dbn2D().xVariance(), self._Dbn2D().yVariance())

    @property
    def std_dev(self):
        """Weighted standard deviation of x"""
        return util.XY(self._Dbn2D().xStdDev(), self._Dbn2D().yStdDev())

    @property
    def std_err(self):
        """Weighted standard error on <x>"""
        return util.XY(self._Dbn2D().xStdErr(), self._Dbn2D().yStdErr())

    @property
    def rms(self):
        """Weighted root mean squared (RMS) of x"""
        return util.XY(self._Dbn2D().xRMS(), self._Dbn2D().yRMS())

    @property
    def count(self):
        """The number of entries"""
        return self._Dbn2D().numEntries()

    @property
    def effective_count(self):
        """Effective number of entries (for weighted events)"""
        return self._Dbn2D().effNumEntries()

    @property
    def sum_w(self):
        """sum(weights)"""
        return self._Dbn2D().sumW()

    @property
    def sum_w2(self):
        """sum(weights**2)"""
        return self._Dbn2D().sumW2()

    @property
    def sum_wx(self):
        """sum(weights * xs)"""
        return self._Dbn2D().sumWX()

    @property
    def sum_wy(self):
        """sum(weights * ys)"""
        return self._Dbn2D().sumWY()

    @property
    def sum_wx2(self):
        """sum(weights * xs**2)"""
        return self._Dbn2D().sumWX2()

    @property
    def sum_wy2(self):
        """sum(weights * ys**2)"""
        return self._Dbn2D().sumWY2()

    @property
    def sum_wxy(self):
        """sum(weights xs * ys)"""
        return self._Dbn2D().sumWXY()

    def __add__(Dbn2D self, Dbn2D other):
        return util.new_owned_cls(Dbn2D, new c.Dbn2D(
            deref(self._Dbn2D()) + deref(other._Dbn2D())))

    def __sub__(Dbn2D self, Dbn2D other):
        return util.new_owned_cls(Dbn2D, new c.Dbn2D(
            deref(self._Dbn2D()) - deref(other._Dbn2D())))

    def __repr__(self):
        return 'Dbn2D(mean=(%g, %g), stdDev=(%g, %g))' % (self.mean + self.std_dev)

    # Magic stuff
    cdef c.Dbn2D *_Dbn2D(self) except NULL:
        return <c.Dbn2D *> self.ptr()

    def __dealloc__(self):
        cdef c.Dbn2D *p = self._Dbn2D()
        if self._deallocate:
            del p

