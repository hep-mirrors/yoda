from libcpp.map cimport map
from libcpp.pair cimport pair
from libcpp.vector cimport vector
from libcpp cimport bool
from libcpp.string cimport string
from cython.operator cimport dereference as deref

# Import the error handling C++ routine
cdef extern from "errors.hh":
    # Have a look in errors.cpp for implementation specifics
    void err "translate_yoda_error" ()

ctypedef map[string, string] Annotations
ctypedef double (*dbl_dbl_fptr) (double)




# Dbn1D {{{
cdef extern from "YODA/Dbn1D.h" namespace "YODA":
    cdef cppclass Dbn1D:
        Dbn1D ()
        Dbn1D (Dbn1D)

        void fill(double val, double weight)
        void reset()
        void scaleW(double)
        void scaleX(double)

        double xMean() except+ err
        double xVariance() except+ err
        double xStdDev() except+ err
        double xStdErr() except+ err
        double xRMS() except+ err

        # Raw distribution running sums
        unsigned long numEntries() except+ err
        double effNumEntries() except+ err
        double sumW() except+ err
        double sumW2() except+ err
        double sumWX() except+ err
        double sumWX2() except+ err

        # Operators: TODO when Cython supports += and -= etc.
        Dbn1D operator+ (Dbn1D) except+ err
        Dbn1D operator- (Dbn1D) except+ err
#}}} Dbn1D


# Dbn2D {{{
cdef extern from "YODA/Dbn2D.h" namespace "YODA":
    cdef cppclass Dbn2D:
        Dbn2D ()
        Dbn2D (Dbn2D)

        void fill(double x, double y, double weight) except+ err
        void reset() except+ err
        void scaleW(double) except+ err
        void scaleX(double) except+ err
        void scaleY(double) except+ err
        void scaleXY(double, double) except+ err

        double xMean() except+ err
        double xVariance() except+ err
        double xStdDev() except+ err
        double xStdErr() except+ err
        double xRMS() except+ err

        double yMean() except+ err
        double yVariance() except+ err
        double yStdDev() except+ err
        double yStdErr() except+ err
        double yRMS() except+ err

        # Raw distribution running sums
        unsigned long numEntries() except+ err
        double effNumEntries() except+ err
        double sumW() except+ err
        double sumW2() except+ err
        double sumWX() except+ err
        double sumWX2() except+ err
        double sumWY() except+ err
        double sumWY2() except+ err
        double sumWXY() except+ err

        # Operators
        void flipXY() except+ err
        Dbn1D transformX() except+ err
        Dbn1D transformY() except+ err

        Dbn2D operator + (Dbn2D) except+ err
        Dbn2D operator - (Dbn2D) except+ err
        # TODO: += and -= operators in Cython (maybe 0.17?)
#}}} Dbn2D


# Dbn3D {{{
cdef extern from "YODA/Dbn3D.h" namespace "YODA":
    cdef cppclass Dbn3D:
        Dbn3D ()
        Dbn3D (Dbn3D)
        void fill(double x, double y, double z, double weight)
        void reset()

        void scaleW(double)
        void scaleX(double)
        void scaleY(double)
        void scaleZ(double)
        void scaleXY(double, double)
        void scaleXYZ(double, double, double)

        # Each of these statistics can return a 3D named tuple
        double xMean()
        double xVariance()
        double xStdDev()
        double xStdErr()
        double xRMS()

        double yMean()
        double yVariance()
        double yStdDev()
        double yStdErr()
        double yRMS()

        double zMean()
        double zVariance()
        double zStdDev()
        double zStdErr()
        double zRMS()

        # Raw distribution running sums
        unsigned long numEntries()
        double effNumEntries()
        double sumW()
        double sumW2()

        double sumWX()
        double sumWX2()

        double sumWY()
        double sumWY2()

        double sumWZ()
        double sumWZ2()

        double sumWXY()
        double sumWXZ()
        double sumWYZ()

        double sumWXYZ()

        # Operators
        void flipXY()
        void flipXZ()
        void flipYZ()

        Dbn1D transformX()
        Dbn1D transformY()
        Dbn1D transformZ()

        Dbn3D operator + (Dbn3D)
        Dbn3D operator - (Dbn3D)
        # TODO: += and -= operators in Cython (maybe 0.17?)
