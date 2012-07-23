cdef extern from "YODA/Profile2D.h" namespace "YODA":
    #cProfile2D operator + (cProfile2D &, cProfile2D &)
    #cProfile2D operator - (cProfile2D &, cProfile2D &)

    cdef cppclass cProfile2D "YODA::Profile2D"(cAnalysisObject):
        cProfile2D()
        cProfile2D(string& path, string& title)
        cProfile2D(size_t nbinsX, double lowerX, double upperX,
                   size_t nbinsY, double lowerY, double upperY,
                   string& path, string& title)
        cProfile2D(vector[cProfileBin2D]&, string& path, string& title)
        cProfile2D(vector[double]& xedges, vector[double]& yedges, string& path, string& title)
        cProfile2D(cProfile2D& h)

        void fill(double x, double y, double z, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleXY(double scaleX, double scaleY)
        # void mergeBins(size_t a, size_t b)
        # void rebin(int a, int b)

        # Bin Accessors
        size_t numBins()
        size_t numBinsX()
        size_t numBinsY()

        double lowEdgeX()
        double lowEdgeY()
        double highEdgeX()
        double highEdgeY()
        cDbn3D& outflow(int ix, int iy)
        cDbn3D& totalDbn()

        vector[cProfileBin2D]& bins()
        int findBinIndex(double coordX, double coordY)
        cProfileBin2D& binByCoord(double x, double y)
        cProfileBin2D& bin(size_t i)
        void eraseBin(size_t i)

        # Statistical functions
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)

        double xMean(bool includeoverflows)
        double yMean(bool includeoverflows)

        double xVariance(bool includeoverflows)
        double yVariance(bool includeoverflows)

        double xStdDev(bool includeoverflows)
        double yStdDev(bool includeoverflows)

        double xStdErr(bool includeoverflows)
        double yStdErr(bool includeoverflows)


