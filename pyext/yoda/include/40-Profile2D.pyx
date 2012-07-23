# cdef extern from "YODA/Histo2D.h" namespace "YODA":
#     #cHisto2D operator + (cHisto2D &, cHisto2D &)
#     #cHisto2D operator - (cHisto2D &, cHisto2D &)

#     cdef cppclass cHisto2D "YODA::Histo2D"(cAnalysisObject):
#         cHisto2D()
#         cHisto2D(string& path, string& title)
#         cHisto2D(size_t nbinsX, double lowerX, double upperX,
#                  size_t nbinsY, double lowerY, double upperY,
#                  string& path, string& title)
#         cHisto2D(vector[cHistoBin2D]&, string& path, string& title)
#         cHisto2D(vector[double]& xedges, vector[double]& yedges, string& path, string& title)
#         cHisto2D(cHisto2D& h)

#         void fill(double x, double y, double weight)
#         void reset()
#         void scaleW(double scalefactor)
#         void scaleXY(double scaleX, double scaleY)
#         # void mergeBins(size_t a, size_t b)
#         # void rebin(int a, int b)

#         # Bin Accessors
#         size_t numBins()
#         size_t numBinsX()
#         size_t numBinsY()

#         double lowEdgeX()
#         double lowEdgeY()
#         double highEdgeX()
#         double highEdgeY()
#         cDbn2D& outflow(int ix, int iy)
#         cDbn2D& totalDbn()

#         vector[cHistoBin2D]& bins()
#         cHistoBin2D& binByCoord(double x, double y)

#         cHistoBin2D& bin(size_t i)
#         void eraseBin(size_t i)

#         # Statistical functions
#         double integral(bool includeoverflows)
#         double sumW(bool includeoverflows)
#         double sumW2(bool includeoverflows)

#         double xMean(bool includeoverflows)
#         double yMean(bool includeoverflows)

#         double xVariance(bool includeoverflows)
#         double yVariance(bool includeoverflows)

#         double xStdDev(bool includeoverflows)
#         double yStdDev(bool includeoverflows)

#         double xStdErr(bool includeoverflows)
#         double yStdErr(bool includeoverflows)


# cdef class Histo2D(AnalysisObject):
#     """
#     2D histogram. Basic histogramming is currently supported, i.e. a two-dimensional bin layout
#     may be specified, filled, and queried. Outflow distributions around the 8 sides and
#     corners of the grid are supported, and the bins need not be regularly sized or contiguous:
#     bins which span rows and/or columns, and gaps between bins are permitted.

#     TODO: Bin merging/rebinning is not currently supported (the algorithm has to
#     be a bit more complex than usual due to the increased number of ways for it
#     to not work if the bin shapes are irregular.)

#     Rescaling of weights and/or the x and y axes is permitted in-place: the result
#     is a still-valid Histo2D. Binning-compatible 2D histograms may be
#     divided, resulting in a Scatter3D rather than a Histo2D, since further
#     fills would not be meaningful.

#     Several sets of arguments are permitted to the constructor:

#     * Histo2D() -- default constructor. Not usually useful in Python, due to availability of None.
#     * Histo2D(bins[, path, title]) -- constructor from a list of bins.
#     * Histo2D(nx, xlow, xhigh, ny, ylow, yhigh[, path, title]) -- construct a uniform bin grid in both x and y directions.
#     * Histo2D(xedges, yedges[, path, title]) -- make a regular bin grid with given bin edges in x and y.

#     The path and title arguments are optional, and may either be specified via the
#     positional parameters or via explicit keyword arguments, e.g. path='/foo/bar'.

#     TODO: Add constructors from Scatter and Profile
#     TODO: Support adding and erasing bins
#     TODO: Support add, mul, div operators
#     """

#     def __cinit__(self):
#         self._dealloc = False


#     def __init__(self, *args, **kwargs):
#         cdef:
#             size_t nbinsX, nbinsY
#             double lowX, highX, lowY, highY
#             HistoBin2D item
#             vector[cHistoBin2D] bin_vector
#             vector[double] xedges
#             vector[double] yedges
#             char* path = '/'
#             char* title = ''

#         ## Permit path and title specification via kwargs
#         if "path" in kwargs:
#             path = kwargs["path"]
#         if "title" in kwargs:
#             title = kwargs["title"]

#         # TODO: Map copy constructor

