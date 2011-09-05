cdef extern from "YODA/ReaderAIDA.h" namespace "YODA::ReaderAIDA":
    cReader createReaderAIDA "YODA::ReaderAIDA::create" ()

class ReaderAIDA:
    """Read AIDA files"""

    def read(self, char *filename):
        """Takes a file, returns a list of AnalysisObjects"""
        cdef vector[AOptr] vec = vector[AOptr]()
        cdef size_t i
        cdef AnalysisObject ana, ana_

        createReaderAIDA().read(string(filename), vec)
        out = []

        ana_ = AnalysisObject()

        for i in range(vec.size()):
            ana_.thisptr = vec[i]
            if ana_.type == 'Scatter2D':
                ana = Scatter2D()
            else:
                ana = AnalysisObject()
            ana.thisptr = vec[i]
            out.append(ana)
        
        #print str(vec[0].type().c_str())

        return out
