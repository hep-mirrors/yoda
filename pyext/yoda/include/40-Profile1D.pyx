cdef extern from "YODA/Profile1D.h" namespace "YODA":
    #cProfile1D operator + (cProfile1D &, cProfile1D &)
    #cProfile1D operator - (cProfile1D &, cProfile1D &)
    #cScatter2D operator / (cProfile1D &, cProfile1D &)"""

    cdef cppclass cProfile1D "YODA::Profile1D"(cAnalysisObject):
        cProfile1D(size_t nbins, double lower, double upper, string &path, string &title)
        cProfile1D(vector[double] &binedges, string &path, string &title)
        cProfile1D(vector[double] &binedges)
        cProfile1D(cProfile1D &h, string &path)
        cProfile1D(cProfile1D &h)

        void fill(double x, double weight)
        void reset()
        void scaleW(double scalefactor)
        void mergeBins(size_t a, size_t b)
        void rebin(int n)

        # Bin Accessors
        size_t numBins()
        double lowEdge()
        double highEdge()
        vector[cProfileBin1D] &bins()
        cProfileBin1D & bin "bin"(size_t i)
        cDbn2D &totalDbn()
        cDbn2D &underflow()
        cDbn2D &overflow()
        #addBin
        #void eraseBin(size_t index)

        # Statistical functions
        #double integral(bool includeoverflows)
        #double integral(size_t a, size_t b)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)


    cProfile1D add(cProfile1D &, cProfile1D &)
    cProfile1D subtract(cProfile1D &, cProfile1D &)
    cScatter2D divide(cProfile1D &, cProfile1D &)
    #cScatter2D mkScatter(cProfile1D &)

from cython.operator cimport dereference as deref


