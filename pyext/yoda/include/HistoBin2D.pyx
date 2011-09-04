cdef class HistoBin2D:
    cdef cHistoBin2D *thisptr

    cdef setptr(self, cHistoBin2D *ptr):
        return self

    cdef set(self, cHistoBin2D ptr):
        self.thisptr = new cHistoBin2D(ptr)
        return self

    cdef cHistoBin2D* ptr(self):
        return self.thisptr

    @property
    def volume(self):
        return self.ptr().volume()

    @property
    def volumeErr(self):
        return self.ptr().volumeErr()

    @property
    def height(self):
        return self.ptr().height()

    @property
    def heightErr(self):
        return self.ptr().heightErr()

    def scaleW(self, double factor):
        self.ptr().scaleW(factor)


    @property
    def lowEdgeX(self):
        return self.ptr().lowEdgeX()

    @property
    def highEdgeX(self):
        return self.ptr().highEdgeX()


    @property
    def lowEdgeY(self):
        return self.ptr().lowEdgeY()
    
    @property
    def highEdgeY(self):
        return self.ptr().highEdgeY()

    xMin = lowEdgeX
    xMax = highEdgeX
    yMin = lowEdgeY
    yMax = highEdgeY

    @property
    def widthX(self):
        return self.ptr().widthX()

    @property
    def widthY(self):
        return self.ptr().widthY()

    @property
    def focus(self):
        cdef pair[double, double] p
        p = self.ptr().focus()
        return (p.first, p.second)

    @property
    def midpoint(self):
        cdef pair[double, double] p
        p = self.ptr().midpoint()
        return (p.first, p.second)

    @property
    def xMean(self):
        return self.ptr().xMean()

    @property
    def xVariance(self):
        return self.ptr().xVariance()

    @property
    def xStdDev(self):
        return self.ptr().xStdDev()

    @property
    def xStdErr(self):
        return self.ptr().xStdDev()

    @property
    def yMean(self):
        return self.ptr().yMean()

    @property
    def yVariance(self):
        return self.ptr().yVariance()

    @property
    def yStdDev(self):
        return self.ptr().yStdDev()

    @property
    def yStdErr(self):
        return self.ptr().yStdDev()

    @property
    def numEntries(self):
        return self.ptr().numEntries()

    """@property
    def effNumEntries(self):
        return self.ptr().effNumEntries()"""

    @property
    def sumW(self):
        return self.ptr().sumW()

    @property
    def sumW2(self):
        return self.ptr().sumW2()

    @property
    def sumWX(self):
        return self.ptr().sumWX()

    @property
    def sumWY(self):
        return self.ptr().sumWY()

    @property
    def sumWXY(self):
        return self.ptr().sumWXY()

    @property
    def sumWX2(self):
        return self.ptr().sumWX2()

    @property
    def sumWY2(self):
        return self.ptr().sumWY2()

    def __repr__(self):
        return 'HistoBin2D(%r)' % self.volume
