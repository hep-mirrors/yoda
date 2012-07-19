cdef extern from "YODA/Point3D.h" namespace "YODA":
    cdef cppclass cPoint3D "YODA::Point3D":
        cPoint3D ()
        cPoint3D (cPoint3D &p)

        cPoint3D (double x, double y, double z,
                  double exminus, double explus,
                  double eyminus, double eyplus,
                  double ezminus, double ezplus)

        double x()
        double y()
        double z()
        void setX(double x)
        void setY(double y)
        void setZ(double z)
        double xMin()
        double xMax()
        double yMin()
        double yMax()
        double zMin()
        double zMax()
        pair[double,double] xErrs()
        pair[double,double] yErrs()
        pair[double,double] zErrs()
        void setXErr(double minus, double plus)
        void setYErr(double minus, double plus)
        void setZErr(double minus, double plus)


cdef class Point3D:
    """
    A 3D point, as contained in Scatter3D.

    Several constructor forms are supported:

    * Point3D() - default point construction at (0,0,0)
    * Point3D(x, y, z) - point construction at (x,y,z)
    * Point3D(x, y, z, ex, ey, ez) - point construction at (x,y,z) with errors. ex and ey can be pairs.
    * Point3D(x, y, z, ex-, ex+, ey-, ey+, ez-, ez+) - point construction at (x,y,z) with asymm errors.
    """

    cdef cPoint3D* thisptr
    cdef bool _dealloc


    cdef cPoint3D* ptr(self):
        return self.thisptr

    cdef Point3D setptr(self, cPoint3D* ptr, dealloc=False):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self


    def __cinit__(self):
        self._dealloc = False

    def __init__(self, *args):
        self.setptr(new cPoint3D(), True)
        self._dealloc = True

        if len(args) == 0:
            self.pos = 0, 0, 0
        elif len(args) == 3:
            self.pos = args
        elif len(args) == 6:
            self.pos = args[:3]
            self.xErrs, self.yErrs, self.zErrs = args[3:]
        elif len(args) == 9:
            self.pos = args[:3]
            self.xErrs = args[3:5]
            self.yErrs = args[5:7]
            self.zErrs = args[7:]
        else:
            raise ValueError(
                'Wrong number of values: can take 3, 6, or 9 parameters')

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr


    def _x(self):
        return self.ptr().x()

    def _y(self):
        return self.ptr().y()

    def _z(self):
        return self.ptr().z()

    def _setX(self, double x):
        self.ptr().setX(x)

    def _setY(self, double y):
        self.ptr().setY(y)

    def _setZ(self, double z):
        self.ptr().setZ(z)

    x = property(_x, _setX)
    y = property(_y, _setY)
    z = property(_z, _setZ)


    def _pos(self):
        """(x, y) coordinates of this point"""
        return (self.x, self.y, self.z)

    def _setPos(self, pos):
        cdef double x, y
        x, y, z = pos
        self.ptr().setX(x)
        self.ptr().setY(y)
        self.ptr().setZ(z)

    pos = property(_pos, _setPos)


    def _xErrs(self):
        """The x-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] xErrs = self.ptr().xErrs()
        return (xErrs.first, xErrs.second)

    def _setxErrs(self, arg):
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.ptr().setXErr(low, high)

    xErrs = property(_xErrs, _setxErrs)


    def _yErrs(self):
        """The y-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] yErrs = self.ptr().yErrs()
        return (yErrs.first, yErrs.second)

    def _setyErrs(self, arg):
        cdef double low, high
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.ptr().setYErr(low, high)

    yErrs = property(_yErrs, _setyErrs)


    def _zErrs(self):
        """The z-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] zErrs = self.ptr().zErrs()
        return (zErrs.first, zErrs.second)

    def _setzErrs(self, arg):
        cdef double low, high
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.ptr().setZErr(low, high)

    zErrs = property(_zErrs, _setzErrs)


    def __repr__(self):
        return 'Point3D({0},{1})'.format(self.x, self.y, self.z)


cdef Point3D Point3D_fromptr(cPoint3D *ptr, dealloc = False):
    cdef Point3D p = Point3D.__new__(Point3D)
    return p.setptr(ptr, dealloc)
