cdef class Axis1D_${BIN1D}_${DBN}(util.Base):

    def __init__(self, size_t nbins, double lower, double upper):
        util.set_owned_ptr(
            self, new c.Axis1D[c.${BIN1D}, c.${DBN}](
                nbins, lower, upper))

    def __len__(self):
        return self._Axis1D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Axis1D().bins().size())
        return util.new_borrowed_cls(
            ${BIN1D}, & self._Axis1D().bins().at(i), self)

    def __repr__(self):
        return "<Axis1D>"

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

    def bin_at(self, x):
        return self[self._Axis1D().getBinIndex(x)]

    # BOILERPLATE STUFF
    cdef inline c.Axis1D[c.${BIN1D}, c.${DBN}] *_Axis1D(self) except NULL:
        return <c.Axis1D[c.${BIN1D}, c.${DBN}]*> self.ptr()

    def __dealloc__(self):
        cdef c.Axis1D[c.${BIN1D}, c.${DBN}] *p = self._Axis1D()
        if self._deallocate:
            del p
