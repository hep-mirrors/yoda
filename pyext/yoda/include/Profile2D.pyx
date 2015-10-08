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


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init4, self.__init8], *args, **kwargs)

    def __init2(Profile2D self, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Profile2D(string(path), string(title)))

    def __init4(Profile2D self, xedges,  yedges,  char* path="", char* title=""):
        # TODO: Do some type-checking and allow iterables of ProfileBin2D as well?
        cutil.set_owned_ptr(self, new c.Profile2D(xedges, yedges, string(path), string(title)))

    def __init8(Profile2D self, nxbins, xlow, xhigh,  nybins, ylow, yhigh,  char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Profile2D(nxbins, xlow, xhigh,  nybins, ylow, yhigh,  string(path), string(title)))


    def __len__(self):
        return self.p2ptr().numBins()

    def __getitem__(self, py_ix):
        "Direct access to bins"
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


    def numEntries(self, includeoverflows=True):
        """([bool]) -> int
        Number of times this histogram was filled, optionally excluding the overflows."""
        return self.p2ptr().numEntries(includeoverflows)

    def effNumEntries(self, includeoverflows=True):
        """([bool]) -> float
        Effective number of times this histogram was filled, computed from weights and optionally excluding the overflows."""
        return self.p2ptr().effNumEntries(includeoverflows)

    def sumW(self, includeoverflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram."""
        return self.p2ptr().sumW(includeoverflows)

    def sumW2(self, includeoverflows=True):
        """([bool]) -> float
        Sum of squared weights filled into this histogram."""
        return self.p2ptr().sumW2(includeoverflows)


    def xMean(self, includeoverflows=True):
        """([bool]) -> float
        Mean x of the histogram, optionally excluding the overflows."""
        return self.p2ptr().xMean(includeoverflows)

    def yMean(self, includeoverflows=True):
        """([bool]) -> float
        Mean y of the histogram, optionally excluding the overflows."""
        return self.p2ptr().yMean(includeoverflows)

    def xyMean(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xMean(includeoverflows), self.yMean(includeoverflows))


    def xVariance(self, includeoverflows=True):
        """([bool]) -> float
        Variance in x of the histogram, optionally excluding the overflows."""
        return self.p2ptr().xVariance(includeoverflows)

    def yVariance(self, includeoverflows=True):
        """([bool]) -> float
        Variance in y of the histogram, optionally excluding the overflows."""
        return self.p2ptr().yVariance(includeoverflows)

    def xyVariance(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Variances in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xVariance(includeoverflows), self.yVariance(includeoverflows))


    def xStdDev(self, includeoverflows=True):
        """([bool]) -> float
        Standard deviation in x of the histogram, optionally excluding the overflows."""
        return self.p2ptr().xStdDev(includeoverflows)

    def yStdDev(self, includeoverflows=True):
        """([bool]) -> float
        Standard deviation in y of the histogram, optionally excluding the overflows."""
        return self.p2ptr().yStdDev(includeoverflows)

    def xyStdDev(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Standard deviations in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xStdDev(includeoverflows), self.yStdDev(includeoverflows))


    def xStdErr(self, includeoverflows=True):
        """([bool]) -> float
        Standard error on the mean x of the histogram, optionally excluding the overflows."""
        return self.p2ptr().xStdErr(includeoverflows)

    def yStdErr(self, includeoverflows=True):
        """([bool]) -> float
        Standard error on the mean y of the histogram, optionally excluding the overflows."""
        return self.p2ptr().yStdErr(includeoverflows)

    def xyStdErr(self, includeoverflows=True):
        """([bool]) -> (float,float)
        Standard errors on the mean (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xStdErr(includeoverflows), self.yStdErr(includeoverflows))


    def xRMS(self, includeoverflows=True):
        """([bool]) -> float
        RMS in x of the histogram, optionally excluding the overflows."""
        return self.p2ptr().xRMS(includeoverflows)

    def yRMS(self, includeoverflows=True):
        """([bool]) -> float
        RMS in y of the histogram, optionally excluding the overflows."""
        return self.p2ptr().yRMS(includeoverflows)

    def xyRMS(self, includeoverflows=True):
        """([bool]) -> (float,float)
        RMS in (x,y) of the histogram, optionally excluding the overflows."""
        return util.XY(self.xRMS(includeoverflows), self.yRMS(includeoverflows))



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

    def bin(self, i):
        """Get the i'th bin"""
        # cdef size_t ii = cutil.pythonic_index(i, self.p2ptr().numBins())
        return cutil.new_borrowed_cls(ProfileBin2D, & self.p2ptr().bin(i), self)

    # TODO: it's more intuitive to have an index for each axis
    # def bin(self, i, j):
    #     """Get the (i,j)'th bin"""
    #     # cdef size_t ii = cutil.pythonic_index(i, self.p2ptr().numBins())
    #     # cdef size_t jj = cutil.pythonic_index(j, self.p2ptr().numBins())
    #     return cutil.new_borrowed_cls(ProfileBin2D, & self.p2ptr().bin(i,j), self)

    def binIndexAt(self, x, y):
        """Get the bin index pair containing position (x,y)"""
        return self.p2ptr().binIndexAt(x, y)

    def binAt(self, x, y):
        """Get the bin containing position (x,y)"""
        # TODO: what's the problem with this direct mapping? Produces compile error re. no default constructor...
        #return cutil.new_borrowed_cls(ProfileBin2D, & self.p2ptr().binAt(x,y), self)
        # TODO: need out-of-range check to return None?
        return self.bin(self.binIndexAt(x,y))


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


    def mkScatter(self, usefocus=False, usestddev=False):
        """None -> Scatter3D.
        Convert this Profile2D to a Scatter3D, with z representing
        mean bin y values and their standard errors."""
        cdef c.Scatter3D s3 = c.mkScatter_Profile2D(deref(self.p2ptr()), usefocus, usestddev)
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
