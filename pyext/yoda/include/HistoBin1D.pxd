cdef extern from "YODA/HistoBin1D.h" namespace "YODA":

    cHistoBin1D operator + (cHistoBin1D &, cHistoBin1D &)
    cHistoBin1D operator - (cHistoBin1D &, cHistoBin1D &)

    cdef cppclass cHistoBin1D "YODA::HistoBin1D":
        cHistoBin1D (cHistoBin1D &h)
        double area()
        double height()
        double areaErr()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
        # way to make Cython acknowledge these (it seems to be a typedef parsing
        # issue rather than a technical issue).

        void scaleW(double factor)
        void scaleX(double factor)
        
        double lowEdge()
        double highEdge()
        double width()
        double focus()
        double midpoint()
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()
        double numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()
        double sumWX()
        double sumWX2()
