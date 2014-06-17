cdef class Profile2D(AnalysisObject):
    """
    2D profile histogram, used to measure mean values of a z variable, binned in x and y.

    Complete histogramming is supported, including uniform/regular binning,
    variable-width bininng, unbinned gaps in the covered range, and outflows
    (under/overflows around all edges and corners).

    Rebinning by integer factors, or by explicit merging of contiguous bins is
    also supported, but in development.

    Rescaling of weights and/or the x axis is permitted in-place: the
    result is still a valid Histo2D. Binning-compatible 1D histograms
    may be divided, resulting in a Scatter3D since further fills would
    not be meaningful.

    Several sets of arguments are tried by the constructor in the
    following order.

    Histo2D(path="", title="").
      Construct a histogram with optional path and title but no bins.

    Histo2D(nxbins, xlow, xhigh, nybins, ylow, yhigh, path="", title="").
      Construct a histogram with nxbins on the x axis and nybins on the y
      axis, distributed linearly between the respective low--high limits.

    NOT YET FINISHED: please contact the YODA authors if you require extra functionality.
    """

    cdef inline c.Profile2D *_Profile2D(self) except NULL:
        return <c.Profile2D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init4, self.__init8], *args, **kwargs)

    def __init2(Profile2D self, char *path="", char *title=""):
        util.set_owned_ptr(self, new c.Profile2D(string(path), string(title)))

    def __init4(Profile2D self, xedges,  yedges,  char* path="", char* title=""):
        # TODO: Do some type-checking and allow iterables of ProfileBin2D as well?
        util.set_owned_ptr(self, new c.Profile2D(xedges, yedges, string(path), string(title)))

    def __init8(Profile2D self, nxbins, xlow, xhigh,  nybins, ylow, yhigh,  char *path="", char *title=""):
        util.set_owned_ptr(self, new c.Profile2D(nxbins, xlow, xhigh,  nybins, ylow, yhigh,  string(path), string(title)))



    def __len__(self):
        return self._Profile2D().numBins()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Profile2D().numBins())
        return util.new_borrowed_cls(ProfileBin2D, & self._Profile2D().bins().at(i), self)

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%0.2g>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self._Profile2D().reset()

    def clone(self):
        """None -> Profile2D.
        Clone this Profile2D."""
        return util.new_owned_cls(Profile2D, self._Profile2D().newclone())


    def fill(self, double x, double y, double z, double weight=1.0):
        """(x,y,z,[w]) -> None.
        Fill with given x,y & z values and optional weight."""
        self._Profile2D().fill(x, y, z, weight)

    def fillBin(self, size_t i, double z, weight=1.0):
        """(i,z,[w]) -> None.
        Fill bin i with value z and optional weight."""
        self._Profile2D().fillBin(i, z, weight)


    @property
    def totalDbn(self):
        """() -> Dbn3D
        The Dbn3D representing the total distribution."""
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().totalDbn()), self)

    def outflow(self, ix, iy):
        """(ix,iy) -> Dbn3D
        The Dbn3D representing the ix,iy outflow distribution."""
        return util.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self._Profile2D().outflow(ix, iy)), self)


    def numEntries(self): # add overflows arg
        """() -> int
        Number of times this histogram was filled."""
        return int(self._Profile2D().numEntries())

    def effNumEntries(self): # add overflows arg
        """() -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self._Profile2D().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self._Profile2D().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self._Profile2D().sumW2(overflows)


    def scaleW(self, w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self._Profile2D().scaleW(w)


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self._Profile2D().numBins()

    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    # TODO: change name to better match / don't map at all
    def bin_by_coord(self, x, y):
        cdef int index = self._Profile2D().binIndexAt(x, y)
        print index
        return self[index]

    def addBin(self, double xlow, double xhigh, double ylow, double yhigh):
        """Add a bin."""
        self._Profile2D().addBin(pair[double, double](xlow, xhigh),
                                 pair[double, double](ylow, yhigh))
        return self

    def addBins(self, xcuts, ycuts):
        """Add several bins."""
        # TODO: simplify / make consistent
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
        """None -> Scatter3D.
        Convert this Profile2D to a Scatter3D, with z representing
        mean bin y values and their standard errors."""
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
