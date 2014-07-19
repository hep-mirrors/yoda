# TODO: docstrings
cdef class Axis2D_${BIN2D}_${DBN}(util.Base):

    cdef inline c.Axis2D[c.${BIN2D}, c.${DBN}]* _Axis2D(self) except NULL:
        return <c.Axis2D[c.${BIN2D}, c.${DBN}]*> self.ptr()

    def __dealloc__(self):
        cdef c.Axis2D[c.${BIN2D}, c.${DBN}]* p = self._Axis2D()
        if self._deallocate:
            del p


    def __init__(self, nx, xl, xu, ny, yl, yu):
        cutil.set_owned_ptr(self, new c.Axis2D[c.${BIN2D}, c.${DBN}](
            nx, pair[double, double](xl, xu),
            ny, pair[double, double](yl, yu)))


    @property
    def numBins(self):
        return self._Axis1D().bins().size()

    def __len__(self):
        return self.numBins

    # TODO: remove
    # def __getitem__(self, py_ix):
    #     cdef size_t i = util.pythonic_index(py_ix, self._Axis2D().bins().size())
    #     return cutil.new_borrowed_cls(${BIN2D}, & self._Axis2D().bins().at(i), self)

    def __repr__(self):
        # TODO: improve
        return "<Axis2D with %d bins>" % self.numBins


    @property
    def totalDbn(self):
        return cutil.new_owned_cls(
            ${DBN}, new c.${DBN}(self._Axis2D().totalDbn()))

    def addBin(self, a, b, c, d):
        self._Axis2D().addBin(a, b, c, d)

    @property
    def outflow(self, ix, iy):
        return cutil.new_owned_cls(${DBN}, new c.${DBN}(self._Axis2D().outflow(ix, iy)))

    @property
    def edges(self):
        return util.XY(
            util.EdgePair(self._Axis2D().lowEdgeX(), self._Axis2D().highEdgeX()),
            util.EdgePair(self._Axis2D().lowEdgeY(), self._Axis2D().highEdgeY())
        )

    def reset(self):
        self._Axis2D().reset()

    def binAt(self, x, y):
        cdef int ix = self._Axis2D().getBinIndex(x, y)
        if ix < 0:
            raise YodaExc_RangeError('No bin found!')
        return self[ix]