#}}} Dbn3D





# Point2D {{{
cdef extern from "YODA/Point2D.h" namespace "YODA":
    cdef cppclass Point2D:
        Point2D () except+ err
        Point2D (Point2D p) except+ err

        Point2D (double x, double y,
                  double exminus, double explus,
                 double eyminus, double eyplus) except+ err

        double x() except+ err
        double y() except+ err
        void setX(double x) except+ err
        void setY(double y) except+ err
        double xErrAvg() except+ err
        double yErrAvg() except+ err
        pair[double,double] xErrs() except+ err
        pair[double,double] yErrs() except+ err
        void setXErr(pair[double, double]) except+ err
        void setYErr(pair[double, double]) except+ err
        double xMin() except+ err
        double xMax() except+ err
        double yMin() except+ err
        double yMax() except+ err
        void scale(double x, double y) except+ err
        bool operator == (Point2D) except+ err
        bool operator != (Point2D b) except+ err
        bool operator < (Point2D b) except+ err
        bool operator <= (Point2D b) except+ err
        bool operator > (Point2D b) except+ err
        bool operator >= (Point2D b) except+ err
# }}} Point2D


# Point3D {{{
cdef extern from "YODA/Point3D.h" namespace "YODA":
    cdef cppclass Point3D:
        Point3D () except+ err
        Point3D (Point3D& p) except+ err

        Point3D (double x, double y, double z,
                 double exminus, double explus,
                 double eyminus, double eyplus,
                 double ezminus, double ezplus) except+ err

        double x() except+ err
        double y() except+ err
        double z() except+ err
        void setX(double x) except+ err
        void setY(double y) except+ err
        void setZ(double z) except+ err
        double xMin() except+ err
        double xMax() except+ err
        double yMin() except+ err
        double yMax() except+ err
        double zMin() except+ err
        double zMax() except+ err
        pair[double,double] xErrs() except+ err
        pair[double,double] yErrs() except+ err
        pair[double,double] zErrs() except+ err
        void setXErr(pair[double, double]) except+ err
        void setYErr(pair[double, double]) except+ err
        void setZErr(pair[double, double]) except+ err
        double xErrAvg()
        double yErrAvg()
        double zErrAvg()
        void scale(double x, double y, double z) except+ err

        bool operator == (Point3D b)
        bool operator != (Point3D b)
        bool operator < (Point3D b)
        bool operator <= (Point3D b)
        bool operator > (Point3D b)
        bool operator >= (Point3D b)
#}}} Point3D





# Bin {{{
cdef extern from "YODA/Bin.h" namespace "YODA":
    cdef cppclass Bin:
        pass
# }}} Bin


#Bin1D {{{
cdef extern from "YODA/Bin1D.h" namespace "YODA":
    cdef cppclass Bin1D[DBN](Bin):
        Bin1D(pair[double, double] edges) except+ err
        Bin1D(pair[double, double] edges, DBN dbn) except+ err
        Bin1D(Bin1D) except+ err

        # THIS IS A CYTHON LIMITATION... DO NOT CALL THIS
        Bin1D() # (DO NOT CALL THIS DO NOT CALL THIS) ###
        #################################################

        #We're fine as long as we don't try to instantiate these from Python

        void scaleW(double scale) except+ err
        void scaleX(double scale) except+ err
        void reset()  except+ err

        double lowEdge() except+ err
        double highEdge()  except+ err
        pair[double, double] edges() except+ err
        double width() except+ err
        double focus() except+ err
        double midpoint() except+ err

        # x statistics
        double xMean() except+ err
        double xVariance() except+ err
        double xStdDev() except+ err
        double xStdErr() except+ err
        double xRMS() except+ err

        # raw statistics
        unsigned long numEntries() except+ err
        double effNumEntries() except+ err
        double sumW() except+ err
        double sumW2() except+ err
        double sumWX() except+ err
        double sumWX2() except+ err

        void merge (Bin1D&) except+ err
        Bin1D operator + (Bin1D&) except+ err
        Bin1D operator - (Bin1D&) except+ err