#         if len(args) == 0:
#             self.setptr(new cHisto2D(string(path), string(title)), True)
#         elif len(args) == 1:
#             # Histo2D(bins[, path, title])
#             for b in args[0]:
#                 item = b
#                 bin_vector.push_back( item.ptr()[0] )
#             self.setptr(new cHisto2D(bin_vector, string(path), string(title)))
#         elif len(args) == 2:
#             # Histo2D(xedges, yedges[, path, title])
#             for x in args[0]:
#                 xedges.push_back(x)
#             for y in args[1]:
#                 yedges.push_back(y)
#             self.setptr(new cHisto2D(xedges, yedges, string(path), string(title)))
#         elif len(args) == 6:
#             # Histo2D(nx, xlow, xhigh, ny, ylow, yhigh[, path, title]) -- construct a uniform bin grid in both x and y directions.
#             nbinsX, lowX, highX, nbinsY, lowY, highY = args
#             self.setptr(new cHisto2D(nbinsX, lowX, highX, nbinsY, lowY, highY,
#                                      string(path), string(title)), True)


#     cdef cHisto2D* ptr(self):
#         return <cHisto2D *> self.thisptr


#     def fill(self, double x, double y, double weight=1.0):
#         "Fill the bin at (x, y) with the given weight"
#         self.ptr().fill(x, y, weight)


#     def reset(self):
#         """Reset the histogram counters but leave the bin structure"""
#         self.ptr().reset()


#     def scaleW(self, double factor):
#         """Scale the histogram and its statistics by the given factor"""
#         self.ptr().scaleW(factor)


#     # def mergeBins(self, size_t a, size_t b):
#     #     self.ptr().mergeBins(a, b)


#     # def rebin(self, int a, int b):
#     #     self.ptr().rebin(a, b)


#     @property
#     def bins(self):
#         "Access the bins"
#         cdef size_t i
#         return tuple(HistoBin2D_fromptr(&self.ptr().bin(i)) for i in xrange(self.ptr().numBins()))

#     @property
#     def lowEdgeX(self):
#         "The lower bound of the histogram bin range in x"
#         return self.ptr().lowEdgeX()


#     @property
#     def lowEdgeY(self):
#         "The lower bound of the histogram bin range in y"
#         return self.ptr().lowEdgeY()


#     @property
#     def highEdgeX(self):
#         "The upper bound of the histogram bin range in x"
#         return self.ptr().highEdgeX()


#     @property
#     def highEdgeY(self):
#         "The upper bound of the histogram bin range in y"
#         return self.ptr().highEdgeY()


#     @property
#     def totalDbn(self):
#         """
#         h.totalDbn -> Dbn2D

#         Return the Dbn2D object representing the total distribution.

#         """
#         return Dbn2D_fromptr(&self.ptr().totalDbn())


#     def outflow(self, ix, iy):
#         """
#         h.outflow(ix, iy) -> Dbn2D

#         Return the Dbn2D object representing the outflow in the (ix,iy)
#         direction, where ix, iy = {-1, 0, 1}.
#         """

#         return Dbn2D_fromptr(&self.ptr().outflow(ix, iy))


#     def __delitem__(self, size_t ix):
#         self.ptr().eraseBin(ix)


#     def __getitem__(self, size_t ix):
#         return HistoBin2D().set(self.ptr().bins()[ix])


#     def integral(self, bool overflows=True):
#         """
#         h.integral([bool overflows=True] -> float

#         Get the integral of the histo, with an optional bool argument to decide
#         whether the bin-range overflows are to be included. Effectively a
#         synonym for sumW.
#         """
#         return self.ptr().integral(overflows)


#     def sumW(self, bool overflows=True):
#         """
#         h.sumW([bool overflows=True] -> float

#         Get the weight integral of the histo, with an optional bool argument to
#         decide whether the bin-range overflows are to be included.
#         """
#         return self.ptr().sumW(overflows)


#     def sumW2(self, bool overflows=True):
#         """
#         h.sumW2([bool overflows=True] -> float

#         Get the weight**2 integral of the histo, with an optional bool argument
#         to decide whether the bin-range overflows are to be included.
#         """
#         return self.ptr().sumW2(overflows)


#     def mean(self, bool overflows=True):
#         """
#         h.mean([bool overflows=True] -> (float, float)

#         Get the mean (x,y) point for this histo, with an optional bool argument
#         to decide whether the bin-range overflows are to be included.
#         """
#         cdef cHisto2D *s = self.ptr()
#         return (s.xMean(overflows), s.yMean(overflows))


