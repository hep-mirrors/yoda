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

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%0.2g>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW())


    def reset(self):
        self._Profile2D().reset()

    def copy(self, char *path=""):
        return util.new_owned_cls(Profile2D,
            new c.Profile2D(deref(self._Profile2D()), string(path)))


    def fill(self, double x, double y, double z, double weight=1.0):
        self._Profile2D().fill(x, y, z, weight)

    def fillBin(self, size_t i, double y, weight=1.0):
        self._Profile2D().fillBin(i, y, weight)

    # def fill_many(self, xs, ys, zs, ws=None):
    #     cdef double x, y, z, w
    #     cdef c.Profile2D *p = self._Profile2D()

    #     itx = iter(xs)
    #     ity = iter(ys)
    #     itz = iter(zs)
    #     if ws:
    #         itw = iter(ws)
    #         while True:
    #             try:
    #                 x = next(itx, None)
    #                 y = next(ity, None)
    #                 z = next(itz, None)
    #                 w = next(itw, None)
    #             except TypeError:
    #                 break
    #             else:
    #                 p.fill(x, y, z, w)
    #     else:
    #         while True:
    #             try:
    #                 x = next(itx, None)
    #                 y = next(ity, None)
    #                 z = next(itz, None)
    #             except TypeError:
    #                 break
    #             else:
    #                 p.fill(x, y, z, 1.0)

    @property
    def totalDbn(self):
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().totalDbn()), self)

    def outflow(self, ix, iy):
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().outflow(ix, iy)), self)


    #def integral(self, overflows=True):
    #    return self._Profile2D().integral(overflows)

    def numEntries(self): # add overflows arg
        """Number of times this histogram was filled."""
        return self._Profile2D().numEntries()

    def effNumEntries(self): # add overflows arg
        """Effective number of times this histogram was filled, computed from weights."""
        return self._Profile2D().effNumEntries()

    def sumW(self, overflows=True):
        """Sum of weights filled into this histogram."""
        return self._Profile2D().sumW(overflows)

    def sumW2(self, overflows=True):
        """Sum of squared weights filled into this histogram."""
        return self._Profile2D().sumW2(overflows)


    def scaleW(self, w):
        """ (float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self._Profile2D().scaleW(w)


    @property
    def numBins(self):
        """Number of bins (not including overflows)."""
        return self._Profile2D().numBins()

    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    def bin_by_coord(self, x, y):
        cdef int index = self._Profile2D().binIndexAt(x, y)
        print index
        return self[index]

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


    # def mergeBins(self, size_t a, size_t b):
    #     self._Profile2D().mergeBins(a, b)

    # def rebin(self, int n):
    #     self._Profile2D().rebin(n)


    def mkScatter(self):
        "Convert this Profile2D to a Scatter3D"
        raise Exception("TODO: Not yet implemented!")
        # cdef c.Scatter2D s2 = c.mkScatter_Profile1D(deref(self._Profile1D()))
        # return util.new_owned_cls(Scatter2D, s2.newclone())


    def __iadd__(Profile2D self, Profile2D other):
        c.Profile2D_iadd_Profile2D(self._Profile2D(), other._Profile2D())
        return self
    def __isub__(Profile2D self, Profile2D other):
        c.Profile2D_isub_Profile2D(self._Profile2D(), other._Profile2D())
        return self

    def __add__(Profile2D self, Profile2D other):
        h = Profile2D()
        util.set_owned_ptr(h, c.Profile2D_add_Profile2D(self._Profile2D(), other._Profile2D()))
        return h
    def __sub__(Profile2D self, Profile2D other):
        h = Profile2D()
        util.set_owned_ptr(h, c.Profile2D_sub_Profile2D(self._Profile2D(), other._Profile2D()))
        return h
