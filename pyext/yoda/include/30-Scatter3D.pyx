cdef extern from "YODA/Scatter3D.h" namespace "YODA":
    cdef cppclass cScatter3D "YODA::Scatter3D" (cAnalysisObject):
        cScatter3D()
        cScatter3D(string& path, string& title)
        cScatter3D(vector[cPoint3D]&, string& path, string& title)
        cScatter3D(cScatter3D &s)

        size_t numPoints()
        vector[cPoint3D] points()

        cPoint3D& point(size_t i)
        void erasePoint(size_t i)


cdef class Scatter3D(AnalysisObject):
    """
    3D scatter plot / cf. TGraphAsymmErrs. Unlike histograms/profiles, the position and error
    values of Scatter points can be set directly. This 3-dimensional scatter is primarily used
    for reference versions of 2D histograms/profiles, or for the results of e.g. division of two
    2D histograms (which is not itself a *histogram* in the sense of something with a fill
    history which can continue to be filled.)

    Several sets of arguments are permitted to the constructor:

    * Scatter3D() -- default constructor. Not usually useful in Python, due to availability of None.
    * Scatter3D(points[, path, title]) -- explicit construction from a list of points.
    * Scatter3D(xs, ys, zs[, path, title]) -- constructing points from lists of x, y and z positions (no errs).
    * Scatter3D(xs, ys, zs, exs, eys, ezs[, path, title]) -- constructing points from lists of x, y and z positions and errs (errs can be pairs).
    * Scatter3D(xs, ys, zs, ex-s, ex+s, ey-s, ey+s, ez-s, ez+s[, path, title]) -- constructing points from lists of x, y and z positions and errs.

    The path and title arguments are optional, and may either be specified via the
    positional parameters or via explicit keyword arguments, e.g. path='/foo/bar'.
    """

    def __init__(self, *args, **kwargs):
        #self._dealloc = True
        cdef:
            Point3D item
            vector[cPoint3D] point_vector
            cScatter3D* scatter
            char* path = '/'
            char* title = ''

        ## Permit path and title specification via kwargs
        if "path" in kwargs:
            path = kwargs["path"]
        if "title" in kwargs:
            title = kwargs["title"]

        ## Trigger different construction methods depending on Python args
        # TODO: Map copy constructors, esp. the path-resetting one
        if len(args) == 0:
            self.setptr(new cScatter3D(string(path), string(title)))
            return
        #
        if len(args) == 1:
            # Scatter3D(points[, path, title])
            for point in args[0]:
                item = point
                point_vector.push_back( item.ptr()[0] )
        elif len(args) <= 9:
            reflen = len(args[0])
            ## Check that lengths of vector arguments agree
            for i in xrange(len(args)):
                assert len(args[i]) == reflen
            for i in xrange(reflen):
                # TODO: Use zip(*args) to compress these to one meta-constructor call for n vector args?
                if len(args) == 3:
                    # Scatter3D(xs, ys, zs[, path, title])
                    item = Point3D(args[0][i], args[1][i], args[2][i])
                elif len(args) == 6:
                    # Scatter3D(xs, ys, zs, exs, eys, ezs[, path, title])
                    item = Point3D(args[0][i], args[1][i], args[2][i],
                                   args[3][i], args[4][i], args[5][i])
                elif len(args) == 9:
                    # Scatter3D(xs, ys, zs, ex-s, ex+s, ey-s, ey+s, ez-s, ez+s[, path, title])
                    item = Point3D(args[0][i], args[1][i], args[2][i],
                                   args[3][i], args[4][i], args[5][i],
                                   args[6][i], args[7][i], args[8][i])
                point_vector.push_back( item.ptr()[0] )
        else:
            raise ValueError('Wrong number of values: can take 3, 6, or 9 parameters')

        ## Finally, construct using the intermediate point vector
        scatter = new cScatter3D(point_vector, string(path), string(title))
        self.setptr(scatter, True)


    @property
    def numPoints(self):
        "The number of points in this scatter"
        return self.ptr().numPoints()

    cdef cScatter3D * ptr(self):
        return <cScatter3D *> self.thisptr

    def __getitem__(self, args):
        return self.points.__getitem__(args)

    def copy(self):
        return Scatter3D_fromptr(new cScatter3D(self.ptr()[0]), True)

    @property
    def points(self):
        "Access the points"
        cdef size_t i
        cdef Point3D pt
        return tuple(Point3D_fromptr(& self.ptr().point(i)) for i in xrange(self.ptr().numPoints()))

    def __repr__(self):
        # TODO: include points information in this repr string
        return '<Scatter3D>'


# dealloc decides whether or not the python object is responsible for freeing
# used memory. Most times, it's not - we're just wrapping a C++ instance.
# However, the same classes should be used for both wrapping and creating.

# It is important that we do free allocated C++ memory, because otherwise we can
# quickly end up with memory leaks.

cdef Scatter3D Scatter3D_fromptr(cScatter3D* ptr, dealloc = False):
    cdef Scatter3D scatter = Scatter3D.__new__(Scatter3D)
    return scatter.setptr(ptr, False)
