
cdef class Histo1D(AnalysisObject):
    """
    1D histogram, with distinction between bin areas and heights.

    Complete histogram binning is supported, including uniform/regular binning,
    variable-width binning, unbinned gaps in the covered range, and
    under/overflows. Rebinning by integer factors, or by explicit merging of
    contiguous bins is also supported.

    Rescaling of weights and/or the x axis is permitted in-place: the result is
    still a valid Histo1D. Binning-compatible 1D histograms may be divided,
    resulting in a Scatter2D since further fills would not be meaningful.

    Several sets of arguments are tried by the constructor in the
    following order.

    Histo1D(path="", title="").
      Construct a histogram with optional path and title but no bins.

    Histo1D(nbins, low, high, path="", title="")
      Construct a histogram with optional path and title, and nbins bins
      uniformly distributed between low and high.

    Histo1D(B, path="", title="").
      Construct a histogram with optional path and title, from an
      iterator of bins, B.
    """

    cdef inline c.Histo1D* h1ptr(self) except NULL:
        return <c.Histo1D*> self.ptr()
    # TODO: remove
    cdef inline c.Histo1D* _Histo1D(self) except NULL:
        return <c.Histo1D*> self.ptr()


    def __init__(self, *args, **kwargs):
        util.try_loop([self.__init2, self.__init5, self.__init3], *args, **kwargs)

    def __init2(self, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Histo1D(string(path), string(title)))

    # TODO: Is Cython clever enough that we can make 3a and 3b versions and let it do the type inference?
    def __init3(self, bins_or_edges, char *path="", char *title=""):
        # TODO: Do this type-checking better
        cdef vector[double] edges
        try:
            ## If float conversions work for all elements, it's a list of edges:
            edges = list(float(x) for x in bins_or_edges)
            cutil.set_owned_ptr(self, new c.Histo1D(edges, string(path), string(title)))
        except:
            ## Assume it's a list of HistoBin1D
            bins = bins_or_edges
            self.__init2(path, title)
            self.addBins(bins)

    def __init5(self, nbins, low, high, char *path="", char *title=""):
        cutil.set_owned_ptr(self, new c.Histo1D(nbins, low, high, string(path), string(title)))


    def __len__(self):
        return self.h1ptr().numBins()

    def __getitem__(self, py_ix):
        cdef size_t i = cutil.pythonic_index(py_ix, self.h1ptr().numBins())
        return cutil.new_borrowed_cls(HistoBin1D, & self.h1ptr().bin(i), self)


    def __repr__(self):
        xmean = None
        if self.sumW() != 0:
            xmean = "%0.2e" % self.xMean()
        return "<%s '%s' %d bins, sumw=%0.2g, xmean=%s>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW(), xmean)


    def reset(self):
        """None -> None.
        Reset the histogram but leave the bin structure."""
        self.h1ptr().reset()


    def clone(self):
        """None -> Histo1D.
        Clone this Histo1D."""
        return cutil.new_owned_cls(Histo1D, self.h1ptr().newclone())


    def fill(self, x, weight=1.0):
        """(x,[w]) -> None.
        Fill with given x value and optional weight."""
        self.h1ptr().fill(x, weight)


    def fillBin(self, size_t ix, weight=1.0):
        """(ix,[w]) -> None.
        Fill bin ix and optional weight."""
        self.h1ptr().fillBin(ix, weight)


    @property
    def totalDbn(self):
        """None -> Dbn1D
        The Dbn1D representing the total distribution."""
        return cutil.new_borrowed_cls(Dbn1D, &self.h1ptr().totalDbn(), self)

    @property
    def underflow(self):
        """None -> Dbn1D
        The Dbn1D representing the underflow distribution."""
        return cutil.new_borrowed_cls(Dbn1D, &self.h1ptr().underflow(), self)

    @property
    def overflow(self):
        """None -> Dbn1D
        The Dbn1D representing the overflow distribution."""
        return cutil.new_borrowed_cls(Dbn1D, &self.h1ptr().overflow(), self)


    def integral(self, overflows=True):
        """([bool]) -> float
        Histogram integral, optionally excluding the overflows."""
        return self.h1ptr().integral(overflows)

    def numEntries(self): # add overflows arg
        """None -> int
        Number of times this histogram was filled."""
        return int(self.h1ptr().numEntries())

    def effNumEntries(self): # add overflows arg
        """None -> float
        Effective number of times this histogram was filled, computed from weights."""
        return self.h1ptr().effNumEntries()

    def sumW(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram, optionally excluding the overflows."""
        return self.h1ptr().sumW(overflows)

    def sumW2(self, overflows=True):
        """([bool]) -> float
        Sum of weights filled into this histogram, optionally excluding the overflows."""
        return self.h1ptr().sumW2(overflows)

    def xMean(self, overflows=True):
        """([bool]) -> float
        Mean x of the histogram, optionally excluding the overflows."""
        return self.h1ptr().xMean(overflows)

    def xVariance(self, overflows=True):
        """([bool]) -> float
        Variance in x of the histogram, optionally excluding the overflows."""
        return self.h1ptr().xVariance(overflows)

    def xStdDev(self, overflows=True):
        """([bool]) -> float
        Standard deviation in x of the histogram, optionally excluding the overflows."""
        return self.h1ptr().xStdDev(overflows)

    def xStdErr(self, overflows=True):
        """([bool]) -> float
        Standard error on the mean x of the histogram, optionally excluding the overflows."""
        return self.h1ptr().xStdErr(overflows)


    def scaleW(self, w):
        """ (float) -> None.
        Rescale the weights in this histogram by the factor w."""
        self.h1ptr().scaleW(w)

    def normalize(self, normto=1.0, overflows=True):
        """ (float, bool) -> None.
        Normalize the histogram."""
        self.h1ptr().normalize(normto, overflows)


    @property
    def numBins(self):
        """() -> int
        Number of bins (not including overflows)."""
        return self.h1ptr().numBins()

    @property
    def xMin(self):
        """Low x edge of the histo."""
        return self.h1ptr().xMin()

    @property
    def xMax(self):
        """High x edge of the histo."""
        return self.h1ptr().xMax()

    @property
    def bins(self):
        """Access the ordered bins list."""
        return list(self)

    def mergeBins(self, ia, ib):
        """mergeBins(ia, ib) -> None.
        Merge bins from indices ia through ib."""
        self.h1ptr().mergeBins(ia, ib)

    def rebin(self, n):
        """(n) -> None.
        Merge every group of n bins together."""
        self.h1ptr().rebin(n)

    def addBin(self, low, high):
        """(low, high) -> None.
        Add a bin."""
        self.h1ptr().addBin(low, high)

    def addBins(self, edges_or_bins):
        """Add several bins."""
        # TODO: simplify / make consistent
        arg = list(edges_or_bins)
        util.try_loop([self.__addBins_edges,
                       self.__addBins_tuples,
                       self.__addBins_points,
                       self.__addBins_bins], arg)

    def __addBins_edges(self, edges):
        cdef vector[double] cedges
        for edge in edges:
            cedges.push_back(edge)
        if len(edges):
            self.h1ptr().addBins(cedges)

    def __addBins_bins(self, bins):
        self.__addBins_tuples(imap(attrgetter('edges'), bins))

    def __addBins_points(self, points):
        self.__addBins_tuples(imap(attrgetter('xRange'), points))

    def __addBins_tuples(self, tuples):
        cdef double a, b
        for a, b in tuples:
            self.h1ptr().addBin(a, b)


    def mkScatter(self):
        """None -> Scatter2D.
        Convert this Histo1D to a Scatter2D, with y representing bin heights
        (not sumW) and height errors."""
        cdef c.Scatter2D s2 = c.mkScatter_Histo1D(deref(self.h1ptr()))
        return cutil.new_owned_cls(Scatter2D, s2.newclone())


    def toIntegral(self, efficiency=False, includeunderflow=True, includeoverflow=True):
        """None -> Scatter2D.
        Convert this Histo1D to a Scatter2D representing an integral (i.e. cumulative)
        histogram constructed from this differential one.

        The efficiency argument is used to construct an 'efficiency integral' histogram
        and the includeXXXflow bools determine whether under and overflows are included
        in computing the (efficiency) integral.
        """
        cdef c.Scatter2D s2
        if not efficiency:
            s2 = c.Histo1D_toIntegral(deref(self.h1ptr()), includeunderflow)
        else:
            s2 = c.Histo1D_toIntegralEff(deref(self.h1ptr()), includeunderflow, includeoverflow)
        return cutil.new_owned_cls(Scatter2D, s2.newclone())


    def divide(self, Histo1D h1, efficiency=False):
        # if type(h1) is not Histo1D:
        #     raise ValueError("Histograms must be of the same type to be divided")
        cdef c.Scatter2D s2
        if not efficiency:
            s2 = c.Histo1D_div_Histo1D(deref(self.h1ptr()), deref(h1.h1ptr()))
        else:
            s2 = c.Histo1D_eff_Histo1D(deref(self.h1ptr()), deref(h1.h1ptr()))
        return cutil.new_owned_cls(Scatter2D, s2.newclone())


    ## In-place special methods

    def __iadd__(Histo1D self, Histo1D other):
        c.Histo1D_iadd_Histo1D(self.h1ptr(), other.h1ptr())
        return self

    def __isub__(Histo1D self, Histo1D other):
        c.Histo1D_isub_Histo1D(self.h1ptr(), other.h1ptr())
        return self

    # def __imul__(Histo1D self, double x):
    #     c.Histo1D_imul_dbl(self.h1ptr(), x)
    #     return self

    # def __idiv__(Histo1D self, double x):
    #     c.Histo1D_idiv_dbl(self.h1ptr(), x)
    #     return self


    ## Unbound special methods

    def __add__(Histo1D self, Histo1D other):
        h = Histo1D()
        cutil.set_owned_ptr(h, c.Histo1D_add_Histo1D(self.h1ptr(), other.h1ptr()))
        return h

    # TODO: Cython doesn't support type overloading for special functions?
    # def __add__(Histo1D self, int x):
    #     """
    #     Special operator support to allow use of sum(histos) which starts from 0.
    #     """
    #     assert(x == 0)
    #     return self

    # TODO: Cython doesn't support type overloading for special functions?
    # def __radd__(Histo1D self, int x):
    #     """
    #     Special operator support to allow use of sum(histos) which starts from 0.
    #     """
    #     assert(x == 0)
    #     return self

    def __sub__(Histo1D self, Histo1D other):
        h = Histo1D()
        cutil.set_owned_ptr(h, c.Histo1D_sub_Histo1D(self.h1ptr(), other.h1ptr()))
        return h

    # def __mul__(Histo1D self, double x):
    #     h = c.Histo1D_mul_dbl(self.h1ptr(), x)
    #     return h
    # def __rmul__(Histo1D self, double x):
    #     h = c.Histo1D_mul_dbl(self.h1ptr(), x)
    #     return h

    def __div__(Histo1D self, Histo1D other):
        h = Histo1D()
        cutil.set_owned_ptr(h, c.Histo1D_div_Histo1D(self.h1ptr(), other.h1ptr()))
        return h
