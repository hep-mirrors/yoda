cdef extern from "YODA/Writer.h" namespace "YODA":
    cdef cppclass cReader "YODA::Writer":
        pass
        #void read(string filename, vector[AOptr]&) except +
