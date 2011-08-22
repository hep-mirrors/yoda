#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"
#include "YODA/Exceptions.h"

namespace YODA {


  // Add two scatters
  /// @todo Implementation in a named add() function
  Scatter3D operator + (const Scatter3D& first, const Scatter3D& second) {
    /// @todo Implement
    throw Exception("Implement me!");
    Scatter3D tmp;
    return tmp;
  }


  // Subtract two scatters
  /// @todo Implementation in a named subtract() function
  Scatter3D operator - (const Scatter3D& first, const Scatter3D& second) {
    /// @todo Implement
    throw Exception("Implement me!");
    Scatter3D tmp;
    return tmp;
  }


  /// Divide two scatters
  Scatter3D divide(const Scatter3D& numer, const Scatter3D& denom) {
    Scatter3D tmp;
    for (size_t i = 0; i < numer.numPoints(); ++i) {
      const Point3D& p1 = numer.point(i);
      const Point3D& p2 = denom.point(i);

      assert(fuzzyEquals(p1.xMin(), p2.xMin()));
      assert(fuzzyEquals(p1.xMax(), p2.xMax()));

      assert(fuzzyEquals(p1.yMin(), p2.yMin()));
      assert(fuzzyEquals(p1.yMax(), p2.yMax()));

      const double x = (p1.x() + p2.x())/2.0;
      const double y = (p1.y() + p2.y())/2.0;
      //
      const double z = p1.z() / p2.z();
      /// @todo Generally deal with +/- errors separately
      const double ez = z * sqrt( sqr(p1.yErrAvg()/p1.z()) + sqr(p2.yErrAvg()/p2.z()) );
      tmp.addPoint(x, p1.xErrMinus(), p1.xErrPlus(),
                   y, p1.yErrMinus(), p1.yErrPlus(), z, ez, ez);
    }
    assert(tmp.numPoints() == numer.numPoints());
    return tmp;
  }


}
