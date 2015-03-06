cdef class Histo2D(AnalysisObject):
    """
    2D histogram.

    Complete histogramming is supported, including uniform/regular binning,
    variable-width bininng, unbinned gaps in the covered range, and outflows
    (under/overflows around all edges and corners).

    Rebinning by integer factors, or by explicit merging of contiguous bins is
    also supported, but in development.

    Rescaling of weights and/or the x axis is permitted in-place: the
    result is still a valid Histo2D. Binning-compatible 2D histograms
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

    cdef inline c.Histo2D* h2ptr(self) except NULL:
        return <c.Histo2D*> self.ptr()
    # TODO: remove
    cdef inline c.Histo2D* _Histo2D(self) except NULL:
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
        cdef size_t i = cutil.pythonic_index(py_ix, self.h2ptr().numBins())
        return cutil.new_borrowed_cls(HistoBin2D, & self.h2ptr().bins().at(i), self)

    def __repr__(self):
        return "<%s '%s' %d bins, sumw=%.2g>" % (self.__class__.__name__, self.path, len(self.bins), self.sumW())


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self.h2ptr().reset()

    def clone(self):
        """None -> Histo2D.
        Clone this Profile2D."""
        return cutil.new_owned_cls(Histo2D, self.h2ptr().newclone())


    def fill(self, double x, double y, weight=1.0):
        """(x,y,[w]) -> None.
        Fill with given x,y values and optional weight."""
        self.h2ptr().fill(x, y, weight)

    def fillBin(self, size_t i, weight=1.0):
        """(i,[w]) -> None.
        Fill bin i and optional weight."""
        self.h2ptr().fillBin(i, weight)


    @property
    def totalDbn(self):
        """() -> Dbn2D
        The Dbn2D representing the total distribution."""
        return cutil.new_borrowed_cls(Dbn2D, &self.h2ptr().totalDbn(), self)

    # TODO: reinstate
    # def outflow(self, ix, iy):
    #     """(ix,iy) -> Dbn2D
    #     The Dbn2D representing the ix,iy outflow distribution."""
    #     return cutil.new_borrowed_cls(Dbn2D, &self.h2ptr().outflow(ix, iy), self)


    def integral(self, overflows=True):
        """([bool]) -> float
        Histogram integral, optionally excluding the overflows."""
        return self.h2ptr().integral(overflows)

    def numEntries(self): # add overflows arg
        """() -> int
        Number of times this histogram was filled."""
        return int(self.h2ptr().numEntries())

    def effNumEntries(self): # add overflows arg
        """() -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self.h2ptr().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self.h2ptr().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self.h2ptr().sumW2(overflows)

    def mean(self, overflows=True):
        """([bool]) -> (float,float)
        Mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self.h2ptr().xMean(overflows),
            self.h2ptr().yMean(overflows))

    def variance(self, overflows=True):
        """([bool]) -> (float,float)
        Variances in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self.h2ptr().xVariance(overflows),
            self.h2ptr().yVariance(overflows))

    def stdDev(self, overflows=True):
        """([bool]) -> (float,float)
        Standard deviations in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self.h2ptr().xStdDev(overflows),
            self.h2ptr().yStdDev(overflows))

    def stdErr(self, overflows=True):
        """([bool]) -> (float,float)
        Standard errors on the mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(
            self.h2ptr().xStdErr(overflows),
            self.h2ptr().yStdErr(overflows))


    def scaleW(self, w):
        """(float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self.h2ptr().scaleW(w)

    def normalize(self, double normto, bint includeoverflows=True):
        """(float, bool) -> None.
        Normalize the histogram."""
        self.h2ptr().normalize(normto, includeoverflows)


    @property
    def xMin(self):
        """Low x edge of the histo."""
        return self.h2ptr().xMin()

    @property
    def xMax(self):
        """High x edge of the histo."""
        return self.h2ptr().xMax()

    @property
    def yMin(self):
        """Low y edge of the histo."""
        return self.h2ptr().yMin()

    @property
    def yMax(self):
        """High y edge of the histo."""
        return self.h2ptr().yMax()


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self.h2ptr().numBins()

    def __len__(self):
        return self.numBins

    @property
    def numBinsX(self):
        """() -> int
        Number of bins (edges) along the x axis."""
        return self.h2ptr().numBinsX()

    @property
    def numBinsY(self):
        """() -> int
        Number of bins (edges) along the y axis."""
        return self.h2ptr().numBinsY()


    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    # TODO: add bin(ix, iy) and bin(iglobal)

    # TODO: add binAt and binIndexAt and binIndexX,Y


    def addBin(self, xlow, xhigh, ylow, yhigh):
        """Add a bin."""
        self.h2ptr().addBin(pair[double, double](xlow, xhigh),
                               pair[double, double](ylow, yhigh))
        return self

    def addBins(self, bounds):
        """Add several bins."""
        # TODO: simplify / make consistent
        for xlow, xhigh, ylow, yhigh in bounds:
            self.h2ptr().addBin(pair[double, double](xlow, xhigh),
                                pair[double, double](ylow, yhigh))

    # def mergeBins(self, size_t a, size_t b):
    #     self.h2ptr().mergeBins(a, b)

    # def rebin(self, int n):
    #     self.h2ptr().rebin(n)


    def mkScatter(self, usefocus=False):
        """None -> Scatter3D.
        Convert this Histo2D to a Scatter3D, with y representing bin heights
        (not sumW) and height errors."""
        cdef c.Scatter3D s3 = c.mkScatter_Histo2D(deref(self.h2ptr()), usefocus)
        return cutil.new_owned_cls(Scatter3D, s3.newclone())

    def divideBy(self, Histo2D h, efficiency=False):
        """Histo2D -> Scatter3D

        Divide this histogram by Histo2D h, returning a Scatter3D. The optional 'efficiency'
        argument, if set True, will use a binomial efficiency treatment of the errors.
        """
        # if type(h) is not Histo2D:
        #     raise ValueError("Histograms must be of the same type to be divided")
        # TODO: allow dividing profiles by histos, etc.? (But then what do the errors mean? Add in quad?)
        cdef c.Scatter3D s
        if not efficiency:
            s = c.Histo2D_div_Histo2D(deref(self.h2ptr()), deref(h.h2ptr()))
        else:
            s = c.Histo2D_eff_Histo2D(deref(self.h2ptr()), deref(h.h2ptr()))
        return cutil.new_owned_cls(Scatter3D, s.newclone())



    def __iadd__(Histo2D self, Histo2D other):
        c.Histo2D_iadd_Histo2D(self.h2ptr(), other.h2ptr())
        return self
    def __isub__(Histo2D self, Histo2D other):
        c.Histo2D_isub_Histo2D(self.h2ptr(), other.h2ptr())
        return self

    def __add__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_add_Histo2D(self.h2ptr(), other.h2ptr()))
        return h
    def __sub__(Histo2D self, Histo2D other):
        h = Histo2D()
        cutil.set_owned_ptr(h, c.Histo2D_sub_Histo2D(self.h2ptr(), other.h2ptr()))
        return h

    def __div__(Histo2D self, Histo2D other):
        return self.divideBy(other)
