cdef class ProfileBin2D(Bin2D_Dbn3D):

    cdef inline c.ProfileBin2D *_ProfileBin2D(self) except NULL:
        return <c.ProfileBin2D *> self.ptr()

    def __init__(self, xlow, xhigh, ylow, yhigh):
        util.set_owned_ptr(
            self, new c.ProfileBin2D(xlow, xhigh, ylow, yhigh))
        
    def fill(self, x, y, z, weight=1.0):
        self._ProfileBin2D().fill(x, y, z, weight)

    def fill_bin(self, z, weight=1.0):
        self._ProfileBin2D().fillBin(z, weight)

    @property
    def mean(self):
        return self._ProfileBin2D().mean()

    @property
    def std_dev(self):
        return self._ProfileBin2D().stdDev()
    
    @property
    def variance(self):
        return self._ProfileBin2D().variance()

    @property
    def std_err(self):
        return self._ProfileBin2D().stdErr()

    @property
    def rms(self):
        return self._ProfileBin2D().rms()

    @property
    def sum_wz(self):
        return self._ProfileBin2D().sumWZ()

    @property
    def sum_wz2(self):
        return self._ProfileBin2D().sumWZ2()

    def __add__(ProfileBin2D a, ProfileBin2D b):
        return util.new_owned_cls(
            ProfileBin2D,
            new c.ProfileBin2D(
                deref(a._ProfileBin2D()) + 
                deref(b._ProfileBin2D())))

    def __sub__(ProfileBin2D a, ProfileBin2D b):
        return util.new_owned_cls(
            ProfileBin2D,
            new c.ProfileBin2D(
                deref(a._ProfileBin2D()) -
                deref(b._ProfileBin2D())))

    def __repr__(self):
        return 'ProfileBin2D(%g, %g, %g, %g)' % (self.edges.x + self.edges.y)
