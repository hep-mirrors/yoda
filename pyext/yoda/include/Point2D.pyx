cdef class Point2D:
    cdef cPoint2D* thisptr

    def __cinit__(self):
        self.thisptr = new cPoint2D()
    
    def __init__(self,  *args):
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
        
    def _x(self):
        return self.thisptr.x()

    def _y(self):
        return self.thisptr.y()

    def _setX(self, double x):
        self.thisptr.setX(x)

    def _setY(self, double y):
        self.thisptr.setY(y)

    x = property(_x, _setX)
    y = property(_y, _setY)

    def _pos(self):
        """(x, y) coordinates of this point"""
        return (self.x, self.y)

    def _setPos(self, pos):
        cdef double x, y
        x, y = pos
        self.thisptr.setX(x)
        self.thisptr.setY(y)

    pos = property(_pos, _setPos)

    def _xErrs(self):
        """The x-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] xErrs = self.thisptr.xErrs()
        return (xErrs.first, xErrs.second)

    def _setxErrs(self, arg):
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.thisptr.setXErr(low, high)

    xErrs = property(_xErrs, _setxErrs)

    def _yErrs(self):
        """The y-errors as a 2-tuple (low, high)"""
        cdef pair[double, double] yErrs = self.thisptr.yErrs()
        return (yErrs.first, yErrs.second)

    def _setyErrs(self, arg):
        cdef double low, high
        try:
            low, high = arg
        except TypeError:
            low = arg
            high = arg

        self.thisptr.setYErr(low, high)

    yErrs = property(_yErrs, _setyErrs)

    def __repr__(self):
        return 'Point2D({0},{1})'.format(self.x, self.y)

