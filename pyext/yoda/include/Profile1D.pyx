#TODO: docstrings!!!
#TODO: introduce better constructors

cdef class Profile1D(AnalysisObject):
    cdef inline c.Profile1D* _Profile1D(self) except NULL:
        return <c.Profile1D*> self.ptr()

    def __init__(self, *args, **kwargs):
        # TODO: convert to the new-style Profile1D
        if len(args) <= 2:
            self.__init2(*args)
        elif len(args) >= 3:
            self.__init5(*args)

    def __init2(self, char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Profile1D(string(path), string(title)))

    def __init5(self, size_t nbins, double lower, double upper,
                  char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Profile1D(
                nbins, lower, upper, string(path), string(title)))


    def __len__(self):
        return self._Profile1D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Profile1D().bins().size())
        return util.new_borrowed_cls(
            ProfileBin1D, & self._Profile1D().bins().at(i), self)

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%0.2g>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW())


    def reset(self):
        self._Profile1D().reset()

    def copy(self, char *path=""):
        return util.new_owned_cls(Profile1D,
            new c.Profile1D(deref(self._Profile1D()), string(path)))


    def fill(self, x, y, weight=1.0):
        self._Profile1D().fill(x, y, weight)

    def fillBin(self, size_t i, double y, weight=1.0):
        self._Profile1D().fillBin(i, y, weight)

    # def fill_many(self, xs, ys, zs, ws=None):
    #     ...

    @property
    def totalDbn(self):
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().totalDbn(), self)

    @property
    def underflow(self):
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().underflow(), self)

    @property
    def overflow(self):
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().overflow(), self)


    def numEntries(self): # add overflows arg
        return self._Profile1D().numEntries()

    def effNumEntries(self): # add overflows arg
        return self._Profile1D().effNumEntries()

    def sumW(self, overflows=True):
        return self._Profile1D().sumW(overflows)

    def sumW2(self, overflows=True):
        return self._Profile1D().sumW2(overflows)


    def scale(self, double w=1.0):
        """
        (w=1.0) -> None

        """
        if w != 1.0:
            self._Profile1D().scaleW(w)


    @property
    def bins(self):
        return list(self)

    def addBin(self, low, high):
        """Add a bin to the Profile1D"""
        self._Profile1D().addBin(low, high)
        return self

    # TODO: look at Histo1D
    def addBins(self, edges):
        cdef vector[double] cedges
        for i in edges:
            cedges.push_back(i)
        self._Profile1D().addBins(cedges)
        return self

    def mergeBins(self, size_t a, size_t b):
        self._Profile1D().mergeBins(a, b)

    def rebin(self, int n):
        self._Profile1D().rebin(n)


    def mkScatter(self):
        "Convert this Profile1D to a Scatter2D"
        cdef c.Scatter2D s2 = c.mkScatter_Profile1D(deref(self._Profile1D()))
        return util.new_owned_cls(Scatter2D, new c.Scatter2D(s2, s2.path()))



    def __iadd__(Profile1D self, Profile1D other):
        c.Profile1D_iadd_Profile1D(self._Profile1D(), other._Profile1D())
        return self
    def __isub__(Profile1D self, Profile1D other):
        c.Profile1D_isub_Profile1D(self._Profile1D(), other._Profile1D())
        return self

    def __add__(Profile1D self, Profile1D other):
        h = Profile1D()
        util.set_owned_ptr(h, c.Profile1D_add_Profile1D(self._Profile1D(), other._Profile1D()))
        return h
    def __sub__(Profile1D self, Profile1D other):
        h = Profile1D()
        util.set_owned_ptr(h, c.Profile1D_sub_Profile1D(self._Profile1D(), other._Profile1D()))
        return h
