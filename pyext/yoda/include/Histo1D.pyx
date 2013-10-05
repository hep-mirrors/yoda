
cdef class Histo1D(AnalysisObject):
    """
    1D histogram. Complete histogramming is supported, including
    uniform/regular binning, variable-width bininng, unbinned gaps in
    the covered range, and under/overflows. Rebinning by integer
    factors, or by explicit merging of contiguous bins is also
    supported.

    Rescaling of weights and/or the x axis is permitted in-place: the
    result is still a valid Histo1D. Binning-compatible 1D histograms
    may be divided, resulting in a Scatter2D since further fills would
    not be meaningful.

    Several sets of arguments are tried by the constructor in the
    following order.

    Histo1D(path="", title=""). Construct a histogram with optional path
    and title but no bins.

    Histo1D(B, path="", title=""). Construct a histogram from an
    iterator of bins, B.

    Histo1D(E, path="", title=""). Construct a histogram from an
    iterator of edges, E.

    Should this constructor fail, then
    Histo1D(nbins, low, high, path="", title="")
    """

    cdef inline c.Histo1D* _Histo1D(self) except NULL:
        return <c.Histo1D*> self.ptr()


    # There is a pythonic constructor here, and it looks a little like...
    # __init__(self, *args, **kwargs)
    # ([edge], path="", title="")
    # ([bins], **kwargs)


    def __init__(self, *args, **kwargs):
        # Multimethod constructors are blatantly unpythonic. However,
        # doing something like the inbuilt collections do would be
        # really neat. So we will use an iterable of bins in place of
        # the "from other type" constructors, and have a copy() method
        # which has exactly the same semantics.
        util.try_loop([self.__init2, self.__init5, self.__init3], *args, **kwargs)

    def __init2(self, char *path="", char *title=""):
        util.set_owned_ptr(self, new c.Histo1D(string(path), string(title)))

    def __init5(self, nbins, low, high, char *path="", char *title=""):
        util.set_owned_ptr(self, new c.Histo1D(nbins, low, high, string(path), string(title)))

    def __init3(self, bins, char *path="", char *title=""):
        # Make an iterator over bins. We might as well make our code
        # general, as that increases pythonicity.
        self.__init2(path, title)
        self.addBins(bins)


    def __len__(self):
        return self._Histo1D().numBins()


    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(
            py_ix, self._Histo1D().numBins())

        return util.new_borrowed_cls(
            HistoBin1D, & self._Histo1D().bin(i), self)


    def __repr__(self):
        xmean = self.mean() if self.sumW() else None
        return "<%s '%s' %d bins, sumw=%0.2e, xmean=%0.2e>" % \
               (self.__class__.__name__, self.path,
                len(self.bins), self.sumW(), xmean)


    def fill(self, x, weight=1.0):
        """
        (x, weight=1.0) -> None. Fill with given x and optional weight.

        """
        self._Histo1D().fill(x, weight)

    # TODO: HACK HACK HACK HACK HACK
    def fill_many(self, xs, weight=1.0):
        """
        (x, weight=1.0) -> None. Fill with given x and optional weight.

        """
        for x in xs:
            self._Histo1D().fill(x, weight)

    def copy(self, char *path=""):
        """(path="") -> Histo1D. Clone this Histo1D with optional new path."""
        return util.new_owned_cls(Histo1D,
            new c.Histo1D(deref(self._Histo1D()), string(path)))

    @property
    def bins(self):
        return list(self)

    # @property
    # def edges(self):
    #     return util.Edges(self._Histo1D().lowEdge(),
    #                       self._Histo1D().highEdge())

    @property
    def totalDbn(self):
        """The Dbn1D representing the total distribution."""
        return util.new_borrowed_cls(Dbn1D, &self._Histo1D().totalDbn(), self)

    @property
    def underflow(self):
        """The Dbn1D representing the underflow distribution."""
        return util.new_borrowed_cls(Dbn1D, &self._Histo1D().underflow(), self)

    @property
    def overflow(self):
        """The Dbn1D representing the overflow distribution."""
        return util.new_borrowed_cls(Dbn1D, &self._Histo1D().overflow(), self)

    def integral(self, overflows=True):
        return self._Histo1D().integral(overflows)

    def sumW(self, overflows=True):
        return self._Histo1D().sumW(overflows)

    def sumW2(self, overflows=True):
        return self._Histo1D().sumW2(overflows)

    def mean(self, overflows=True):
        return self._Histo1D().mean(overflows)

    def variance(self, overflows=True):
        return self._Histo1D().variance(overflows)

    def stdDev(self, overflows=True):
        return self._Histo1D().stdDev(overflows)

    def stdErr(self, overflows=True):
        return self._Histo1D().stdErr(overflows)

    def reset(self):
        """
        Reset the histogram but leave the bin structure.
        """
        self._Histo1D().reset()

    def scaleW(self, w):
        """
        (float) -> None. Scale the histogram and its statistics as if all
        weights had been scaled by given factor.
        """
        self._Histo1D().scaleW(w)

    def normalize(self, normto=1.0, overflows=True):
        """
        (float, bool) -> None. Normalize the histogram.
        """
        self._Histo1D().normalize(normto, overflows)

    def mergeBins(self, ia, ib):
        """mergeBins(ia, ib) -> None. Merge bins from indices ia through ib."""
        self._Histo1D().mergeBins(ia, ib)

    def rebin(self, n):
        """(n) -> None. Merge every nth bin."""
        self._Histo1D().rebin(n)

    def addBin(self, low, high):
        """(low, high) -> None. Add a bin."""
        self._Histo1D().addBin(low, high)

    def addBins(self, edges_or_bins):
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
            self._Histo1D().addBins(cedges)

    def __addBins_bins(self, bins):
        self.__addBins_tuples(imap(attrgetter('edges'), bins))

    def __addBins_points(self, points):
        self.__addBins_tuples(imap(attrgetter('xRange'), points))

    def __addBins_tuples(self, tuples):
        cdef double a, b
        for a, b in tuples:
            self._Histo1D().addBin(a, b)


    ## In-place special methods

    def __iadd__(Histo1D self, Histo1D other):
        c.Histo1D_iadd_Histo1D(self._Histo1D(), other._Histo1D())
        return self
    def __isub__(Histo1D self, Histo1D other):
        c.Histo1D_isub_Histo1D(self._Histo1D(), other._Histo1D())
        return self
    # def __imul__(Histo1D self, double x):
    #     c.Histo1D_imul_dbl(self._Histo1D(), x)
    #     return self
    # def __idiv__(Histo1D self, double x):
    #     c.Histo1D_idiv_dbl(self._Histo1D(), x)
    #     return self


    ## Unbound special methods

    def __add__(Histo1D self, Histo1D other):
        h = Histo1D()
        util.set_owned_ptr(h, c.Histo1D_add_Histo1D(self._Histo1D(), other._Histo1D()))
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
        util.set_owned_ptr(h, c.Histo1D_sub_Histo1D(self._Histo1D(), other._Histo1D()))
        return h

    # def __mul__(Histo1D self, double x):
    #     h = c.Histo1D_mul_dbl(self._Histo1D(), x)
    #     return h
    # def __rmul__(Histo1D self, double x):
    #     h = c.Histo1D_mul_dbl(self._Histo1D(), x)
    #     return h