#     def variance(self, bool overflows=True):
#         """
#         h.variance([bool overflows=True] -> (float, float)

#         Get the (x,y) variances for this histo, with an optional bool argument
#         to decide whether the bin-range overflows are to be included.
#         """
#         cdef cHisto2D *s = self.ptr()
#         return (s.xVariance(overflows), s.yVariance(overflows))


#     def stdDev(self, bool overflows=True):
#         """
#         h.stdDev([bool overflows=True] -> (float, float)

#         Get the (x,y) standard deviations for this histo, with an optional bool
#         argument to decide whether the bin-range overflows are to be included.
#         """
#         cdef cHisto2D *s = self.ptr()
#         return (s.xStdDev(overflows), s.yStdDev(overflows))


#     def stdErr(self, bool overflows=True):
#         """
#         h.stdErr([bool overflows=True] -> (float, float)

#         Get the (x,y) standard errors on the mean for this histo, with an
#         optional bool argument to decide whether the bin-range overflows are to
#         be included.
#         """
#         cdef cHisto2D *s = self.ptr()
#         return (s.xStdErr(overflows), s.yStdErr(overflows))



#     # def __add__(Histo2D a, Histo2D b):
#     #     cdef cHisto2D *res
#     #     res = new cHisto2D(a.ptr()[0] + b.ptr()[0])
#     #     return Histo2D().setptr(res)


#     # def __sub__(Histo2D a, Histo2D b):
#     #     cdef cHisto2D *res
#     #     res = new cHisto2D(a.ptr()[0] - b.ptr()[0])
#     #     return Histo2D().setptr(res)


#     # def __mul__(x, y):
#     #     cdef cHisto2D *res
#     #     tx, ty = type(x), type(y)
#     #     if (tx is int or tx is float) and ty is Histo2D:
#     #         histo = <Histo2D> y; factor = <Histo2D> x
#     #     elif tx is Histo2D and (ty is int or ty is float):
#     #         histo = <Histo2D> x; factor = <Histo2D> y
#     #     else:
#     #         raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

#     #     res = new cHisto2D(histo.ptr()[0])
#     #     res.scaleW(factor)
#     #     return Histo2D().setptr(res)


#     def __repr__(self):
#         return 'Histo2D%r' % self.bins


# cdef Histo2D Histo2D_fromptr(cHisto2D *ptr, dealloc=False):
#     # Construct a Python Histo2D from a pointer to a cHisto2D,
#     # without calling __init__ and excessive memory allocation
#     cdef Histo2D bin = Histo2D.__new__(Histo2D)
#     return bin.setptr(ptr, dealloc)
# cdef extern from "YODA/Profile1D.h" namespace "YODA":

#     cdef cppclass cProfile1D "YODA::Profile1D"(cAnalysisObject):
#         cProfile1D()
#         cProfile1D(size_t nbins, double lower, double upper, string& path, string& title)
#         cProfile1D(vector[double]& binedges, string& path, string& title)
#         cProfile1D(cProfile1D& h, string& path)
#         cProfile1D(cProfile1D& h)

#         void fill(double x, double y, double weight)
#         void reset()
#         void scaleW(double scalefactor)
#         void mergeBins(size_t a, size_t b)
#         void rebin(int n)

#         # Bin Accessors
#         size_t numBins()
#         double lowEdge()
#         double highEdge()

#         vector[cProfileBin1D]& bins()
#         cProfileBin1D& bin(size_t i)
#         void eraseBin(size_t i)

#         cDbn2D& totalDbn()
#         cDbn2D& underflow()
#         cDbn2D& overflow()
#         void addBin(double low, double high)
#         void addBins(vector[double]& binedges)

#         # Statistical functions
#         #double integral(bool includeoverflows)
#         #double integral(size_t a, size_t b)
#         double sumW(bool includeoverflows)
#         double sumW2(bool includeoverflows)

#     cProfile1D add(cProfile1D &, cProfile1D &)
#     cProfile1D subtract(cProfile1D &, cProfile1D &)
#     cScatter2D divide(cProfile1D &, cProfile1D &)
#     #cScatter2D mkScatter(cProfile1D &)

# cdef extern from "YODA/Scatter2D.h" namespace "YODA":
#     cScatter2D mkScatter(cProfile1D &)



