
#TODO docstrings
cdef class Bin2D_${DBN}(Bin):
    """1D Bin class templated on a ${DBN}"""

    def __init__(self, xlow, xhigh, ylow, yhigh):
        util.set_owned_ptr(
            self, new c.Bin2D_${DBN}(
                pair[double, double](xlow, xhigh),
                pair[double, double](ylow, yhigh),
            ))

    def scale(self, x=1.0, y=1.0, w=1.0):
        if x != 1.0 or y != 1.0:
            self.scaleXY(x, y)

        if w != 1.0:
            self.scaleW(w)

    @property
    def edges(self):
        """The lower and upper edges."""
        cdef pair[double, double] x = self._Bin2D().xedges()
        cdef pair[double, double] y = self._Bin2D().yedges()
        return util.XY(util.Edges(x.first, x.second),
                  util.Edges(y.first, y.second))

    @property
    def widths(self):
        """The widths of this bin in the x- and y-dimensions."""
        return util.XY(self._Bin2D().widthX(), self._Bin2D().widthY())

    @property
    def focus(self):
        """The focus of the bin in the x- and y-dimensions"""
        cdef pair[double, double] f = self._Bin2D().focus()
        return util.XY(f.first, f.second)

    @property
    def midpoint(self):
        cdef pair[double, double] f = self._Bin2D().midpoint()
        return util.XY(f.first, f.second)

    @property
    def mean(self):
        return util.XY(self._Bin2D().xMean(), self._Bin2D().yMean())

    @property
    def std_dev(self):
        return util.XY(self._Bin2D().xStdDev(), self._Bin2D().yStdDev())

    @property
    def std_err(self):
        return util.XY(self._Bin2D().xStdErr(), self._Bin2D().yStdErr())

    @property
    def rms(self):
        return util.XY(self._Bin2D().xRMS(), self._Bin2D().yRMS())

    # Raw statistics #
    ##################
    @property
    def count(self):
        return self._Bin2D().numEntries()

    @property
    def effective_count(self):
        return self._Bin2D().effNumEntries()

    @property
    def sum_w(self):
        return self._Bin2D().sumW()

    @property
    def sum_w2(self):
        return self._Bin2D().sumW2()

    @property
    def sum_wx(self):
        return self._Bin2D().sumWX()

    @property
    def sum_wy(self):
        return self._Bin2D().sumWY()

    @property
    def sum_wxy(self):
        return self._Bin2D().sumWXY()

    @property
    def sum_wx2(self):
        return self._Bin2D().sumWX2()

    @property
    def sum_wy2(self):
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
    
    cdef inline c.Bin2D_${DBN} *_Bin2D(self) except NULL:
        return <c.Bin2D_${DBN} *> self.ptr()
