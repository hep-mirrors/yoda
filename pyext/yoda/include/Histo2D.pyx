#TODO tidy up when Axis2D is fixed

cdef class Histo2D(AnalysisObject):

    cdef inline c.Histo2D *_Histo2D(self) except NULL:
        return <c.Histo2D*> self.ptr()

    def __init__(self, *args, **kwargs):
        if len(args) <= 2:
            self.__init2(*args, **kwargs)
        else:
            self.__init7(*args, **kwargs)

    def __init2(Histo2D self, char *path="", char *title=""):
        util.set_owned_ptr(
            self, new c.Histo2D(string(path), string(title))) 

    def __init7(Histo2D self,   nxbins, xlow, xupp,   nybins, ylow, yupp,
                char *path="", char *title=""):

        util.set_owned_ptr(
            self, new c.Histo2D(
                nxbins, xlow, xupp,
                nybins, ylow, yupp,
                string(path), string(title)))

    def __len__(self):
        return self._Histo2D().bins().size()

    def __getitem__(self, py_ix):
        cdef size_t i = util.pythonic_index(py_ix, self._Histo2D().bins().size())
        return util.new_borrowed_cls(
            HistoBin2D, & self._Histo2D().bins().at(i), self)

    def __repr__(self):
        return "Histo2D"

    def fill(self, x, y, weight=1.0):
        self._Histo2D().fill(x, y, weight)

    def copy(self, char *path=""):
        return util.new_owned_cls(Histo2D,
            new c.Histo2D(deref(self._Histo2D()), string(path)))

    @property
    def total_dbn(self):
        return util.new_owned_cls(
            Dbn2D,
            new c.Dbn2D(self._Histo2D().totalDbn()))

    def outflow(self, ix=0, iy=0):
        # For now we do manual error detection, due to exceptions and references
        return util.new_owned_cls(
            Dbn2D,
            new c.Dbn2D(self._Histo2D().outflow(ix, iy)))

    def sum_w(self, overflows=True):
        return self._Histo2D().sumW(overflows)

    def mean(self, overflows=True):
        return XY(
            self._Histo2D().xMean(overflows),
            self._Histo2D().yMean(overflows))

    def variance(self, overflows=True):
        return XY(
            self._Histo2D().xVariance(overflows),
            self._Histo2D().yVariance(overflows))

    def std_dev(self, overflows=True):
        return XY(
            self._Histo2D().xStdDev(overflows),
            self._Histo2D().yStdDev(overflows))

    def std_err(self, overflows=True):
        return XY(
            self._Histo2D().xStdErr(overflows),
            self._Histo2D().yStdErr(overflows))

    def reset(self):
        self._Histo2D().reset()

    def scale(self, w=1.0):
        """
        (w=1.0) -> None

        Scale the given parameters

        """
        if w != 1.0:
            self._Histo2D().scaleW(w)

    def normalize(self, double normto, bint includeoverflows=True):
        self._Histo2D().normalize(normto, includeoverflows)

    #def mergeBins(self, size_t a, size_t b):
    #    self._Histo2D().mergeBins(a, b)

    #def rebin(self, int n):
    #    self._Histo2D().rebin(n)

    #def addBin(self, double low, double high):
    #    """Add a bin to the Histo2D"""
    #    self._Histo2D().addBin(low, high)
    #    return self

    #def addBins(self, edges):
    #    cdef vector[double] cedges
    #    for i in edges:
    #        cedges.push_back(i)
    #    self._Histo2D().addBins(cedges)
    #    return self

