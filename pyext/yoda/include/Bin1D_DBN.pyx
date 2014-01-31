cdef class Bin1D_${DBN}(Bin):
    """
    1D Bin based on an underlying ${DBN}.

    ProfileBin1D(xlow, xhigh)

    """

    def __init__(self, xlow, xhigh):
        util.set_owned_ptr(
            self, new c.Bin1D_${DBN}(pair[double, double](xlow, xhigh)))

    def scale(self, x=1.0, w=1.0):
        """
        scale(x=1.0, w=1.0) -> None. Scale this bin's arguments by
        their respective scalefactors.

        """
        if w != 1.0:
            self._Bin1D().scaleW(w)
        if x != 1.0:
            self._Bin1D().scaleX(x)

    @property
    def lowEdge(self):
        """The lower bin edge."""
        return self._Bin1D().lowEdge()

    @property
    def xMin(self):
        """Synonym for lowEdge."""
        return self.lowEdge

    @property
    def highEdge(self):
        """The upper bin edge."""
        return self._Bin1D().highEdge()

    @property
    def xMax(self):
        """Synonym for highEdge."""
        return self.highEdge

    @property
    def edges(self):
        """The lower and upper edges."""
        return (self.lowEdge, self.highEdge)

    @property
    def width(self):
        """The width of the bin."""
        return self._Bin1D().width()

    @property
    def focus(self):
        """
        The focus of the bin. If the bin has been filled, then this
        is the mean fill on this bin. If the bin has not been filled,
        then the focus is the midpoint of the bin.

        """
        return self._Bin1D().focus()

    @property
    def midpoint(self):
        """The point half-way between the two bin edges."""
        return self._Bin1D().midpoint()

    @property
    def xMid(self):
        """Synonym for midpoint."""
        return self.midpoint

    @property
    def mean(self):
        """The mean of the x-values that have filled the bin."""
        return self._Bin1D().xMean()

    @property
    def stdDev(self):
        """
        The standard deviation of the x-values that have filled the bin.

        """
        return self._Bin1D().xStdDev()

    @property
    def stdErr(self):
        """
        The standard error of the x-values that have filled the bin.

        """
        return self._Bin1D().xStdErr()

    @property
    def rms(self):
        """
        The root-mean-square of the x-values that have filled the bin.

        """
        return self._Bin1D().xRMS()

    ##
    ## Raw statistics
    ##

    @property
    def numEntries(self):
        """
        The number of entries that have filled the bin.

        """
        return self._Bin1D().numEntries()

    @property
    def effNumEntries(self):
        """
        The effective number of entries in the bin.

        s.effNumEntries <==> (s.sumW ** 2) / s.sumW2

        """
        return self._Bin1D().effNumEntries()

    @property
    def sumW(self):
        """
        The sum of weights: sum(weights).

        """
        return self._Bin1D().sumW()

    @property
    def sumW2(self):
        """
        The sum of weights-squared: sum(weights * weights)

        """
        return self._Bin1D().sumW2()

    @property
    def sumWX(self):
        """
        The sum of weights-times-x: sum(weights * x)

        """
        return self._Bin1D().sumWX()

    @property
    def sumWX2(self):
        """
        The sum of weights-times-x-squared: sum(weights * x * x)

        """
        return self._Bin1D().sumWX2()

    def merge(Bin1D_${DBN} self, Bin1D_${DBN} other):
        """
        merge(other) -> Bin1D_${DBN}. Merge this bin with another of the
        same type. Only directly adjacent bins, i.e. those sharing a
        common edge, can be merged.

        """
        self._Bin1D().merge(deref(other._Bin1D()))
        return self

    def __repr__(self):
        return '<%s[%g, %g)>' % ((self.__class__.__name__,) + self.edges)

    def __add__(Bin1D_${DBN} self, Bin1D_${DBN} other):
        return util.new_owned_cls(
            Bin1D_${DBN},
            new c.Bin1D_${DBN}(deref(self._Bin1D()) + deref(other._Bin1D())))

    def __sub__(Bin1D_${DBN} self, Bin1D_${DBN} other):
        return util.new_owned_cls(
            Bin1D_${DBN},
            new c.Bin1D_${DBN}(deref(self._Bin1D()) - deref(other._Bin1D())))

    cdef inline c.Bin1D_${DBN} *_Bin1D(self) except NULL:
        return <c.Bin1D_${DBN} *> self.ptr()
