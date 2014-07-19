cdef class Bin(util.Base):
    cdef inline c.Bin* _Bin(self) except NULL:
        return <c.Bin*> self.ptr()

    def __dealloc__(self):
        cdef c.Bin* p = self._Bin()
        if self._deallocate:
            del p
