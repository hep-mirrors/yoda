#TODO: Tidy up + docstrings etc.


cdef class Scatter2D(AnalysisObject):
    """
    2D Scatter plot.

    """

    cdef inline c.Scatter2D* _Scatter2D(self) except NULL:
        return <c.Scatter2D*> self.ptr()

    def __init__(self, *args, **kwargs):
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

    def points(self):
        return list(self)

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


    def transformX(self, f):
        # Import ctypes here (rather than at module import) so that Python 2.4
        # users don't need ctypes to use the rest of the library.
        import ctypes

        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")


        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]

        c.Scatter2D_transformX(deref(self._Scatter2D()), fptr)

    def transformY(self, f):
        # Import ctypes here (rather than at module import) so that Python 2.4
        # users don't need ctypes to use the rest of the library.
        import ctypes

        try:
            callback = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
        except:
            raise RuntimeError("Callback is not of type (double) -> double")


        fptr = (<c.dbl_dbl_fptr*><size_t>ctypes.addressof(callback))[0]

        c.Scatter2D_transformY(deref(self._Scatter2D()), fptr)



    def __add__(Scatter2D self, Scatter2D other):
        h = Scatter2D()
        util.set_owned_ptr(h, c.Scatter2D_add_Scatter2D(self._Scatter2D(), other._Scatter2D()))
        return h
    def __sub__(Scatter2D self, Scatter2D other):
        h = Scatter2D()
        util.set_owned_ptr(h, c.Scatter2D_sub_Scatter2D(self._Scatter2D(), other._Scatter2D()))
        return h
