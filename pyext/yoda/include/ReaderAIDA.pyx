class ReaderAIDA:
    """Read AIDA files"""
    @staticmethod
    def read(char *filename):
        """Takes a file, returns a list of AnalysisObjects"""
        cdef vector[cAnalysisObject *] vec
        cdef int i
        vec = createReaderAIDA().read(string(filename))
        out = []
        for i in range(vec.size()):
            if str(vec[i].type().c_str()) == 'Scatter2D':
                ana = Scatter2D()
            else:
                ana = AnalysisObject()

            ana.thisptr = vec[i]

            out.append(ana)

        return out

