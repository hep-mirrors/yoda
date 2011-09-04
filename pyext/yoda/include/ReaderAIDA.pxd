cdef extern from "YODA/ReaderAIDA.h" namespace "YODA::ReaderAIDA":
    cReader createReaderAIDA "YODA::ReaderAIDA::create" ()
