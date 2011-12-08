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
    Scatter<2> s2;
    Utils::ndarray<double,2> bar({1,2});
    Point<2> p1(bar);
    Point<2> p2({{3,4}}, bar);
    s2.addPoint(p1);
    s2.addPoint(p2);
  }


}