ctypedef Bin1D[Dbn1D] Bin1D_Dbn1D
ctypedef Bin1D[Dbn2D] Bin1D_Dbn2D
ctypedef Bin1D[Dbn3D] Bin1D_Dbn3D
#}}} Bin1D


# Bin2D {{{
cdef extern from "YODA/Bin2D.h" namespace "YODA":
    cdef cppclass Bin2D[DBN](Bin):
        Bin2D(pair[double, double] xedges,
              pair[double, double] yedges) except+
        Bin2D(Bin2D bin) except+ err

        # CYTHON HACK DO NOT CALL THIS IT DOES NOT EXIST
        Bin2D() # (DO NOT CALL DO NOT CALL)
        ################################################

        void scaleW(double scale) except+ err
        void scaleXY(double, double) except+ err
        void reset()  except+ err

        # Also xMin, xMax etc...
        pair[double, double] xedges() except+ err
        pair[double, double] yedges() except+ err

        double xMin() except+ err
        double yMin() except+ err

        double xMax() except+ err
        double yMax() except+ err

        double xMid() except+ err
        double yMid() except+ err

        double xWidth() except+ err
        double yWidth() except+ err

        pair[double, double] focus() except+ err
        pair[double, double] midpoint() except+ err

        # x statistics
        double xMean() except+ err
        double xVariance() except+ err
        double xStdDev() except+ err
        double xStdErr() except+ err
        double xRMS() except+ err

        double yMean() except+ err
        double yVariance() except+ err
        double yStdDev() except+ err
        double yStdErr() except+ err
        double yRMS() except+ err

        # raw statistics
        unsigned long numEntries() except+ err
        double effNumEntries() except+ err
        double sumW() except+ err
        double sumW2() except+ err
        double sumWX() except+ err
        double sumWY() except+ err
        double sumWXY() except+ err
        double sumWX2() except+ err
        double sumWY2() except+ err

        #void merge(Bin2D) except+ err
        Bin2D operator + (Bin2D) except+ err
        Bin2D operator - (Bin2D) except+ err

        int adjacentTo(Bin2D) except+ err

ctypedef Bin2D[Dbn2D] Bin2D_Dbn2D
ctypedef Bin2D[Dbn3D] Bin2D_Dbn3D
# }}} Bin2D


# ProfileBin1D {{{
cdef extern from "YODA/ProfileBin1D.h" namespace "YODA":

    cdef cppclass ProfileBin1D(Bin1D_Dbn2D):
        ProfileBin1D(ProfileBin1D) except +err
        ProfileBin1D(double, double) except +err
        void fill(double x, double y, double weight) except+ err
        void fillBin(double y, double weight) except+ err
        void reset() except+ err

        double mean() except+ err
        double stdDev() except+ err
        double variance() except+ err
        double stdErr() except+ err
        double rms() except+ err

        double sumWY() except+ err
        double sumWY2() except+ err
        ProfileBin1D operator + (ProfileBin1D) except+ err
        ProfileBin1D operator - (ProfileBin1D) except+ err

# }}} ProfileBin1D


# ProfileBin2D {{{
cdef extern from "YODA/ProfileBin2D.h" namespace "YODA":

    cdef cppclass ProfileBin2D(Bin2D_Dbn3D):
        ProfileBin2D (ProfileBin2D h) except+ err
        ProfileBin2D (double, double, double, double) except+ err
        void fill(double x, double y, double z, double weight) except+ err
        void fillBin(double z, double weight) except+ err

        double mean() except+ err
        double stdDev() except+ err
        double variance() except+ err
        double stdErr() except+ err
        double rms() except+ err

        double sumWZ() except+ err
        double sumWZ2() except+ err
        ProfileBin2D operator + (ProfileBin2D) except+ err
        ProfileBin2D operator - (ProfileBin2D) except+ err

# }}} ProfileBin2D


