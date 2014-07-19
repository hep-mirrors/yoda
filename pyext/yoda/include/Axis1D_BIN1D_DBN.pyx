# TODO (when there is absolutely nothing else to do) docstrings (but never will
# it be a user facing class... it's merely there for tests)
cdef class Axis1D_${BIN1D}_${DBN}(util.Base):

    cdef inline c.Axis1D[c.${BIN1D}, c.${DBN}]* _Axis1D(self) except NULL:
        return <c.Axis1D[c.${BIN1D}, c.${DBN}]*> self.ptr()

    def __dealloc__(self):
        cdef c.Axis1D[c.${BIN1D}, c.${DBN}]* p = self._Axis1D()
        if self._deallocate:
            del p


    def __init__(self):
        util.set_owned_ptr(self, new c.Axis1D[c.${BIN1D}, c.${DBN}]())

    def __repr__(self):
        return "<Axis1D with %d bins>" % self.numBins


    @property
    def numBins(self):
        return self._Axis1D().bins().size()

    def __len__(self):
        return self.numBins

    # TODO: remove
    # def __getitem__(self, py_ix):
    #     cdef size_t i = util.pythonic_index(py_ix, self._Axis1D().bins().size())
    #     return util.new_borrowed_cls(
    #         ${BIN1D}, & self._Axis1D().bins().at(i), self)


    def addBin(self, a, b):
        self._Axis1D().addBin(a, b)

    @property
    def totalDbn(self):
        return util.new_borrowed_cls(
            ${DBN}, &self._Axis1D().totalDbn(), self)

    @property
    def underflow(self):
        return util.new_borrowed_cls(
            ${DBN}, &self._Axis1D().underflow(), self)

    @property
    def overflow(self):
        return util.new_borrowed_cls(
            ${DBN}, &self._Axis1D().overflow(), self)

    def reset(self):
        self._Axis1D().reset()

    def eraseBin(self, i):
        self._Axis1D().eraseBin(i)

    def getBinIndex(self, x):
        return self._Axis1D().getBinIndex(x)

    def mergeBins(self, a, b):
        self._Axis1D().mergeBins(a, b)

    #def binAt(self, x):
    #    return self[self._Axis1D().getBinIndex(x)]
