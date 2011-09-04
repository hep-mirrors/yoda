cdef extern from "YODA/Histo1D.h" namespace "YODA":
    cHisto1D operator + (cHisto1D &, cHisto1D &)
    cHisto1D operator - (cHisto1D &, cHisto1D &)
    cScatter2D operator / (cHisto1D &, cHisto1D &)

    cdef cppclass cHisto1D "YODA::Histo1D"(cAnalysisObject):
        cHisto1D(size_t nbins, double lower, double upper, string &path,
                string &title)
        cHisto1D(vector[double] &binedges, string &path, string &title)
        cHisto1D(vector[double] &binedges)
        cHisto1D(cHisto1D &h, string &path)
        cHisto1D(cHisto1D &h)

        void fill(double x, double weight)
        void reset()
        void scaleW(double scalefactor)
        void mergeBins(size_t a, size_t b)
        void rebin(int n)

        # Bin Accessors
        size_t numBins()
        double lowEdge()
        double highEdge()
        vector[cHistoBin1D] &bins()
        cDbn1D &underflow()
        cDbn1D &overflow()
        void eraseBin(size_t index)

        # Statistical functions
        double integral(bool includeoverflows)
        double integral(size_t a, size_t b)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double variance(bool includeoverflows)
        double stdDev(bool includeoverflows)