# HistoBin1D {{{
cdef extern from "YODA/HistoBin1D.h" namespace "YODA":
    cdef cppclass HistoBin1D(Bin1D_Dbn1D):
        HistoBin1D(double lowedge, double highedge) except +err
        HistoBin1D(HistoBin1D) except +err
        void fill(double x, double weight) except +err
        void fillBin(double weight) except +err

        double area() except +err
        double height() except +err
        double areaErr() except +err
        double heightErr() except +err
        double relErr() except +err

        HistoBin1D operator+(HistoBin1D) except +err
        HistoBin1D operator-(HistoBin1D) except +err

#}}} HistoBin1D


# HistoBin2D {{{
cdef extern from "YODA/HistoBin2D.h" namespace "YODA":
    cdef cppclass HistoBin2D(Bin2D_Dbn2D):
        HistoBin2D(double xmin, double xmax,
                   double ymin, double ymax) except +err
        HistoBin2D(HistoBin2D) except +err

        void fill(double x, double y, double weight) except +err
        void fillBin(double weight) except +err
        void reset()

        # Accessors
        double volume() except +err
        double volumeErr() except +err
        double height() except +err
        double heightErr() except +err

        HistoBin2D operator+(HistoBin2D) except +err
        HistoBin2D operator-(HistoBin2D) except +err

        #Bin2D_Dbn2D merge(HistoBin2D b)
#}}} HistoBin2D




# AnalysisObject {{{
cdef extern from "YODA/AnalysisObject.h" namespace "YODA":
    cdef cppclass AnalysisObject:
        # Constructors
        AnalysisObject(string type,
                       string path,
                       string title) except+ err
        AnalysisObject(string type,
                       string path,
                       AnalysisObject ao,
                       string title) except+ err
        AnalysisObject()
        # AnalysisObject* newclone() except+ err

        # Annotations
        map[string, string] annotations() except+ err

        bool hasAnnotation(string key) except+ err
        string annotation(string key) except+ err
        string annotation(string key, string default) except+ err
        void setAnnotation(string, string) except+ err
        void rmAnnotation(string name) except+ err
        void clearAnnotations() except+ err

        # Standard annotations
        string title() except+ err
        void setTitle(string title) except+ err
        string path() except+ err
        void setPath(string title) except+ err
        string type() except +err
# }}} AnalysisObject


cdef extern from "YODA/Utils/sortedvector.h" namespace "YODA::Utils":
    cdef cppclass sortedvector[T](vector):
        sortedvector(vector[T]) except +err
        void insert(T) except +err

# TODO: forward declarations for bin-copying constructors




# TODO: Scatter1D / Counter



# Scatter2D {{{
cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    cdef cppclass Scatter2D(AnalysisObject):
        Scatter2D() except+ err

        Scatter2D(string path, string title) except+ err

        Scatter2D(sortedvector[Point2D],
                string path,
                string title) except+ err

        Scatter2D(vector[double], vector[double],
                  vector[pair[double, double]],
                  vector[pair[double, double]]) except+ err

        Scatter2D(Scatter2D p, string path)

        Scatter2D clone() except+ err
        Scatter2D* newclone() except+ err


        void reset() except+ err

        size_t numPoints() except+ err
        Point2D& point(size_t index)

        Scatter2D addPoint(Point2D&)
        Scatter2D addPoint(double, double)
        Scatter2D addPoint(double, double,
                            pair[double, double], pair[double, double]) except+ err

        Scatter2D addPoints(sortedvector[Point2D])

        Scatter2D combineWith(Scatter2D)
        Scatter2D combineWith(vector[Scatter2D])

    void Scatter2D_transformX "YODA::transformX" (Scatter2D&, dbl_dbl_fptr)
    void Scatter2D_transformY "YODA::transformY" (Scatter2D&, dbl_dbl_fptr)

#}}} Scatter2D

cdef extern from "merge.hh":
    Scatter2D* Scatter2D_add_Scatter2D "cython_add" (Scatter2D*, Scatter2D*)
    Scatter2D* Scatter2D_sub_Scatter2D "cython_sub" (Scatter2D*, Scatter2D*)




