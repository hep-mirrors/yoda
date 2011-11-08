cdef extern from "YODA/Point2D.h" namespace "YODA":
    cdef cppclass cPoint2D "YODA::Point2D":
        cPoint2D ()
        cPoint2D (cPoint2D &p)

        cPoint2D (double x, double y,
                  double exminus, double explus,
                 double eyminus, double eyplus)

        double x()
        double y()
        void setX(double x)
        void setY(double y)
        double xMin()
        double xMax()
        pair[double,double] xErrs()
        pair[double,double] yErrs()
        void setXErr(double minus, double plus)
        void setYErr(double minus, double plus)

cdef class Point2D:
    cdef cPoint2D* thisptr

    cdef bool _dealloc

    cdef cPoint2D* ptr(self):
        return self.thisptr

    cdef Point2D setptr(self, cPoint2D* ptr, dealloc=False):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    def __cinit__(self):
        self._dealloc = False

    def __init__(self,  *args):
        self.setptr(new cPoint2D(), True)
        self._dealloc = True

        if len(args) == 0:
            self.pos = 0, 0
        elif len(args) == 2 :
            self.pos = args
        elif len(args) == 4:
            self.pos = args[:2]
            self.xErrs, self.yErrs = args[2:]
        elif len(args) == 6:
            self.pos = args[:2]
            self.xErrs = args[2:4]
            self.yErrs = args[4:]
        else:
            raise ValueError(
                'Wrong number of values: can take 2, 4, or 6 parameters')

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr
        
    def _x(self):
        return self.ptr().x()

    def _y(self):
        return self.ptr().y()

    def _setX(self, double x):
        self.ptr().setX(x)

    def _setY(self, double y):
        self.ptr().setY(y)

    x = property(_x, _setX)
    y = property(_y, _setY)

    def _pos(self):
        """(x, y) coordinates of this point"""
        return (self.x, self.y)

    def _setPos(self, pos):
        cdef double x, y
        x, y = pos
        self.ptr().setX(x)
        self.ptr().setY(y)

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

    def __repr__(self):
        return 'Point2D({0},{1})'.format(self.x, self.y)

cdef Point2D Point2D_fromptr(cPoint2D *ptr, dealloc = False):
    cdef Point2D p = Point2D.__new__(Point2D)
    return p.setptr(ptr, dealloc)
