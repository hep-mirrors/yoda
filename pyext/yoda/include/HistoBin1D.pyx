cdef class HistoBin1D(Bin1D_Dbn1D):

    cdef inline c.HistoBin1D *_HistoBin1D(self) except NULL:
        return <c.HistoBin1D *> self.ptr()

    def __init__(self, double a, double b):
        util.set_owned_ptr(
            self, new c.HistoBin1D(a, b))
        
    def fill(self, double value, double weight=1.0):
        self._HistoBin1D().fill(value, weight)

    @property
    def area(self):
        return self._HistoBin1D().area()

    @property
    def height(self):
        return self._HistoBin1D().height()

    @property
    def areaErr(self):
        return self._HistoBin1D().areaErr()

    @property
    def heightErr(self):
        return self._HistoBin1D().heightErr()

    def __add__(HistoBin1D a, HistoBin1D b):
        return util.new_owned_cls(
            HistoBin1D,
            new c.HistoBin1D(
                deref(a._HistoBin1D()) + 
                deref(b._HistoBin1D())))

    def __sub__(HistoBin1D a, HistoBin1D b):
        return util.new_owned_cls(
            HistoBin1D,
            new c.HistoBin1D(
                deref(a._HistoBin1D()) -
                deref(b._HistoBin1D())))