# Scatter3D {{{
cdef extern from "YODA/Scatter3D.h" namespace "YODA":
    cdef cppclass Scatter3D(AnalysisObject):
        Scatter3D() except+ err

        Scatter3D(string path, string title) except+ err

        Scatter3D(sortedvector[Point3D],
                string path,
                string title) except+ err

        Scatter3D(vector[double], vector[double],
                  vector[pair[double, double]],
                  vector[pair[double, double]],
                  vector[pair[double, double]]) except+ err

        Scatter3D(Scatter3D p, string path)

        Scatter3D clone() except+ err
        Scatter3D* newclone() except+ err


        void reset() except+ err

        size_t numPoints() except+ err
        sortedvector[Point3D] points() except+ err
        Point3D point(size_t index) except+ err

        Scatter3D addPoint(Point3D)
        Scatter3D addPoint(double, double,
                            pair[double, double], pair[double, double]) except+ err

        Scatter3D addPoints(sortedvector[Point3D])

        Scatter3D combineWith(Scatter3D)
        Scatter3D combineWith(vector[Scatter3D])
#}}} Scatter3D



# Profile1D {{{
cdef extern from "YODA/Profile1D.h" namespace "YODA":
    cdef cppclass Profile1D(AnalysisObject):
        Profile1D() except+ err

        Profile1D(string path, string title) except+ err

        Profile1D(size_t nxbins,
                double xlower,
                double xupper,
                string path,
                string title) except+ err

        Profile1D(vector[double] xbinedges,
                string path,
                string title) except+ err

        Profile1D(Profile1D p, string path) except+ err

        Profile1D(Scatter2D s, string path) except+ err

        #Profile1D(Histo1D p, string path)

        Profile1D clone() except+ err
        Profile1D* newclone() except+ err


        void reset() except+ err

        void fill(double x, double y, double weight) except+ err
        void fillBin(size_t i, double y, double weight) except+ err

        void scaleW(double s) except+ err
        void mergeBins(size_t, size_t) except+ err
        void rebin(int n) except+ err

        void addBin(double, double) except+ err
        void addBins(vector[double] edges) except+ err
        # void eraseBin(size_t index) except+ err

        size_t numBins() except+ err
        double lowEdge() except+ err
        double highEdge() except+ err

        vector[ProfileBin1D] bins()
        int binIndexAt(double x) except+ err
        ProfileBin1D bin(size_t ix) except+ err
        ProfileBin1D binAt(double x) except+ err

        # The trick here is to treat these not as references.
        # I suppose when you think about it, it makes sense
        Dbn2D& totalDbn()
        Dbn2D& underflow()
        Dbn2D& overflow()

        double numEntries() # @todo Add bool arg
        double effNumEntries() # @todo Add bool arg
        double sumW(bool)
        double sumW2(bool)

        operator + (Profile1D)
        operator - (Profile1D)
        operator / (Profile1D)

#}}} Profile1D

cdef extern from "merge.hh":
    void Profile1D_iadd_Profile1D "cython_iadd" (Profile1D*, Profile1D*)
    void Profile1D_isub_Profile1D "cython_isub" (Profile1D*, Profile1D*)
    # void Profile1D_imul_dbl "cython_imul_dbl" (Profile1D*, double)
    # void Profile1D_idiv_dbl "cython_idiv_dbl" (Profile1D*, double)
    Profile1D* Profile1D_add_Profile1D "cython_add" (Profile1D*, Profile1D*)
    Profile1D* Profile1D_sub_Profile1D "cython_sub" (Profile1D*, Profile1D*)

cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    Scatter2D mkScatter_Profile1D "YODA::mkScatter" (const Profile1D&)




