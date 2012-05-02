#include "YODA/ScatterND.h"

namespace YODA {


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
