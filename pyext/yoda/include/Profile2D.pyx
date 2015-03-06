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

    cdef inline c.Profile2D* p2ptr(self) except NULL:
        return <c.Profile2D*> self.ptr()
    # TODO: Remove
    cdef inline c.Profile2D* _Profile2D(self) except NULL:
        return <c.Profile2D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init4, self.__init8], *args, **kwargs)

    def __init2(Profile2D self, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Profile2D(string(path), string(title)))

    def __init4(Profile2D self, xedges,  yedges,  char* path="", char* title=""):
        # TODO: Do some type-checking and allow iterables of ProfileBin2D as well?
        cutil.set_owned_ptr(self, new c.Profile2D(xedges, yedges, string(path), string(title)))

    def __init8(Profile2D self, nxbins, xlow, xhigh,  nybins, ylow, yhigh,  char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Profile2D(nxbins, xlow, xhigh,  nybins, ylow, yhigh,  string(path), string(title)))


    # TODO: remove
    def __getitem__(self, py_ix):
        cdef size_t i = cutil.pythonic_index(py_ix, self.p2ptr().numBins())
        return cutil.new_borrowed_cls(ProfileBin2D, & self.p2ptr().bins().at(i), self)

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%0.2g>" % (self.__class__.__name__, self.path, len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self.p2ptr().reset()

    def clone(self):
        """None -> Profile2D.
        Clone this Profile2D."""
        return cutil.new_owned_cls(Profile2D, self.p2ptr().newclone())


    def fill(self, double x, double y, double z, double weight=1.0):
        """(x,y,z,[w]) -> None.
        Fill with given x,y & z values and optional weight."""
        self.p2ptr().fill(x, y, z, weight)

    def fillBin(self, size_t i, double z, weight=1.0):
        """(i,z,[w]) -> None.
        Fill bin i with value z and optional weight."""
        self.p2ptr().fillBin(i, z, weight)


    @property
    def totalDbn(self):
        """() -> Dbn3D
        The Dbn3D representing the total distribution."""
        return cutil.new_borrowed_cls(
            Dbn3D, new c.Dbn3D(self.p2ptr().totalDbn()), self)

    # TODO: reinstate
    # def outflow(self, ix, iy):
    #     """(ix,iy) -> Dbn3D
    #     The Dbn3D representing the ix,iy outflow distribution."""
    #     return cutil.new_borrowed_cls(
    #         Dbn3D, new c.Dbn3D(self.p2ptr().outflow(ix, iy)), self)


    def numEntries(self): # add overflows arg
        """() -> int
        Number of times this histogram was filled."""
        return int(self.p2ptr().numEntries())

    def effNumEntries(self): # add overflows arg
        """() -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self.p2ptr().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self.p2ptr().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self.p2ptr().sumW2(overflows)


    def scaleW(self, w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self.p2ptr().scaleW(w)

    def scaleZ(self, double f):
        """(float) -> None.
        Scale the z-direction (profiled value) in this histogram by the factor f."""
        self.p1ptr().scaleZ(f)


    @property
    def xMin(self):
        """Low x edge of the histo."""
        return self.p2ptr().xMin()

    @property
    def xMax(self):
        """High x edge of the histo."""
        return self.p2ptr().xMax()

    @property
    def yMin(self):
        """Low y edge of the histo."""
        return self.p2ptr().yMin()

    @property
    def yMax(self):
        """High y edge of the histo."""
        return self.p2ptr().yMax()


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self.p2ptr().numBins()

    def __len__(self):
        return self.p2ptr().numBins()

    @property
    def numBinsX(self):
        """() -> int
        Number of bins (edges) along the x axis."""
        return self.p2ptr().numBinsX()

    @property
    def numBinsY(self):
        """() -> int
        Number of bins (edges) along the y axis."""
        return self.p2ptr().numBinsY()


    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    # TODO: add bin(ix, iy) and bin(iglobal)

    # TODO: add binAt and binIndexAt and binIndexX,Y
    # def binAt(self, x, y):
    #     cdef int index = self.p2ptr().binIndexAt(x, y)
    #     print index
    #     return self[index]


    def addBin(self, double xlow, double xhigh, double ylow, double yhigh):
        """Add a bin."""
        self.p2ptr().addBin(pair[double, double](xlow, xhigh),
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
        self.p2ptr().addBins(_xcuts, _ycuts)
        return self


    # def mergeBins(self, size_t a, size_t b):
    #     self.p2ptr().mergeBins(a, b)

    # def rebin(self, int n):
    #     self.p2ptr().rebin(n)


    def mkScatter(self):
        """None -> Scatter3D.
        Convert this Profile2D to a Scatter3D, with z representing
        mean bin y values and their standard errors."""
        cdef c.Scatter3D s3 = c.mkScatter_Profile2D(deref(self.p2ptr()))
        return cutil.new_owned_cls(Scatter3D, s3.newclone())

    def divideBy(self, Profile2D h):
        cdef c.Scatter3D s = c.Profile2D_div_Profile2D(deref(self.p2ptr()), deref(h.p2ptr()))
        return cutil.new_owned_cls(Scatter3D, s.newclone())


    def __iadd__(Profile2D self, Profile2D other):
        c.Profile2D_iadd_Profile2D(self.p2ptr(), other.p2ptr())
        return self
    def __isub__(Profile2D self, Profile2D other):
        c.Profile2D_isub_Profile2D(self.p2ptr(), other.p2ptr())
        return self

    def __add__(Profile2D self, Profile2D other):
        h = Profile2D()
        cutil.set_owned_ptr(h, c.Profile2D_add_Profile2D(self.p2ptr(), other.p2ptr()))
        return h
    def __sub__(Profile2D self, Profile2D other):
        h = Profile2D()
        cutil.set_owned_ptr(h, c.Profile2D_sub_Profile2D(self.p2ptr(), other.p2ptr()))
        return h

    def __div__(Profile2D self, Profile2D other):
        return self.divideBy(other)
