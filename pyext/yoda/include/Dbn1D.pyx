cdef class Dbn1D(util.Base):
    """
    A 1D distribution 'counter', used and exposed by 1D histograms and their bins.

    """

    def __init__(self):
        util.set_owned_ptr(self, new c.Dbn1D())

    def copy(self):
        return util.set_owned_ptr(self, new c.Dbn1D(deref(self._Dbn1D())))

    def fill(self, x, weight=1.0):
        """
        (float x, float weight=1.0) -> None
        
        Fills the distribution with the given weight at given x.
        
        """
        self._Dbn1D().fill(x, weight)

    def reset(self):
        """
        () -> None

        Reset the distribution counters to the unfilled state."""
        self._Dbn1D().reset()


    def scale(self, x=1.0, w=1.0):
        """
        (x=1.0, w=1.0) -> None

        Scale the Dbn's variables by the given factors

        """
        if x != 1.0:
            self._Dbn1D().scaleX(x)
        if w != 1.0:
            self._Dbn1D().scaleW(w)

    @property
    def mean(self):
        """Weighted mean of x"""
        return (self._Dbn1D().xMean())

    @property
    def variance(self):
        """Weighted variance of x"""
        return (self._Dbn1D().xVariance())

    @property
    def stdDev(self):
        """Weighted standard deviation of x"""
        return (self._Dbn1D().xStdDev())

    @property
    def stdErr(self):
        """Weighted standard error on <x>"""
        return (self._Dbn1D().xStdErr())

    @property
    def rms(self):
        """Weighted root mean squared (RMS) of x"""
        return (self._Dbn1D().xRMS())

    @property
    def numEntries(self):
        """The number of entries"""
        return self._Dbn1D().numEntries()

    @property
    def effNumEntries(self):
        """Effective number of entries (for weighted events)"""
        return self._Dbn1D().effNumEntries()

    @property
    def sumW(self):
        """sum(weights)"""
        return self._Dbn1D().sumW()

    @property
    def sumW2(self):
        """sum(weights * weights)"""
        return self._Dbn1D().sumW2()

    @property
    def sumWX(self):
        """sum(weights * xs)"""
        return self._Dbn1D().sumWX()

    @property
    def sumWX2(self):
        """sum(weights * xs * xs)"""
        return self._Dbn1D().sumWX2()

    def __add__(Dbn1D self, Dbn1D other):
        return util.new_owned_cls(Dbn1D, new c.Dbn1D(
            deref(self._Dbn1D()) + deref(other._Dbn1D())))

    def __sub__(Dbn1D self, Dbn1D other):
        return util.new_owned_cls(Dbn1D, new c.Dbn1D(
            deref(self._Dbn1D()) - deref(other._Dbn1D())))

    def __repr__(self):
        return '<Dbn1D(mean=%g, stdDev=%g)>' % (self.mean, self.stdDev)

    # Magic stuff
    cdef c.Dbn1D *_Dbn1D(self) except NULL:
        return <c.Dbn1D *> self.ptr()

    def __dealloc__(self):
        cdef c.Dbn1D *p = self._Dbn1D()
        if self._deallocate:
            del p

