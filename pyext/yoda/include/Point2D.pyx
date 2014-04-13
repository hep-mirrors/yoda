cdef inline pair[double, double] read_symmetric(object val) except *:
    try:
        a, b = val
    except TypeError:
        a = b = val
    return pair[double, double](a, b)


cdef inline object read_error_pair(pair[double, double] es):
    return util.Errors(es.first, es.second)



cdef class Point2D(util.Base):
    """
    A 2D point with errors, used by the Scatter2D class.

    """

    def __init__(self, x=0, y=0, xerrs=0, yerrs=0):
        util.set_owned_ptr(self, new c.Point2D())
        self.coords = x, y
        self.xErrs = xerrs
        self.yErrs = yerrs

    def copy(self):
        return util.new_owned_cls(Point2D, new c.Point2D(deref(self._Point2D())))


    property x:
        """x coordinate"""
        def __get__(self):
            return self._Point2D().x()

        def __set__(self, x):
            self._Point2D().setX(x)


    property y:
        """y coordinate"""
        def __get__(self):
            return self._Point2D().y()

        def __set__(self, y):
            self._Point2D().setY(y)


    # TODO: remove!
    property coords:
        """x and y coordinates as a tuple"""
        def __get__(self):
            return util.XY(self.x, self.y)

        def __set__(self, val):
            self.x, self.y = val


    # TODO: remove!
    property xRange:
        """The minimum and maximum points within the x errors"""
        def __get__(self):
            return util.Edges(self._Point2D().xMin(),
                              self._Point2D().xMax())

    # TODO: remove!
    property yRange:
        """The minimum and maximum points within the y errors"""
        def __get__(self):
            return util.Edges(self._Point2D().yMin(),
                              self._Point2D().yMax())

    # TODO: remove!
    property ranges:
        """The x- and y-ranges"""
        def __get__(self):
            return util.XY(self.xRange, self.yRange)


    property xErrs:
        """The x errors"""
        def __get__(self):
            return read_error_pair(self._Point2D().xErrs())

        def __set__(self, val):
            self._Point2D().setXErr(read_symmetric(val))


    property yErrs:
        """The y errors"""
        def __get__(self):
            return read_error_pair(self._Point2D().yErrs())

        def __set__(self, val):
            self._Point2D().setYErr(read_symmetric(val))


    property errs:
        """The x and y errors as a tuple"""
        def __get__(self):
            return util.XY(self.xErrs, self.yErrs)

        def __set__(self, val):
            self.xErrs, self.yErrs = val


    property xErrAvg:
        def __get__(self):
            return self.xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self.yErrAvg()

    def scale(self, x=1.0, y=1.0):
        """
        (x=1.0, y=1.0) -> None

        Scale the Point's variables by the given factors.

        """
        self._Point2D().scale(x, y)

    def __repr__(self):
        return '<Point2D(%g, %g)>' % self.coords

    def __richcmp__(Point2D self, Point2D other, int op):
        if op == 0:
            return deref(self._Point2D()) < deref(other._Point2D())
        elif op == 1:
            return deref(self._Point2D()) <= deref(other._Point2D())
        elif op == 2:
            return deref(self._Point2D()) == deref(other._Point2D())
        elif op == 3:
            return deref(self._Point2D()) != deref(other._Point2D())
        elif op == 4:
            return deref(self._Point2D()) > deref(other._Point2D())
        elif op == 5:
            return deref(self._Point2D()) >= deref(other._Point2D())

    # Magic stuff
    cdef c.Point2D *_Point2D(self) except NULL:
        return <c.Point2D *> self.ptr()

    def __dealloc__(self):
        cdef c.Point2D *p = self._Point2D()
        if self._deallocate:
            del p