# Profile2D {{{
cdef extern from "YODA/Profile2D.h" namespace "YODA":
    cdef cppclass Profile2D(AnalysisObject):
        Profile2D() except+ err

        Profile2D(string path, string title) except+ err

        Profile2D(size_t nbinsX, double lowerX, double upperX,
                  size_t nbinsY, double lowerY, double upperY,
                  string path, string title) except+ err

        Profile2D(vector[double] xedges, vector[double] yedges,
                  string path, string title) except+ err

        Profile2D(Profile2D p, string path) except+ err

        #Profile2D(Scatter3D s, string path) except+ err

        #Profile2D(Histo2D p, string path)

        Profile2D clone() except+ err
        Profile2D* newclone() except+ err


        void reset() except+ err

        void fill(double x, double y, double z, double weight) except+ err
        void fillBin(size_t i, double z, double weight) except+ err

        void scaleW(double s) except+ err
        #void mergeBins(size_t, size_t) except+ err
        #void rebin(int n) except+ err

        void addBin(pair[double, double], pair[double, double]) except+ err
        void addBins(vector[double], vector[double]) except+ err
        # void eraseBin(size_t index) except+ err

        size_t numBins() except+ err
        double lowEdge() except+ err
        double highEdge() except+ err

        vector[ProfileBin2D] bins()
        int binIndexAt(double x, double y) except+ err
        ProfileBin2D bin(size_t ix) except+ err
        ProfileBin2D binAt(double x, y) except+ err

        # The trick here is to treat these not as references.
        # I suppose when you think about it, it makes sense
        # -- Cython is a code generator
        Dbn3D totalDbn() except+ err
        Dbn3D outflow(int, int) except+ err

        double numEntries() # @todo Add bool arg
        double effNumEntries() # @todo Add bool arg
        double sumW(bool)
        double sumW2(bool)

        operator + (Profile2D)
        operator - (Profile2D)
        operator / (Profile2D)

#}}} Profile2D

cdef extern from "merge.hh":
    void Profile2D_iadd_Profile2D "cython_iadd" (Profile2D*, Profile2D*)
    void Profile2D_isub_Profile2D "cython_isub" (Profile2D*, Profile2D*)
    # void Profile2D_imul_dbl "cython_imul_dbl" (Profile2D*, double)
    # void Profile2D_idiv_dbl "cython_idiv_dbl" (Profile2D*, double)
    Profile2D* Profile2D_add_Profile2D "cython_add" (Profile2D*, Profile2D*)
    Profile2D* Profile2D_sub_Profile2D "cython_sub" (Profile2D*, Profile2D*)




# Histo1D#{{{
cdef extern from "YODA/Histo1D.h" namespace "YODA":
    cdef cppclass Histo1D(AnalysisObject):
        Histo1D() except +err

        Histo1D(string path, string title) except+ err

        Histo1D(size_t nbins,
                double lower,
                double upper,
                string path,
                string title) except+ err

        Histo1D(vector[double] binedges,
                string path,
                string title) except+ err

        Histo1D(vector[Bin] bins, string path, string title) except+ err

        Histo1D(Histo1D h, string path) except+ err

        #Histo1D(Profile1D p, string path)

        #Histo1D(Scatter2D p, string path)

        Histo1D clone() except+ err
        Histo1D* newclone() except+ err


        void reset() except+ err

        void fill(double x, double weight) except+ err
        void fillBin(size_t i, double weight) except+ err

        void scaleW(double s) except+ err
        void normalize(double normto, bool includeoverflows) except+ err
        void mergeBins(size_t, size_t) except+ err
        void rebin(int n) except+ err

        size_t numBins() except+ err
        double lowEdge() except+ err
        double highEdge() except+ err

        vector[HistoBin1D]& bins()
        int binIndexAt(double x) except+ err
        HistoBin1D& bin(size_t ix)
        HistoBin1D binAt(double x) except+ err

        # TODO: Some Cython mapping problem?
        Dbn1D& totalDbn()
        Dbn1D& underflow()
        Dbn1D& overflow()

        # Whole histo data
        double integral(bool)
        double numEntries() # @todo Add bool arg
        double effNumEntries() # @todo Add bool arg
        double sumW(bool)
        double sumW2(bool)
        double mean(bool)
        double variance(bool)
        double stdDev(bool)
        double stdErr(bool)

        # TODO: BUG! This does not correctly report identical bins...
        void addBin(double, double) except+ err
        void addBins(vector[double] edges) except+ err
        void eraseBin(size_t index) except+ err

        # TODO: declare operators here like for Histo2D? How does that interact with what's below?
        # operator == (Histo2D)
        # operator != (Histo2D)
        # operator + (Histo2D)
        # operator - (Histo2D)
        # operator / (Histo2D)
