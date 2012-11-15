# TODO (when there is absolutely nothing else to do) docstrings (but never will
# it be a user facing class... it's merely there for tests)
cdef class Axis2D_${BIN2D}_${DBN}(util.Base):

    def __init__(self, nx, xl, xu, ny, yl, yu):
        util.set_owned_ptr(
            self, new c.Axis2D[c.${BIN2D}, c.${DBN}](
                nx, pair[double, double](xl, xu),
                ny, pair[double, double](yl, yu)))

    def __len__(self):
        return self._Axis2D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Axis2D().bins().size())
        return util.new_borrowed_cls(
            ${BIN2D}, & self._Axis2D().bins().at(i), self)

    def __repr__(self):
        return "<Axis2D>"

    @property
    def totalDbn(self):
        return util.new_owned_cls(
            ${DBN}, new c.${DBN}(self._Axis2D().totalDbn()))

    def addBin(self, a, b, c, d):
        self._Axis1D().addBin(a, b, c, d)

    @property
    def outflow(self, ix, iy):
        return util.new_owned_cls(
            ${DBN}, new c.${DBN}(self._Axis2D().outflow(ix, iy)))
    
    @property
    def edges(self):
        return util.XY(
            util.Edges(self._Axis2D().lowEdgeX(), self._Axis2D().highEdgeX()),
            util.Edges(self._Axis2D().lowEdgeY(), self._Axis2D().highEdgeY())
        )

    def reset(self):
        self._Axis2D().reset()

    def binByCoord(self, x, y):
        cdef int ix = self._Axis2D().getBinIndex(x, y)
        if ix < 0:
            raise YodaExc_RangeError('No bin found!')
        return self[ix]

    # BOILERPLATE STUFF
    cdef inline c.Axis2D[c.${BIN2D}, c.${DBN}] *_Axis2D(self) except NULL:
        return <c.Axis2D[c.${BIN2D}, c.${DBN}]*> self.ptr()

    def __dealloc__(self):
        cdef c.Axis2D[c.${BIN2D}, c.${DBN}] *p = self._Axis2D()
        if self._deallocate:
            del p
