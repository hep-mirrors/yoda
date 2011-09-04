cdef extern from "YODA/HistoBin2D.h" namespace "YODA":

    cHistoBin2D operator + (cHistoBin2D &, cHistoBin2D &)
    cHistoBin2D operator - (cHistoBin2D &, cHistoBin2D &)

    cdef cppclass cHistoBin2D "YODA::HistoBin2D":
        cHistoBin2D (cHistoBin2D &h)

        double volume()
        double volumeErr()
        double height()
        double heightErr()
        void reset()

        # These are inherited methods from Bin1D... I can't seem to find a nice
        # way to make Cython acknowledge these (it seems to be a typedef parsing
        # issue rather than a technical issue).

        void scaleW(double factor)
        
        double lowEdgeX()
        double lowEdgeY()
        double highEdgeX()
        double highEdgeY()
        double widthX()
        double widthY()
        pair[double,double] focus()
        pair[double,double] midpoint()
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()

        double yStdErr()
        double yMean()
        double yVariance()
        double yStdDev()

        double numEntries()
        # double effNumEntries()

        double sumW()
        double sumW2()

        double sumWX()
        double sumWY()
        double sumWXY()
        double sumWX2()
        double sumWY2()
