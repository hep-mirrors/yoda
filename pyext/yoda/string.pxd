cdef extern from "<string>" namespace "std":

    size_t npos = -1

    cdef cppclass string:
        string(char *)
        char* c_str()
