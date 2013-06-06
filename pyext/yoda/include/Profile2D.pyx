#TODO rewrite when we have a Profile2D
cdef class Profile2D(AnalysisObject):
    cdef inline c.Profile2D *_Profile2D(self) except NULL:
        return <c.Profile2D*> self.ptr()

    def __init__(self, *args, **kwargs):
        if len(args) <= 2:
            self.__init2(*args, **kwargs)
        else:
            self.__init7(*args, **kwargs)

    def __init2(Profile2D self, char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Profile2D(string(path), string(title)))

    def __init7(Profile2D self,   nxbins, xlow, xupp,   nybins, ylow, yupp,
                char *path="", char *title=""):

        util.set_owned_ptr(
            self, new c.Profile2D(
                nxbins, xlow, xupp,
                nybins, ylow, yupp,
                string(path), string(title)))

    def __len__(self):
        return self._Profile2D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Profile2D().bins().size())
        return util.new_borrowed_cls(
            ProfileBin2D, & self._Profile2D().bins().at(i), self)

    def fill(self, double x, double y, double z, double weight=1.0):
        self._Profile2D().fill(x, y, z, weight)

    def bin_by_coord(self, x, y):
        cdef int index = self._Profile2D().binIndexAt(x, y)
        print index
        return self[index]

    def fill_many(self, xs, ys, zs, ws=None):
        cdef double x, y, z, w
        cdef c.Profile2D *p = self._Profile2D()

        itx = iter(xs)
        ity = iter(ys)
        itz = iter(zs)
        if ws:
            itw = iter(ws)
            while True:
                try:
                    x = next(itx, None)
                    y = next(ity, None)
                    z = next(itz, None)
                    w = next(itw, None)
                except TypeError:
                    break
                else:
                    p.fill(x, y, z, w)
        else:
            while True:
                try:
                    x = next(itx, None)
                    y = next(ity, None)
                    z = next(itz, None)
                except TypeError:
                    break
                else:
                    p.fill(x, y, z, 1.0)

    def copy(self, char *path=""):
        return util.new_owned_cls(Profile2D,
            new c.Profile2D(deref(self._Profile2D()), string(path)))

    @property
    def total_dbn(self):
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().totalDbn()), self)

    def outflow(self, ix, iy):
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().outflow(ix, iy)), self)


    #def integral(self, overflows=True):
    #    return self._Profile2D().integral(overflows)

    def sum_w(self, overflows=True):
        return self._Profile2D().sumW(overflows)

    def sum_w2(self, overflows=True):
        return self._Profile2D().sumW2(overflows)

    #def mean(self, overflows=True):
    #    return XY(
    #        self._Profile2D().xMean(overflows),
    #        self._Profile2D().yMean(overflows))

    #def variance(self, overflows=True):
    #    return XY(
    #        self._Profile2D().xVariance(overflows),
    #        self._Profile2D().yVariance(overflows))

    #def std_dev(self, overflows=True):
    #    return XY(
    #        self._Profile2D().xStdDev(overflows),
    #        self._Profile2D().yStdDev(overflows))

    #def std_err(self, overflows=True):
    #    return XY(
    #        self._Profile2D().xStdErr(overflows),
    #        self._Profile2D().yStdErr(overflows))

    def reset(self):
        self._Profile2D().reset()

    #def scale(self, double w=1.0):
    #    """
    #    (w=1.0) -> None

    #    """
    #    if w != 1.0:
    #        self._Profile2D().scaleW(w)

    #def merge_bins(self, size_t a, size_t b):
    #    self._Profile2D().mergeBins(a, b)

    # NOTE: Cython 0.17 will be bringing automatic conversion between python and
    # C++ -- will be very nice here.

    #def rebin(self, int n):
    #    self._Profile2D().rebin(n)

    def addBin(self, double xlow, double xhigh, double ylow, double yhigh):
        """Add a bin to the Profile2D"""
        self._Profile2D().addBin(pair[double, double](xlow, xhigh),
                                 pair[double, double](ylow, yhigh))
        return self

    def addBins(self, xcuts, ycuts):
        cdef vector[double] _xcuts
        cdef vector[double] _ycuts
        for x in xcuts:
            _xcuts.push_back(x)
        for y in ycuts:
            _ycuts.push_back(y)

        self._Profile2D().addBins(_xcuts, _ycuts)
        return self