# cdef class Profile1D(AnalysisObject):
#     """
#     1D profile histogram. Complete histogramming is supported, including
#     uniform/regular binning, variable width binning, unbinned gaps in the
#     covered range, and under/overflows (including the gaps). Rebinning by
#     integer factors, or by explicit merging of contiguous bins is also
#     supported.

#     Rescaling of weights and/or the x axis is permitted in-place: the result
#     is a still-valid Profile1D. Binning-compatible 2D profiles may be
#     divided, resulting in a Scatter2D rather than a Profile1D, since further
#     fills would not be meaningful.

#     Several sets of arguments are permitted to the constructor:

#     * Profile1D() -- default constructor. Not usually useful in Python, due to availability of None.
#     * Profile1D(nbins, low, high[, path, title]) -- linear binning with n bins between low-high.
#     * Profile1D(binedges[, path, title]) -- explicit bin edges (no bin gaps)

#     The path and title arguments are optional, and may either be specified via the
#     positional parameters or via explicit keyword arguments, e.g. path='/foo/bar'.

#     TODO: Add constructors from Scatter and Histo
#     TODO: Support addBin, addBins, eraseBin
#     """

#     def __init__(self, *args, **kwargs):
#         self._dealloc = True
#         cdef:
#             size_t nbins
#             double lower
#             double upper
#             vector[double] binedges
#             char* path = '/'
#             char* title = ''

#         ## Permit path and title specification via kwargs
#         if "path" in kwargs:
#             path = kwargs["path"]
#         if "title" in kwargs:
#             path = kwargs["title"]

#         ## Trigger different C++ constructors depending on Python args
#         # TODO: Map copy constructors, esp. the path-resetting one
#         if len(args) == 0:
#             self.setptr(new cProfile1D())
#         else:
#             if type(args[0]) is list:
#                 try:
#                     for i in args[0]:
#                         binedges.push_back(float(i))
#                 except:
#                     raise Exception("Invalid binedges container supplied to Profile1D constructor")
#                 if len(args) >= 2:
#                     assert "path" not in kwargs
#                     path = args[1]
#                 if len(args) == 3:
#                     assert "title" not in kwargs
#                     path = args[2]
#                 if len(args) > 3:
#                     raise ValueError("Too many arguments supplied to Profile1D constructor with a binedge list first arg")
#                 self.setptr(new cProfile1D(binedges, string(path), string(title)))
#             else:
#                 assert len(args) >= 3
#                 nbins, lower, upper = args[0:3]
#                 if len(args) >= 4:
#                     assert "path" not in kwargs
#                     path = args[3]
#                 if len(args) == 5:
#                     assert "title" not in kwargs
#                     path = args[4]
#                 if len(args) > 5:
#                     raise ValueError("Too many arguments supplied to Profile1D constructor")
#                 self.setptr(new cProfile1D(nbins, lower, upper, string(path), string(title)))


#     cdef cProfile1D* ptr(self):
#         return <cProfile1D *> self.thisptr


#     def asScatter(self):
#         """
#         h.asScatter() -> Scatter2D

#         Return a 2D scatter data object from the profile's bins and heights.

#         """
#         cdef cScatter2D *s = new cScatter2D()
#         s[0] = mkScatter(self.ptr()[0])
#         return Scatter2D_fromptr(s, True)


#     def fill(self, double x, double y, double weight=1.0):
#         """
#         h.fill(x, y[, weight=1.0]) -> self

#         Fill the given profile with value y at position x, and optional weighting.

#         """
#         self.ptr().fill(x, y, weight)
#         return self


#     def reset(self):
#         """
#         h.reset() -> self

#         Reset the histogram but leave the bin structure

#         """
#         self.ptr().reset()
#         return self


#     def scaleW(self, double factor):
#         """
#         s.scaleW(factor) -> self

#         Scale the histogram and its statistics by given factor

#         """
#         self.ptr().scaleW(factor)
#         return self


#     def mergeBins(self, size_t a, size_t b):
#         """
#         s.mergeBins(from, to) -> self

#         Merge bins between indexes from `from` to `to`

#         """
#         self.ptr().mergeBins(a, b)


#     def rebin(self, int n):
#         """
#         s.rebin(n) -> self

#         Merge every nth bin in the current histogram.

#         """
#         self.ptr().rebin(n)


#     @property
#     def bins(self):
#         """
#         h.bins -> tuple(ProfileBin1D)

