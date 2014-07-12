cdef class Point3D(util.Base):
    """
    A 3D point with errors, used by the Scatter3D class.
    """

    cdef c.Point3D *_Point3D(self) except NULL:
        return <c.Point3D *> self.ptr()

    def __dealloc__(self):
        cdef c.Point3D *p = self._Point3D()
        if self._deallocate:
            del p


    def __init__(self, x=0, y=0, xerrs=0, yerrs=0):
        util.set_owned_ptr(self, new c.Point3D())
        self.xy = x, y
        self.xerrs = xerrs
        self.yerrs = yerrs

    def copy(self):
        return util.new_owned_cls(Point3D, new c.Point3D(deref(self._Point3D())))

    property x:
        """x coordinate"""
        def __get__(self):
            return self._Point3D().x()
        def __set__(self, x):
            self._Point3D().setX(x)

    property y:
        """y coordinate"""
        def __get__(self):
            return self._Point3D().y()
        def __set__(self, y):
            self._Point3D().setY(y)

    property z:
        """y coordinate"""
        def __get__(self):
            return self._Point3D().z()
        def __set__(self, z):
            self._Point3D().setZ(z)

    property xyz:
        def __get__(self):
            return util.XYZ(self.x, self.y, self.z)
        def __set__(self, val):
            self.x, self.y, self.z = val

    property xErrs:
        def __get__(self):
            return read_error_pair(self._Point3D().xErrs())
        def __set__(self, val):
            self._Point3D().setXErr(read_symmetric(val))

    property yErrs:
        def __get__(self):
            return read_error_pair(self._Point3D().yErrs())
        def __set__(self, val):
            self._Point3D().setYErr(read_symmetric(val))

    property zErrs:
        def __get__(self):
            return read_error_pair(self._Point3D().zErrs())
        def __set__(self, val):
            self._Point3D().setZErr(read_symmetric(val))

    property xErrAvg:
        def __get__(self):
            return self._Point3D().xErrAvg()

    property yErrAvg:
        def __get__(self):
            return self._Point3D().yErrAvg()

    property zErrAvg:
        def __get__(self):
            return self._Point3D().zErrAvg()

    def scale(self, x=1.0, y=1.0, z=1.0):
        """
        (x=1.0, y=1.0, z=1.0) -> None
        Scale the point coordinates by the given factors.
        """
        self._Point3D().scale(x, y, z)

    def __repr__(self):
        return '<Point3D(x=%g, y=%g)>' % (
            self.x, self.y, self.xerrs, self.yerrs)

    # def __str__(self):
    #     return 'Point3D(%g, %g)' % self.xy

    def __richcmp__(Point3D self, Point3D other, int op):
        if op == 0:
            return deref(self._Point3D()) < deref(other._Point3D())
        elif op == 1:
            return deref(self._Point3D()) <= deref(other._Point3D())
        elif op == 2:
            return deref(self._Point3D()) == deref(other._Point3D())
        elif op == 3:
            return deref(self._Point3D()) != deref(other._Point3D())
        elif op == 4:
            return deref(self._Point3D()) > deref(other._Point3D())
        elif op == 5:
            return deref(self._Point3D()) >= deref(other._Point3D())
