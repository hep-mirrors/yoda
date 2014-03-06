
cdef class Profile1D(AnalysisObject):
    """
    1D profile histogram, used to measure mean values of a y variable, binned in x.

    Complete histogram binning is supported, including uniform/regular binning,
    variable-width binning, unbinned gaps in the covered range, and
    under/overflows. Rebinning by integer factors, or by explicit merging of
    contiguous bins is also supported.

    Rescaling of weights and/or the x axis is permitted in-place: the result is
    still a valid Histo1D. Binning-compatible 1D histograms may be divided,
    resulting in a Scatter2D since further fills would not be meaningful.

    Several sets of arguments are tried by the constructor in the following
    order.

    Profile1D(path="", title="").
      Construct a histogram with optional path and title but no bins.

    Profile1D(nbins, low, high, path="", title="")
      Construct a histogram with optional path and title, and nbins bins
      uniformly distributed between low and high.

    Profile1D(B, path="", title="").
      Construct a histogram with optional path and title, from an
      iterator of bins, B.
    """

    cdef inline c.Profile1D* _Profile1D(self) except NULL:
        return <c.Profile1D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init5, self.__init3], *args, **kwargs)

    def __init2(self, char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Profile1D(string(path), string(title)))

    def __init5(self, size_t nbins, double lower, double upper,
                  char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Profile1D(
                nbins, lower, upper, string(path), string(title)))

    def __init3(self, bins, char *path="", char *title=""):
        self.__init2(path, title)
        self.addBins(bins)


    def __len__(self):
        return self._Profile1D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Profile1D().bins().size())
        return util.new_borrowed_cls(ProfileBin1D, & self._Profile1D().bins().at(i), self)


    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%0.2g>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self._Profile1D().reset()

    def clone(self):
        """None -> Profile1D.
        Clone this Profile1D."""
        return util.new_owned_cls(Profile1D, self._Profile1D().newclone())


    def fill(self, x, y, weight=1.0):
        """(x,y,[w]) -> None.
        Fill with given x & y values and optional weight."""
        self._Profile1D().fill(x, y, weight)

    def fillBin(self, size_t ix, double y, weight=1.0):
        """(ix,y,[w]) -> None.
        Fill bin ix with y value and optional weight."""
        self._Profile1D().fillBin(ix, y, weight)


    @property
    def totalDbn(self):
        """() -> Dbn2D
        The Dbn2D representing the total distribution."""
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().totalDbn(), self)

    @property
    def underflow(self):
        """() -> Dbn2D
        The Dbn2D representing the underflow distribution."""
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().underflow(), self)

    @property
    def overflow(self):
        """() -> Dbn2D
        The Dbn2D representing the overflow distribution."""
        return util.new_borrowed_cls(
            Dbn2D, &self._Profile1D().overflow(), self)


    def numEntries(self): # add overflows arg
        """([bool]) -> int
        Number of times this histogram was filled."""
        return self._Profile1D().numEntries()

    def effNumEntries(self): # add overflows arg
        """([bool]) -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self._Profile1D().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self._Profile1D().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self._Profile1D().sumW2(overflows)


    def scaleW(self, double w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self._Profile1D().scaleW(w)


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self._Profile1D().numBins()

    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    def addBin(self, low, high):
        """Add a bin."""
        self._Profile1D().addBin(low, high)
        return self

    def addBins(self, edges):
        """Add several bins."""
        # TODO: simplify / make consistent
        cdef vector[double] cedges
        for i in edges:
            cedges.push_back(i)
        self._Profile1D().addBins(cedges)
        return self

    def mergeBins(self, a, b):
        """mergeBins(ia, ib) -> None.
        Merge bins from indices ia through ib."""
        self._Profile1D().mergeBins(a, b)

    def rebin(self, int n):
        """(low, high) -> None.
        Add a bin."""
        self._Profile1D().rebin(n)


    def mkScatter(self):
        """None -> Scatter2D.
        Convert this Profile1D to a Scatter2D, with y representing
        mean bin y values and their standard errors."""
        cdef c.Scatter2D s2 = c.mkScatter_Profile1D(deref(self._Profile1D()))
        return util.new_owned_cls(Scatter2D, s2.newclone())



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
