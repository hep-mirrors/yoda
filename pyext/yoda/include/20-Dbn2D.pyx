cdef extern from "YODA/Dbn2D.h" namespace "YODA":
    cdef cppclass cDbn2D "Dbn2D":
        pass


cdef class Dbn2D:
    cdef cDbn2D *thisptr
    cdef bool _dealloc

    def __cinit__(self):
        self._dealloc = False

    def __dealloc__(self):
        if self._dealloc:
            del self.thisptr

    cdef Dbn2D setptr(self, cDbn2D *ptr, bool dealloc):
        if self._dealloc:
            del self.thisptr

        self.thisptr = ptr
        self._dealloc = dealloc
        return self

    cdef cDbn2D* ptr(self):
        return self.thisptr


cdef Dbn2D Dbn2D_fromptr(cDbn2D *ptr, dealloc=False):
    # Construct a Python Dbn2D from a pointer to a cDbn2D,
    # without calling __init__ and excessive memory allocation
    cdef Dbn2D dbn = Dbn2D.__new__(Dbn2D)
    return dbn.setptr(ptr, dealloc)
