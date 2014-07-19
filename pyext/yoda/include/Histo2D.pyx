cdef class Histo2D(AnalysisObject):
    """
    2D histogram.

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
    """

    cdef inline c.Histo2D *_Histo2D(self) except NULL:
        return <c.Histo2D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init4, self.__init8], *args, **kwargs)

    def __init2(Histo2D self, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Histo2D(string(path), string(title)))

    def __init4(Histo2D self, xedges,  yedges,  char* path="", char* title=""):
        # TODO: Do some type-checking and allow iterables of HistoBin2D as well?
        cutil.set_owned_ptr(self, new c.Histo2D(xedges, yedges, string(path), string(title)))

    def __init8(Histo2D self, nxbins, xlow, xhigh,  nybins, ylow, yhigh,  char* path="", char* title=""):
        cutil.set_owned_ptr(self, new c.Histo2D(nxbins, xlow, xhigh,  nybins, ylow, yhigh,  string(path), string(title)))


    # TODO: remove
    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Histo2D().numBins())
        return cutil.new_borrowed_cls(HistoBin2D, & self._Histo2D().bins().at(i), self)

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%.2g>" % (self.__class__.__name__, self.path, len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self._Histo2D().reset()

    def clone(self):
        """None -> Histo2D.
        Clone this Profile2D."""
        return cutil.new_owned_cls(Histo2D, self._Histo2D().newclone())


    def fill(self, double x, double y, weight=1.0):
        """(x,y,[w]) -> None.
        Fill with given x,y values and optional weight."""
        self._Histo2D().fill(x, y, weight)

    def fillBin(self, size_t i, weight=1.0):
        """(i,[w]) -> None.
        Fill bin i and optional weight."""
        self._Histo2D().fillBin(i, weight)


    @property
    def totalDbn(self):
        """() -> Dbn2D
        The Dbn2D representing the total distribution."""
        return cutil.new_borrowed_cls(Dbn2D, &self._Histo2D().totalDbn(), self)

    # TODO: reinstate
    # def outflow(self, ix, iy):
    #     """(ix,iy) -> Dbn2D
    #     The Dbn2D representing the ix,iy outflow distribution."""
    #     return cutil.new_borrowed_cls(Dbn2D, &self._Histo2D().outflow(ix, iy), self)


    def integral(self, overflows=True):
        """([bool]) -> float
        Histogram integral, optionally excluding the overflows."""
        return self._Histo2D().integral(overflows)

    def numEntries(self): # add overflows arg
        """() -> int
        Number of times this histogram was filled."""
        return int(self._Histo2D().numEntries())

    def effNumEntries(self): # add overflows arg
        """() -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self._Histo2D().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self._Histo2D().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self._Histo2D().sumW2(overflows)

    def mean(self, overflows=True):
        """([bool]) -> (float,float)
        Mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self._Histo2D().xMean(overflows),
            self._Histo2D().yMean(overflows))

    def variance(self, overflows=True):
        """([bool]) -> (float,float)
        Variances in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self._Histo2D().xVariance(overflows),
            self._Histo2D().yVariance(overflows))

    def stdDev(self, overflows=True):
        """([bool]) -> (float,float)
        Standard deviations in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self._Histo2D().xStdDev(overflows),
            self._Histo2D().yStdDev(overflows))

    def stdErr(self, overflows=True):
        """([bool]) -> (float,float)
        Standard errors on the mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self._Histo2D().xStdErr(overflows),
            self._Histo2D().yStdErr(overflows))


    def scaleW(self, w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self._Histo2D().scaleW(w)

    def normalize(self, double normto, bint includeoverflows=True):
        """(float, bool) -> None.
        Normalize the histogram."""
        self._Histo2D().normalize(normto, includeoverflows)


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self._Histo2D().numBins()

    def __len__(self):
        return self.numBins

    @property
    def numBinsX(self):
        """() -> int
        Number of bins (edges) along the x axis."""
        return self._Histo2D().numBinsX()

    @property
    def numBinsY(self):
        """() -> int
        Number of bins (edges) along the y axis."""
        return self._Histo2D().numBinsY()


    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    # TODO: add bin(ix, iy) and bin(iglobal)

    # TODO: add binAt and binIndexAt and binIndexX,Y


    def addBin(self, xlow, xhigh, ylow, yhigh):
        """Add a bin."""
        self._Histo2D().addBin(pair[double, double](xlow, xhigh),
                               pair[double, double](ylow, yhigh))
        return self

    def addBins(self, bounds):
        """Add several bins."""
        # TODO: simplify / make consistent
        for xlow, xhigh, ylow, yhigh in bounds:
            self._Histo2D().addBin(pair[double, double](xlow, xhigh),
                                   pair[double, double](ylow, yhigh))

    # def mergeBins(self, size_t a, size_t b):
    #     self._Histo2D().mergeBins(a, b)

    # def rebin(self, int n):
    #     self._Histo2D().rebin(n)


    def mkScatter(self):
        """None -> Scatter3D.
        Convert this Histo2D to a Scatter3D, with y representing bin heights
        (not sumW) and height errors."""
        cdef c.Scatter3D s3 = c.mkScatter_Histo2D(deref(self._Histo2D()))
        return cutil.new_owned_cls(Scatter3D, s3.newclone())


    def __iadd__(Histo2D self, Histo2D other):
        c.Histo2D_iadd_Histo2D(self._Histo2D(), other._Histo2D())
        return self
    def __isub__(Histo2D self, Histo2D other):
        c.Histo2D_isub_Histo2D(self._Histo2D(), other._Histo2D())
        return self

    def __add__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_add_Histo2D(self._Histo2D(), other._Histo2D()))
        return h
    def __sub__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_sub_Histo2D(self._Histo2D(), other._Histo2D()))
        return h
