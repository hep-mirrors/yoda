cdef class Bin1D_${DBN}(Bin):
    """
    1D Bin based on an underlying ${DBN}.

    ProfileBin1D(xlow, xhigh)

    """

    cdef inline c.Bin1D_${DBN}* b1ptr(self) except NULL:
        return <c.Bin1D_${DBN}*> self.ptr()
    # TODO: remove
    cdef inline c.Bin1D_${DBN}* _Bin1D(self) except NULL:
        return <c.Bin1D_${DBN}*> self.ptr()


    def __init__(self, xlow, xhigh):
        cutil.set_owned_ptr(self, new c.Bin1D_${DBN}(pair[double, double](xlow, xhigh)))

    def __repr__(self):
        return '<%s x=[%g, %g)>' % (self.__class__.__name__, self.xMin, self.xMax)


    def scaleX(self, ax):
        """
        float -> None
        Scale this bin's x arguments by ax.
        """
        self.b1ptr().scaleX(ax)

    def scaleW(self, a):
        """
        float -> None
        Scale this bin's weights by a.
        """
        self.b1ptr().scaleW(a)


    @property
    def xMin(self):
        """The lower bin edge."""
        return self.b1ptr().xMin()

    @property
    def xMax(self):
        """The upper bin edge."""
        return self.b1ptr().xMax()


    @property
    def xEdges(self):
        """The lower and upper edges."""
        return (self.xMin, self.xMax)

    @property
    def xWidth(self):
        """The width of the bin."""
        return self.b1ptr().xWidth()

    @property
    def xMid(self):
        """The midpoint of the bin."""
        return self.b1ptr().xMid()


    @property
    def xMean(self):
        """The mean of the x-values that have filled the bin."""
        return self.b1ptr().xMean()

    @property
    def xFocus(self):
        """
        The focus of the bin. If the bin has been filled, then this
        is the mean fill on this bin. If the bin has not been filled,
        then the focus is the midpoint of the bin.
        """
        return self.b1ptr().xFocus()

    @property
    def xStdDev(self):
        """
        The standard deviation of the x-values that have filled the bin.
        """
        return self.b1ptr().xStdDev()

    @property
    def xStdErr(self):
        """
        The standard error of the x-values that have filled the bin.
        """
        return self.b1ptr().xStdErr()

    @property
    def xRMS(self):
        """
        The root-mean-square of the x-values that have filled the bin.
        """
        return self.b1ptr().xRMS()


    @property
    def numEntries(self):
        """
        The number of entries that have filled the bin.
        """
        return int(self.b1ptr().numEntries())

    @property
    def effNumEntries(self):
        """
        The effective number of entries in the bin.

        s.effNumEntries <==> (s.sumW ** 2) / s.sumW2
        """
        return self.b1ptr().effNumEntries()


    @property
    def sumW(self):
        """
        The sum of weights: sum(weights).
        """
        return self.b1ptr().sumW()

    @property
    def sumW2(self):
        """
        The sum of weights-squared: sum(weights * weights)
        """
        return self.b1ptr().sumW2()

    @property
    def sumWX(self):
        """
        The sum of weights-times-x: sum(weights * x)
        """
        return self.b1ptr().sumWX()

    @property
    def sumWX2(self):
        """
        The sum of weights-times-x-squared: sum(weights * x * x)
        """
        return self.b1ptr().sumWX2()


    def merge(Bin1D_${DBN} self, Bin1D_${DBN} other):
        """
        merge(other) -> Bin1D_${DBN}. Merge this bin with another of the
        same type. Only directly adjacent bins, i.e. those sharing a
        common edge, can be merged.
        """
        self.b1ptr().merge(deref(other.b1ptr()))
        return self


    def __add__(Bin1D_${DBN} self, Bin1D_${DBN} other):
        return cutil.new_owned_cls(Bin1D_${DBN},
                                   new c.Bin1D_${DBN}(deref(self.b1ptr()) + deref(other.b1ptr())))

    def __sub__(Bin1D_${DBN} self, Bin1D_${DBN} other):
        return cutil.new_owned_cls(Bin1D_${DBN},
                                   new c.Bin1D_${DBN}(deref(self.b1ptr()) - deref(other.b1ptr())))
