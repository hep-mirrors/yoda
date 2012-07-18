cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass cReader "YODA::Reader":
        void read(string filename, vector[AOptr]&) except +