cdef class Profile2D(AnalysisObject):
    """
    2D profile histogram. Basic histogramming is currently supported, i.e. a
    two-dimensional bin layout may be specified, filled, and queried. Outflow
    distributions around the 8 sides and corners of the grid are supported, and
    the bins need not be regularly sized or contiguous: bins which span rows
    and/or columns, and gaps between bins are permitted.

    TODO: Bin merging/rebinning is not currently supported (the algorithm has to
    be a bit more complex than usual due to the increased number of ways for it
    to not work if the bin shapes are irregular.)

    Rescaling of weights and/or the x and y axes is permitted in-place: the result
    is a still-valid Profile2D. Binning-compatible 2D histograms may be
    divided, resulting in a Scatter3D rather than a Profile2D, since further
    fills would not be meaningful.

    Several sets of arguments are permitted to the constructor:

    * Profile2D() -- default constructor. Not usually useful in Python, due to availability of None.
    * Profile2D(bins[, path, title]) -- constructor from a list of bins.
    * Profile2D(nx, xlow, xhigh, ny, ylow, yhigh[, path, title]) -- construct a uniform bin grid in both x and y directions.
    * Profile2D(xedges, yedges[, path, title]) -- make a regular bin grid with given bin edges in x and y.

    The path and title arguments are optional, and may either be specified via the
    positional parameters or via explicit keyword arguments, e.g. path='/foo/bar'.

    TODO: Add constructors from Scatter and Histo2D
    TODO: Support adding and erasing bins
    TODO: Support add, mul, div operators
    """

    def __cinit__(self):
        self._dealloc = False


    def __init__(self, *args, **kwargs):
        cdef:
            size_t nbinsX, nbinsY
            double lowX, highX, lowY, highY
            ProfileBin2D item
            vector[cProfileBin2D] bin_vector
            vector[double] xedges
            vector[double] yedges
            char* path = '/'
            char* title = ''

        ## Permit path and title specification via kwargs
        if "path" in kwargs:
            path = kwargs["path"]
        if "title" in kwargs:
            title = kwargs["title"]

        # TODO: Map copy constructor

        if len(args) == 0:
            self.setptr(new cProfile2D(string(path), string(title)), True)
        elif len(args) == 1:
            # Profile2D(bins[, path, title])
            for b in args[0]:
                item = b
                bin_vector.push_back( item.ptr()[0] )
            self.setptr(new cProfile2D(bin_vector, string(path), string(title)))
        elif len(args) == 2:
            # Profile2D(xedges, yedges[, path, title])
            for x in args[0]:
                xedges.push_back(x)
            for y in args[1]:
                yedges.push_back(y)
            self.setptr(new cProfile2D(xedges, yedges, string(path), string(title)))
        elif len(args) == 6:
            # Profile2D(nx, xlow, xhigh, ny, ylow, yhigh[, path, title]) -- construct a uniform bin grid in both x and y directions.
            nbinsX, lowX, highX, nbinsY, lowY, highY = args
            self.setptr(new cProfile2D(nbinsX, lowX, highX, nbinsY, lowY, highY,
                                       string(path), string(title)), True)


    cdef cProfile2D* ptr(self):
        return <cProfile2D *> self.thisptr


    def fill(self, double x, double y, double z, double weight=1.0):
        "Fill the bin at (x, y) with the given z value and weight"
        self.ptr().fill(x, y, z, weight)


    def reset(self):
        """Reset the histogram counters but leave the bin structure"""
        self.ptr().reset()


    def scaleW(self, double factor):
        """Scale the histogram and its statistics by the given factor"""
        self.ptr().scaleW(factor)


    # def mergeBins(self, size_t a, size_t b):
    #     self.ptr().mergeBins(a, b)


    # def rebin(self, int a, int b):
    #     self.ptr().rebin(a, b)


    @property
    def bins(self):
        "Access the bins"
        cdef size_t i
        return tuple(ProfileBin2D_fromptr(&self.ptr().bin(i)) for i in xrange(self.ptr().numBins()))

    @property
    def lowEdgeX(self):
        "The lower bound of the histogram bin range in x"
        return self.ptr().lowEdgeX()


    @property
    def lowEdgeY(self):
        "The lower bound of the histogram bin range in y"
        return self.ptr().lowEdgeY()


    @property
    def highEdgeX(self):
        "The upper bound of the histogram bin range in x"
        return self.ptr().highEdgeX()


    @property
    def highEdgeY(self):
        "The upper bound of the histogram bin range in y"
        return self.ptr().highEdgeY()


    @property
    def totalDbn(self):
        """
        h.totalDbn -> Dbn3D

        Return the Dbn3D object representing the total distribution.

        """
        return Dbn3D_fromptr(&self.ptr().totalDbn())


    def outflow(self, ix, iy):
        """
        h.outflow(ix, iy) -> Dbn3D

        Return the Dbn3D object representing the outflow in the (ix,iy)
        direction, where ix, iy = {-1, 0, 1}.
        """
        return Dbn3D_fromptr(&self.ptr().outflow(ix, iy))


    def __delitem__(self, size_t ix):
        self.ptr().eraseBin(ix)


    def __getitem__(self, size_t ix):
        return ProfileBin2D().set(self.ptr().bins()[ix])


    def sumW(self, bool overflows=True):
        """
        h.sumW([bool overflows=True] -> float

        Get the weight integral of the histo, with an optional bool argument to
        decide whether the bin-range overflows are to be included.
        """
        return self.ptr().sumW(overflows)


    def sumW2(self, bool overflows=True):
        """
        h.sumW2([bool overflows=True] -> float

        Get the weight**2 integral of the histo, with an optional bool argument
        to decide whether the bin-range overflows are to be included.
        """
        return self.ptr().sumW2(overflows)


    def mean(self, bool overflows=True):
        """
        h.mean([bool overflows=True] -> (float, float)

        Get the mean (x,y) point for this histo, with an optional bool argument
        to decide whether the bin-range overflows are to be included.
        """
        cdef cProfile2D *s = self.ptr()
        return (s.xMean(overflows), s.yMean(overflows))


    def variance(self, bool overflows=True):
        """
        h.variance([bool overflows=True] -> (float, float)

        Get the (x,y) variances for this histo, with an optional bool argument
        to decide whether the bin-range overflows are to be included.
        """
        cdef cProfile2D *s = self.ptr()
        return (s.xVariance(overflows), s.yVariance(overflows))


    def stdDev(self, bool overflows=True):
        """
        h.stdDev([bool overflows=True] -> (float, float)

        Get the (x,y) standard deviations for this histo, with an optional bool
        argument to decide whether the bin-range overflows are to be included.
        """
        cdef cProfile2D *s = self.ptr()
        return (s.xStdDev(overflows), s.yStdDev(overflows))


    def stdErr(self, bool overflows=True):
        """
        h.stdErr([bool overflows=True] -> (float, float)

        Get the (x,y) standard errors on the mean for this histo, with an
        optional bool argument to decide whether the bin-range overflows are to
        be included.
        """
        cdef cProfile2D *s = self.ptr()
        return (s.xStdErr(overflows), s.yStdErr(overflows))



    # def __add__(Profile2D a, Profile2D b):
    #     cdef cProfile2D *res
    #     res = new cProfile2D(a.ptr()[0] + b.ptr()[0])
    #     return Profile2D().setptr(res)


    # def __sub__(Profile2D a, Profile2D b):
    #     cdef cProfile2D *res
    #     res = new cProfile2D(a.ptr()[0] - b.ptr()[0])
    #     return Profile2D().setptr(res)


    # def __mul__(x, y):
    #     cdef cProfile2D *res
    #     tx, ty = type(x), type(y)
    #     if (tx is int or tx is float) and ty is Profile2D:
    #         histo = <Profile2D> y; factor = <Profile2D> x
    #     elif tx is Profile2D and (ty is int or ty is float):
    #         histo = <Profile2D> x; factor = <Profile2D> y
    #     else:
    #         raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

    #     res = new cProfile2D(histo.ptr()[0])
    #     res.scaleW(factor)
    #     return Profile2D().setptr(res)


    def __repr__(self):
        return 'Profile2D%r' % self.bins


cdef Profile2D Profile2D_fromptr(cProfile2D *ptr, dealloc=False):
    # Construct a Python Profile2D from a pointer to a cProfile2D,
    # without calling __init__ and excessive memory allocation
    cdef Profile2D bin = Profile2D.__new__(Profile2D)
    return bin.setptr(ptr, dealloc)