#}}} Histo1D

cdef extern from "merge.hh":
    void Histo1D_iadd_Histo1D "cython_iadd" (Histo1D*, Histo1D*)
    void Histo1D_isub_Histo1D "cython_isub" (Histo1D*, Histo1D*)
    # void Histo1D_imul_dbl "cython_imul_dbl" (Histo1D*, double)
    # void Histo1D_idiv_dbl "cython_idiv_dbl" (Histo1D*, double)
    Histo1D* Histo1D_add_Histo1D "cython_add" (Histo1D*, Histo1D*)
    Histo1D* Histo1D_sub_Histo1D "cython_sub" (Histo1D*, Histo1D*)

cdef extern from "YODA/Scatter2D.h" namespace "YODA":
    Scatter2D mkScatter_Histo1D "YODA::mkScatter" (const Histo1D&)




# Histo2D {{{
cdef extern from "YODA/Histo2D.h" namespace "YODA":
    cdef cppclass Histo2D(AnalysisObject):
        Histo2D() except+ err

        Histo2D(string path, string title) except+ err

        Histo2D(size_t nBinsX, double lowerX, double upperX,
                size_t nBinsY, double lowerY, double upperY,
                string path, string title) except+ err

        Histo2D(vector[double] xedges, vector[double] yedges,
                string path, string title) except+ err

        Histo2D(Histo2D, string path)
        #Histo2D(Profile1D p, string path)
        #Histo2D(Scatter2D p, string path)

        Histo2D clone() except+ err
        Histo2D* newclone() except+ err


        void reset() except+ err

        void fill(double x, double y, double weight) except+ err
        void fillBin(size_t i, double weight) except+ err

        void scaleW(double scalefactor) except+ err
        void normalize(double normto, bool includeoverflows) except+ err
        void scaleXY(double, double)

        void mergeBins(size_t, size_t) except+ err
        void rebin(int n) except+ err
        size_t numBins() except+ err

        vector[HistoBin1D] bins()
        int binIndexAt(double x, double y) except+ err
        HistoBin2D bin(size_t ix) except+ err
        HistoBin2D binAt(double x, double y) except+ err

        void addBins(vector[HistoBin2D]&)

        void addBin(pair[double, double], pair[double, double])

        # TODO: Still some Cython mapping problem? Refs work ok on Histo1D
        #
        # These must be treated as references or the semantics is wrong.
        # However, these can also throw exceptions! But the two cannot mix, or
        # Cython puts out rubbish C++. Since this *is* a reported 'major' bug,
        # we should expect it to be fixed sometime in the future.
        Dbn2D& totalDbn() #except +err
        Dbn2D& outflow(int, int) #except +err

        # Bin accessors
        #void addBin(double, double) except+ err
        #void addBins(vector[double] edges) except+ err
        void eraseBin(size_t index) except+ err

        double lowEdgeX() except+ err
        double lowEdgeY() except+ err

        double highEdgeX() except+ err
        double highEdgeY() except+ err

        size_t numBinsX()
        size_t numBinsY()

        # Whole histo data
        double integral(bool)
        double numEntries() # @todo Add bool arg
        double effNumEntries() # @todo Add bool arg
        double sumW(bool)
        double sumW2(bool)
        double xMean(bool)
        double yMean(bool)
        double xVariance(bool)
        double yVariance(bool)
        double xStdDev(bool)
        double yStdDev(bool)
        double xStdErr(bool)
        double yStdErr(bool)

        operator == (Histo2D)
        operator != (Histo2D)
        operator + (Histo2D)
        operator - (Histo2D)
        operator / (Histo2D)
# Histo2D }}}

