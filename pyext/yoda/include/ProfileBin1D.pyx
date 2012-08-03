cdef class ProfileBin1D(Bin1D_Dbn2D):

    cdef inline c.ProfileBin1D *_ProfileBin1D(self) except NULL:
        return <c.ProfileBin1D *> self.ptr()

    def __init__(self, double a, double b):
        util.set_owned_ptr(
            self, new c.ProfileBin1D(a, b))

    def fill(self, double x, double y, double weight=1.0):
        self._ProfileBin1D().fill(x, y, weight)

    def fill_bin(self, double y, double weight=1.0):
        self._ProfileBin1D().fillBin(y, weight)

    @property
    def mean(self):
        return self._ProfileBin1D().mean()

    @property
    def stdDev(self):
        return self._ProfileBin1D().stdDev()

    @property
    def variance(self):
        return self._ProfileBin1D().variance()

    @property
    def stdErr(self):
        return self._ProfileBin1D().stdErr()

    @property
    def rms(self):
        return self._ProfileBin1D().rms()

    @property
    def sumWY(self):
        return self._ProfileBin1D().sumWY()

    @property
    def sumWY2(self):
        return self._ProfileBin1D().sumWY2()

    def __add__(ProfileBin1D a, ProfileBin1D b):
        return util.new_owned_cls(
            ProfileBin1D,
            new c.ProfileBin1D(
                deref(a._ProfileBin1D()) +
                deref(b._ProfileBin1D())))

    def __sub__(ProfileBin1D a, ProfileBin1D b):
        return util.new_owned_cls(
            ProfileBin1D,
            new c.ProfileBin1D(
                deref(a._ProfileBin1D()) -
                deref(b._ProfileBin1D())))