cdef class Profile1D(AnalysisObject):
    def __init__(self, *args, **kwargs):
        self._dealloc = True
        cdef:
            size_t nbins
            double lower
            double upper
            char* path = '/'
            char* title = ''

        if len(args) == 3:
            nbins, lower, upper = args[0], args[1], args[2]

            self.setptr(
                new cProfile1D(nbins, lower, upper, string(path), string(title))
            )
        else:
            raise ValueError('Profile1D: Expected 3 arguments')


    cdef cProfile1D* ptr(self):
        return <cProfile1D *> self.thisptr


    # def asScatter(self):
    #     """
    #     h.asScatter() -> Scatter2D

    #     Return a 2D scatter data object from the profile's bins and heights

    #     """
    #     cdef cScatter2D *s = new cScatter2D()
    #     s[0] = Scatter2D_mkScatter(self.ptr()[0])
    #     return Scatter2D_fromptr(s, True)


    def fill(self, double x, double weight=1.0):
        """
        h.fill(x[, weight=1.0]) -> self

        Fill the given histogram with value x and optional weighting

        """
        self.ptr().fill(x, weight)
        return self


    def reset(self):
        """
        h.reset() -> self

        Reset the histogram but leave the bin structure

        """
        self.ptr().reset()
        return self


    def scaleW(self, double factor):
        """
        s.scaleW(factor) -> self

        Scale the histogram and its statistics by given factor

        """
        self.ptr().scaleW(factor)
        return self


    def mergeBins(self, size_t a, size_t b):
        """
        s.mergeBins(from, to) -> self

        Merge bins between indexes from `from` to `to`

        """
        self.ptr().mergeBins(a, b)


    def rebin(self, int n):
        """
        s.rebin(n) -> self

        Merge every nth bin in the current histogram.

        """
        self.ptr().rebin(n)


    @property
    def bins(self):
        """
        h.bins -> tuple(ProfileBin1D)

        Access the bin objects of this histogram. Bin objects are mutable and
        changes to the bin objects will be propagated back to the histogram
        unless the copy() method is called on a bin.

        """
        cdef size_t numbins = self.ptr().numBins()
        cdef size_t i
        cdef ProfileBin1D bin

        out = []
        for i in xrange(numbins):
            bin = ProfileBin1D_fromptr(& self.ptr().bins()[i])
            out.append(bin)
        # TODO: Why was this here?
        # self.ptr().bins()

        return out


    @property
    def lowEdge(self):
        """
        h.lowEdge -> float

        The x-value of the lowest edge of the lowest bin of the histogram.

        """
        return self.ptr().lowEdge()


    @property
    def highEdge(self):
        """
        h.highEdge -> float

        The x-value of the highest edge of the highest bin of the histogram.

        """
        return self.ptr().highEdge()


    @property
    def totalDbn(self):
        """
        h.totalDbn -> Distribution1D

        Return the Distribution1D object representing the total distribution.

        """
        return Dbn2D_fromptr(&self.ptr().totalDbn())


    @property
    def underflow(self):
        """
        h.underflow -> Distribution1D

        Return the Distribution1D object representing the underflow.

        """
        return Dbn2D_fromptr(&self.ptr().underflow())


    @property
    def overflow(self):
        """
        h.overflow -> Distribution1D

        Return the Distribution1D object representing the overflow.

        """
        return Dbn2D_fromptr(&self.ptr().overflow())


    # def __delitem__(self, size_t ix):
    #     self.ptr().eraseBin(ix)


    def __getitem__(self, size_t ix):
        return ProfileBin1D_fromptr(& self.ptr().bin(ix))


    def sumW(self, bool overflows=True):
        """
        s.sumW([overflows]) -> float

        Return the sum of weights of the histogram. If overflows is False,
        ignore over-and underflow bins.

        """

        return self.ptr().sumW(overflows)


    def sumW2(self, bool overflows=True):
        """
        s.sumW2([overflows]) -> float

        Return the sum of weights squared. If overflows is False, ignore
        over-and underflow bins.

        """
        return self.ptr().sumW2(overflows)


    def __add__(Profile1D a, Profile1D b):
        cdef cProfile1D *res = new cProfile1D(add(a.ptr()[0], b.ptr()[0]))
        return Profile1D_fromptr(res, True)


    def __sub__(Profile1D a, Profile1D b):
        cdef cProfile1D *res = new cProfile1D(subtract(a.ptr()[0], b.ptr()[0]))
        return Profile1D_fromptr(res, True)


    def __mul__(x, y):
        """
        Scalar multiplication. Equivalent to scaleW acting on a copy.

        """
        cdef cProfile1D *res
        tx, ty = type(x), type(y)
        if (tx is int or tx is float) and ty is Profile1D:
            histo = <Profile1D> y; factor = <Profile1D> x
        elif tx is Profile1D and (ty is int or ty is float):
            histo = <Profile1D> x; factor = <Profile1D> y
        else:
            raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))

        res = new cProfile1D(histo.ptr()[0])
        res.scaleW(factor)
        return Profile1D_fromptr(res, True)


    def _div_scalar(Profile1D x, double y):
        if y == 0:
            raise ArithmeticError('Profile1D: Divide by zero scalar')

        cdef cProfile1D *res = new cProfile1D(x.ptr()[0])

        res.scaleW(1.0 / y)
        return Profile1D_fromptr(res, True)


    def _div_profile(Profile1D x, Profile1D y):
        cdef cScatter2D s = divide(x.ptr()[0], y.ptr()[0])
        return Scatter2D_fromptr(&s)


    def __div__(x, y):
        """
        Division by scalar (i.e. multiplication by reciprocal) or another
        1D histogram.

        """
        tx = type(x); ty = type(y)
        if tx is Profile1D:
            if ty is int or ty is float:
                return x._div_scalar(y)
            elif ty is Profile1D:
                return x._div_profile(y)
        raise RuntimeError('Cannot multiply %r by %r' % (tx, ty))


    def __repr__(self):
        return 'Profile1D%r' % self.bins


cdef Profile1D Profile1D_fromptr(cProfile1D *ptr, bool dealloc=False):
    cdef Profile1D profile = Profile1D.__new__(Profile1D)
    return profile.setptr(ptr, dealloc)
