
cdef class Scatter2D(AnalysisObject):
    """
    1D histogram. Complete histogramming is supported, including
    uniform/regular binning, variable wideth bininng, unbinned gaps in
    the covered range, and under/overflows. Rebinning by integer
    factors, or by explicit merging of contiguous bins is also
    supported.

    Rescaling of weights and/or the x axis is permitted in-place: the
    result is still a valid Scatter2D. Binning-compatible 1D histograms
    may be divided, resulting in a Scatter2D since further fulls would
    not be meaningful.

    Several sets of arguments are tried by the constructor in the
    following order.

    Scatter2D(path="", title=""). Construct a histogram with optional path
    and title but no bins.

    Scatter2D(B, path="", title=""). Construct a histogram from an
    iterator of bins, B.

    Scatter2D(E, path="", title=""). Construct a histogram from an
    iterator of edges, E.

    Should this constructor fail, then 
    Scatter2D(nbins, low, high, path="", title="")
    """

    # Is there a reason why this sounds like an advert? I'M ALREADY SOLD
    # DAMMIT!


    cdef inline c.Scatter2D *_Scatter2D(self) except NULL:
        return <c.Scatter2D*> self.ptr()

    # There is a pythonic constructor here, and it looks a little like...
    # __init__(self, *args, **kwargs)
    # ([edge], path="", title="")
    # ([bins], **kwargs)

    def __init__(self, *args, **kwargs):
        #try_loop([self.__init2, self.__init5, self.__init3], *args, **kwargs)
        util.try_loop([self.__init_2, self.__init_3], *args, **kwargs)

    def __init_2(self, char* path="", char* title=""):
        util.set_owned_ptr(self, new c.Scatter2D(string(path), string(title)))

    def __init_3(self, points, char* path="", char* title=""):
        self.__init_2(path, title)
        self.addPoints(points)

    def __len__(self):
        return self._Scatter2D().numPoints()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(
            py_ix, self._Scatter2D().numPoints())

        return util.new_borrowed_cls(
            Point2D, & self._Scatter2D().point(i), self)

    def copy(self, char *path=""):
        """(path="") -> Scatter2D. Clone this Scatter2D with optional new path."""
        return util.new_owned_cls(Scatter2D,
            new c.Scatter2D(deref(self._Scatter2D()), string(path)))

    def addPoints(self, iterable):
        for row in iterable:
            self.addPoint(*row)

    def addPoint(self, *args, **kwargs):
        try:
            self.__addPoint_point(*args, **kwargs)
        except TypeError:
            self.__addPoint_explicit(*args, **kwargs)

    def __addPoint_explicit(self, x, y, xerrs=0, yerrs=0):
        self.__addPoint_point(Point2D(x, y, xerrs, yerrs))

    def __addPoint_point(self, Point2D p):
        self._Scatter2D().addPoint(p._Point2D()[0])

    def combineWith(self, others):
        cdef Scatter2D other
        try:
            # Can we type it as a scatter2D?
            other = others
        except TypeError:
            # Could be an iterable...
            for other in others:
                self._Scatter2D().combineWith(deref(other._Scatter2D()))
        else:
            self._Scatter2D().combineWith(deref(other._Scatter2D()))