cdef extern from "merge.hh":
    void Histo2D_iadd_Histo2D "cython_iadd" (Histo2D*, Histo2D*)
    void Histo2D_isub_Histo2D "cython_isub" (Histo2D*, Histo2D*)
    # void Histo2D_imul_dbl "cython_imul_dbl" (Histo2D*, double)
    # void Histo2D_idiv_dbl "cython_idiv_dbl" (Histo2D*, double)
    Histo2D* Histo2D_add_Histo2D "cython_add" (Histo2D*, Histo2D*)
    Histo2D* Histo2D_sub_Histo2D "cython_sub" (Histo2D*, Histo2D*)




# Streams {{{

cdef extern from "<sstream>" namespace "std":
    cdef cppclass ostringstream:
        ostringstream()
        string& str()

cdef extern from "<sstream>" namespace "std":
    cdef cppclass istringstream:
        istringstream()
        string& str(string&)


cdef extern from "YODA/Reader.h" namespace "YODA":
    cdef cppclass Reader:
        void read(istringstream&, vector[AnalysisObject*]&) except +err

cdef extern from "YODA/ReaderYODA.h" namespace "YODA":
    Reader& ReaderYODA_create "YODA::ReaderYODA::create" ()

cdef extern from "YODA/ReaderFLAT.h" namespace "YODA":
    Reader& ReaderFLAT_create "YODA::ReaderFLAT::create" ()

cdef extern from "YODA/ReaderAIDA.h" namespace "YODA":
    Reader& ReaderAIDA_create "YODA::ReaderAIDA::create" ()

cdef extern from "YODA/Reader.h" namespace "YODA":
    Reader& Reader_create "YODA::mkReader" (string& filename)


cdef extern from "YODA/Writer.h" namespace "YODA":
    cdef cppclass Writer:
        void write(ostringstream&, vector[AnalysisObject*]&)

cdef extern from "YODA/WriterYODA.h" namespace "YODA":
    Writer& WriterYODA_create "YODA::WriterYODA::create" ()

cdef extern from "YODA/WriterFLAT.h" namespace "YODA":
    Writer& WriterFLAT_create "YODA::WriterFLAT::create" ()

cdef extern from "YODA/WriterAIDA.h" namespace "YODA":
    Writer& WriterAIDA_create "YODA::WriterAIDA::create" ()

cdef extern from "YODA/Reader.h" namespace "YODA":
    Writer& Writer_create "YODA::mkWriter" (string& filename)

# Streams }}}



# Axis1D {{{
cdef extern from "YODA/Axis1D.h" namespace "YODA":
    cdef cppclass Axis1D[BIN1D, DBN]:
        Axis1D() except+ err
        Axis1D(vector[double] binedges) except+ err
        Axis1D(size_t, double, double) except+ err
        Axis1D(vector[BIN1D] bins) except+ err
        void addBin(double, double) except+ err
        size_t numBins() except+ err
        vector[BIN1D]& bins()
        double lowEdge() except+ err
        double highEdge() except+ err
        long getBinIndex(double)
        void reset()
        DBN& totalDbn()
        DBN& underflow()
        DBN& overflow()
        void eraseBin(size_t index) except+ err
        void mergeBins(size_t, size_t) except+ err
# Axis1D }}}



# Axis2D {{{
cdef extern from "YODA/Axis2D.h" namespace "YODA":
    cdef cppclass Axis2D[BIN2D, DBN]:
        Axis2D() except+ err
        Axis2D(vector[double], vector[double]) except+ err
        Axis2D(size_t, pair[double, double], size_t, pair[double, double]) except+ err
        Axis2D(vector[BIN2D] bins) except+ err
        void addBin(pair[double, double], pair[double, double]) except+ err
        size_t numBins() except+ err
        vector[BIN2D]& bins()
        double lowEdgeX() except+ err
        double highEdgeX() except+ err
        double lowEdgeY() except+ err
        double highEdgeY() except+ err
        long getBinIndex(double, double)
        void reset()
        DBN& totalDbn()
        DBN& outflow(int, int)
        void eraseBin(size_t index) except+ err
        void mergeBins(size_t, size_t) except+ err
# Axis2D }}}
