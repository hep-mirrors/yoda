cdef extern from "YODA/Point2D.h" namespace "YODA":
    cdef cppclass cPoint2D "YODA::Point2D":
        cPoint2D ()
        cPoint2D (cPoint2D &p)

        cPoint2D (double x, double y,
                  double exminus, double explus,
                 double eyminus, double eyplus)

        double x()
        double y()
        void setX(double x)
        void setY(double y)
        double xMin()
        double xMax()
        pair[double,double] xErrs()
        pair[double,double] yErrs()
        void setXErr(double minus, double plus)
        void setYErr(double minus, double plus)

