cdef extern from "YODA/Dbn1D.h" namespace "YODA":
    cdef cppclass cDbn1D "Dbn1D":
        pass


cdef class Dbn1D:
    cdef cDbn1D *thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef Dbn1D setptr(self, cDbn1D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cDbn1D* ptr(self):
        return self.thisptr


cdef Dbn1D Dbn1D_fromptr(cDbn1D *ptr, dealloc=False):
    # Construct a Python Dbn1D from a pointer to a cDbn1D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn1D dbn = Dbn1D.__new__(Dbn1D)
    return dbn.setptr(ptr, dealloc)