#         Access the bin objects of this histogram. Bin objects are mutable and
#         changes to the bin objects will be propagated back to the histogram
#         unless the copy() method is called on a bin.

#         """
#         cdef size_t i
#         # cdef ProfileBin1D bin
#         return tuple(ProfileBin1D_fromptr(& self.ptr().bin(i)) for i in xrange(self.ptr().numBins()))


#     @property
#     def lowEdge(self):
#         """
#         h.lowEdge -> float

#         The x-value of the lowest edge of the lowest bin of the histogram.

#         """
#         return self.ptr().lowEdge()


#     @property
#     def highEdge(self):
#         """
#         h.highEdge -> float

#         The x-value of the highest edge of the highest bin of the histogram.

#         """
#         return self.ptr().highEdge()


#     @property
#     def totalDbn(self):
#         """
#         h.totalDbn -> Distribution1D

#         Return the Distribution1D object representing the total distribution.

#         """
#         return Dbn2D_fromptr(&self.ptr().totalDbn())


#     @property
#     def underflow(self):
#         """
#         h.underflow -> Distribution1D

#         Return the Distribution1D object representing the underflow.

#         """
#         return Dbn2D_fromptr(&self.ptr().underflow())


#     @property
#     def overflow(self):
#         """
#         h.overflow -> Distribution1D

#         Return the Distribution1D object representing the overflow.

#         """
#         return Dbn2D_fromptr(&self.ptr().overflow())


#     # def __delitem__(self, size_t ix):
#     #     self.ptr().eraseBin(ix)


#     def __getitem__(self, size_t ix):
#         return ProfileBin1D_fromptr(& self.ptr().bin(ix))


#     def sumW(self, bool overflows=True):
#         """
#         s.sumW([overflows]) -> float

#         Return the sum of weights of the histogram. If overflows is False,
#         ignore over-and underflow bins.

#         """

#         return self.ptr().sumW(overflows)


#     def sumW2(self, bool overflows=True):
#         """
#         s.sumW2([overflows]) -> float

#         Return the sum of weights squared. If overflows is False, ignore
#         over-and underflow bins.

#         """
#         return self.ptr().sumW2(overflows)


#     def __add__(Profile1D a, Profile1D b):
#         cdef cProfile1D *res = new cProfile1D(add(a.ptr()[0], b.ptr()[0]))
#         return Profile1D_fromptr(res, True)


#     def __sub__(Profile1D a, Profile1D b):
#         cdef cProfile1D *res = new cProfile1D(subtract(a.ptr()[0], b.ptr()[0]))
#         return Profile1D_fromptr(res, True)


#     def __mul__(x, y):
#         """
#         Scalar multiplication. Equivalent to scaleW acting on a copy.

#         """
#         cdef cProfile1D *res
#         tx, ty = type(x), type(y)
#         if (tx is int or tx is float) and ty is Profile1D:
#             histo = <Profile1D> y; factor = <Profile1D> x
#         elif tx is Profile1D and (ty is int or ty is float):
#             histo = <Profile1D> x; factor = <Profile1D> y
#         else:
#             raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

#         res = new cProfile1D(histo.ptr()[0])
#         res.scaleW(factor)
#         return Profile1D_fromptr(res, True)


#     def _div_scalar(Profile1D x, double y):
#         if y == 0:
#             raise ArithmeticError('Profile1D: Divide by zero scalar')

#         cdef cProfile1D *res = new cProfile1D(x.ptr()[0])

#         res.scaleW(1.0 / y)
#         return Profile1D_fromptr(res, True)


#     def _div_profile(Profile1D x, Profile1D y):
#         cdef cScatter2D s = divide(x.ptr()[0], y.ptr()[0])
#         return Scatter2D_fromptr(&s)


#     def __div__(x, y):
#         """
#         Division by scalar (i.e. multiplication by reciprocal) or another
#         1D histogram.

#         """
#         tx = type(x); ty = type(y)
#         if tx is Profile1D:
#             if ty is int or ty is float:
#                 return x._div_scalar(y)
#             elif ty is Profile1D:
#                 return x._div_profile(y)
#         raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))


#     def __repr__(self):
#         return 'Profile1D%r' % self.bins


# cdef Profile1D Profile1D_fromptr(cProfile1D *ptr, bool dealloc=False):
#     cdef Profile1D profile = Profile1D.__new__(Profile1D)
#     return profile.setptr(ptr, dealloc)
