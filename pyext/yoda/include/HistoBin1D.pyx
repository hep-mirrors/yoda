
cdef class HistoBin1D:
    cdef cHistoBin1D *thisptr

    cdef setptr(self, cHistoBin1D *ptr):
        return self

    cdef set(self, cHistoBin1D ptr):
        self.thisptr = new cHistoBin1D(ptr)
        return self

    cdef cHistoBin1D* ptr(self):
        return self.thisptr

    @property
    def lowEdge(self):
        return self.ptr().lowEdge()
    
    xMin = lowEdge

    @property
    def highEdge(self):
        return self.ptr().highEdge()

    xMax = highEdge

    @property
    def width(self):
        return self.ptr().width()

    @property
    def focus(self):
        return self.ptr().focus()

    @property
    def midpoint(self):
        return self.ptr().midpoint()

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
    def numEntries(self):
        return self.ptr().numEntries()

    @property
    def effNumEntries(self):
        return self.ptr().effNumEntries()

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
    def sumWX2(self):
        return self.ptr().sumWX2()

    @property
    def area(self):
        return self.ptr().area()

    @property
    def height(self):
        return self.ptr().height()

    @property
    def heightErr(self):
        return self.ptr().heightErr()

    @property
    def areaErr(self):
        return self.ptr().areaErr()

    def scaleX(self, double factor):
        self.ptr().scaleX(factor)

    def __repr__(self):
        return 'HistoBin1D(%r)' % self.area
