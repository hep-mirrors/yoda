# TODO: docstrings
cdef class Bin2D_${DBN}(Bin):
    """2D Bin class templated on a ${DBN}"""

    cdef inline c.Bin2D_${DBN}* _Bin2D(self) except NULL:
        return <c.Bin2D_${DBN}*> self.ptr()


    def __init__(self, xlow, xhigh, ylow, yhigh):
        util.set_owned_ptr(self, new c.Bin2D_${DBN}( pair[double, double](xlow, xhigh),
                                                     pair[double, double](ylow, yhigh) ))


    def scaleXY(self, x=1.0, y=1.0):
        self._Bin2D().scaleXY(x, y)

    def scaleW(self, w):
        self._Bin2D().scaleW(w)


    @property
    def xMin(self):
        """Low edge in x."""
        return self._Bin2D().xMin()

    @property
    def xMax(self):
        """High edge in x."""
        return self._Bin2D().xMax()

    @property
    def yMin(self):
        """Low edge in y."""
        return self._Bin2D().yMin()

    @property
    def yMax(self):
        """High edge in y."""
        return self._Bin2D().yMax()

    @property
    def edges(self):
        """
        The lower and upper edges.

        TODO: Do this in Python, minimise the intelligence in Cython.
        """
        cdef pair[double, double] x = self._Bin2D().xedges()
        cdef pair[double, double] y = self._Bin2D().yedges()
        return util.XY((x.first, x.second), (y.first, y.second))


    @property
    def widths(self):
        """The widths of this bin in the x- and y-dimensions."""
        return util.XY(self._Bin2D().xWidth(), self._Bin2D().yWidth())


    @property
    def focus(self):
        """The focus of the bin in the x- and y-dimensions"""
        cdef pair[double, double] f = self._Bin2D().focus()
        return util.XY(f.first, f.second)

    @property
    def xMid(self):
        """Geometric centre of the bin in x"""
        return self._Bin2D().xMid()

    @property
    def yMid(self):
        """Geometric centre of the bin in y"""
        return self._Bin2D().yMid()

    @property
    def midpoint(self):
        """Geometric centre of the bin"""
        cdef pair[double, double] f = self._Bin2D().midpoint()
        return util.XY(f.first, f.second)


    @property
    def xMean(self):
        return self._Bin2D().xMean()

    @property
    def yMean(self):
        return self._Bin2D().xMean()

    @property
    def mean(self):
        return util.XY(self._Bin2D().xMean(), self._Bin2D().yMean())

    @property
    def xStdDev(self):
        return self._Bin2D().xStdDev()

    @property
    def yStdDev(self):
        return self._Bin2D().yStdDev()

    @property
    def stdDev(self):
        return util.XY(self._Bin2D().xStdDev(), self._Bin2D().yStdDev())

    @property
    def xStdErr(self):
        return self._Bin2D().xStdErr()

    @property
    def yStdErr(self):
        return self._Bin2D().yStdErr()

    @property
    def stdErr(self):
        return util.XY(self._Bin2D().xStdErr(), self._Bin2D().yStdErr())

    @property
    def xRMS(self):
        return self._Bin2D().xRMS()

    @property
    def yRMS(self):
        return self._Bin2D().yRMS()

    @property
    def rms(self):
        return util.XY(self._Bin2D().xRMS(), self._Bin2D().yRMS())


    # Raw statistics #
    ##################

    @property
    def numEntries(self):
        return int(self._Bin2D().numEntries())

    @property
    def effNumEntries(self):
        return self._Bin2D().effNumEntries()

    @property
    def sumW(self):
        return self._Bin2D().sumW()

    @property
    def sumW2(self):
        return self._Bin2D().sumW2()

    @property
    def sumWX(self):
        return self._Bin2D().sumWX()

    @property
    def sumWY(self):
        return self._Bin2D().sumWY()

    @property
    def sumWXY(self):
        return self._Bin2D().sumWXY()

    @property
    def sumWX2(self):
        return self._Bin2D().sumWX2()

    @property
    def sumWY2(self):
        return self._Bin2D().sumWY2()


    #def merge(Bin2D_${DBN} self, Bin2D_${DBN} other):
    #    self._Bin2D().merge(deref(other._Bin2D()))
    #    return self

    def adjacentTo(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return self._Bin2D().adjacentTo(deref(other._Bin2D()))


    def __add__(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return util.new_owned_cls(
            Bin2D_${DBN},
            new c.Bin2D_${DBN}(deref(self._Bin2D()) + deref(other._Bin2D())))

    def __sub__(Bin2D_${DBN} self, Bin2D_${DBN} other):
        return util.new_owned_cls(
            Bin2D_${DBN},
            new c.Bin2D_${DBN}(deref(self._Bin2D()) - deref(other._Bin2D())))
