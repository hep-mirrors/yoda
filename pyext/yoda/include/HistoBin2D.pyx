# TODO: tidy once we have a working Histo2D
cdef class HistoBin2D(Bin2D_Dbn2D):

    cdef inline c.HistoBin2D *_HistoBin2D(self) except NULL:
        return <c.HistoBin2D *> self.ptr()

    def __init__(self, xlow, xhigh, ylow, yhigh):
        util.set_owned_ptr(
            self, new c.HistoBin2D(xlow, xhigh, ylow, yhigh))
        
    def fill(self, x, y, weight=1.0):
        self._HistoBin2D().fill(x, y, weight)

    @property
    def volume(self):
        return self._HistoBin2D().volume()

    @property
    def height(self):
        return self._HistoBin2D().height()

    @property
    def volume_err(self):
        return self._HistoBin2D().volumeErr()

    @property
    def height_err(self):
        return self._HistoBin2D().heightErr()

    def __add__(HistoBin2D a, HistoBin2D b):
        return util.new_owned_cls(
            HistoBin2D,
            new c.HistoBin2D(
                deref(a._HistoBin2D()) + 
                deref(b._HistoBin2D())))

    def __sub__(HistoBin2D a, HistoBin2D b):
        return util.new_owned_cls(
            HistoBin2D,
            new c.HistoBin2D(
                deref(a._HistoBin2D()) -
                deref(b._HistoBin2D())))

    def __repr__(self):
        return 'HistoBin2D(%g, %g, %g, %g)' % (self.edges.x + self.edges.y)
