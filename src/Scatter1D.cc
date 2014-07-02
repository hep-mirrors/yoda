#include "YODA/ScatterND.h"

namespace YODA {

  /// @todo Finish and make like the other scatters


  /// A collection of 1D data points with errors
  class Scatter1D : public Scatter<1> {
  public:

    typedef Utils::sortedvector< Point<1> > Points;


    /// @name Constructors
    //@{

    /// Empty constructor
    Scatter1D(const std::string& path="", const std::string& title="")
      : Scatter<1>(path, title)
    {  }


    /// Constructor from a set of points
    Scatter1D(const Points& points,
              const std::string& path="", const std::string& title="")
      : Scatter<1>(points, path, title)
    {  }

  };


  /// Make a Scatter2D representation of a Counter
  Scatter1D mkScatter(const Histo1D& h) {
    Scatter2D rtn;
    BOOST_FOREACH (const std::string& a, h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type()); // might override the copied ones
    BOOST_FOREACH (const HistoBin1D& b, h.bins()) {
      // const double x = b.focus();
      // const double ex_m = b.focus() - b.lowEdge();
      // const double ex_p = b.highEdge() - b.focus();
      const double x = b.midpoint();
      const double ex_m = b.width()/2.0;
      const double ex_p = b.width()/2.0;
      const double y = b.height();
      const double ey = b.heightErr();
      const Point2D pt(x, y, ex_m, ex_p, ey, ey);
      rtn.addPoint(pt);
    }
    assert(h.numBins() == rtn.numPoints());
    return rtn;
  }


  /// @todo mkScatter from a vector of Counters?


  void test() {
    Utils::ndarray<double,2> bar; // in C++-11 we can do bar({1,2}) instead
    bar[0] = 1;
    bar[1] = 2;

    Utils::ndarray<double,2> baz; // in C++-11 we can do p2({{3,4}},...) instead below
    baz[0] = 3;
    baz[1] = 4;

    Point<2> p1(bar);
    Point<2> p2(baz, bar);

    Scatter<2> s2;
    s2.addPoint(p1);
    s2.addPoint(p2);
  }


}
