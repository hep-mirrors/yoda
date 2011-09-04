cdef extern from "YODA/Histo2D.h" namespace "YODA":
    cHisto2D operator + (cHisto2D &, cHisto2D &)
    cHisto2D operator - (cHisto2D &, cHisto2D &)

    cdef cppclass cHisto2D "YODA::Histo2D"(cAnalysisObject):
        cHisto2D(size_t nbinsX, double lowerX, double upperX,
                 size_t nbinsY, double lowerY, double upperY,
                 string &path, string &title)
        
        cHisto2D(cHisto2D &h)

        void fill(double x, double y, double weight)
        void reset()
        void scaleW(double scalefactor)
        void scaleXY(double scaleX, double scaleY)
        void mergeBins(size_t a, size_t b)
        void rebin(int a, int b)

        # Bin Accessors
        size_t numBins()
        double lowEdgeX()
        double lowEdgeY()
        double highEdgeX()
        double highEdgeY()

        vector[cHistoBin2D] &bins()
        cHistoBin2D& binByCoord(double x, double y)

        void eraseBin(size_t index)

        # Statistical functions
        double integral(bool includeoverflows)
        double sumW(bool includeoverflows)
        double sumW2(bool includeoverflows)
        double xMean(bool includeoverflows)
        double yMean(bool includeoverflows)

        double xVariance(bool includeoverflows)
        double yVariance(bool includeoverflows)
        
        double xStdDev(bool includeoverflows)
        double yStdDev(bool includeoverflows)
