// -*- C++ -*-
#ifndef YODA_MathUtils_H
#define YODA_MathUtils_H

#include <stdexcept>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace YODA {

  
  const double MAXDOUBLE = std::numeric_limits<double>::max();
  const double MAXINT = std::numeric_limits<int>::max();

  /// A pre-defined value of \f$ \pi \f$.
  const double PI = M_PI;

  /// A pre-defined value of \f$ 2\pi \f$.
  const double TWOPI = 2*M_PI;

  /// A pre-defined value of \f$ \pi/2 \f$.
  const double HALFPI = M_PI_2;

  /// Enum for signs of numbers.
  enum Sign { MINUS = -1, ZERO = 0, PLUS = 1 };

  
  /// Compare a floating point number to zero with a degree 
  /// of fuzziness expressed by the absolute @a tolerance parameter.
  inline bool isZero(double val, double tolerance=1E-8) {
    return (fabs(val) < tolerance);
  }

  /// Compare an integral-type number to zero. Since there is no
  /// risk of floating point error, this function just exists in
  /// case @c isZero is accidentally used on an integer type, to avoid 
  /// implicit type conversion. The @a tolerance parameter is ignored.
  inline bool isZero(long val, double tolerance=1E-8) {
    return val == 0;
  }

  /// Compare a number to zero with a degree of fuzziness expressed by the
  /// absolute @a tolerance parameter.
  inline int sign(double val) {
    if (isZero(val)) return ZERO;
    const int valsign = (val > 0) ? PLUS : MINUS;
    //cerr << val << " -> sign = " << valsign << endl;
    return valsign;
  }

  /// Compare a number to zero with a degree of fuzziness expressed by the
  /// absolute @a tolerance parameter.
  inline int sign(long val) {
    if (val == 0) return ZERO;
    return (val > 0) ? PLUS : MINUS;
  }

  /// Compare two floating point numbers with a degree of fuzziness 
  /// expressed by the fractional @a tolerance parameter.
  inline bool fuzzyEquals(double a, double b, double tolerance=1E-5) {
    const double absavg = fabs(a + b)/2.0;
    const double absdiff = fabs(a - b);
    const bool rtn = (absavg == 0.0 && absdiff == 0.0) || absdiff/absavg < tolerance;
    return rtn;
  }

  /// Compare two integral-type numbers with a degree of fuzziness.
  /// Since there is no risk of floating point error with integral types, 
  /// this function just exists in case @c fuzzyEquals is accidentally 
  /// used on an integer type, to avoid implicit type conversion. The @a 
  /// tolerance parameter is ignored, even if it would have an
  /// absolute magnitude greater than 1.
  inline bool fuzzyEquals(long a, long b, double tolerance=1E-5) {
    return a == b;
  }

  /// Represents whether an interval is open (non-inclusive) or closed
  /// (inclusive). For example, the interval \f$ [0, \pi) \f$ is closed (an inclusive
  /// boundary) at 0, and open (a non-inclusive boundary) at \f$ \pi \f$.
  enum RangeBoundary { OPEN=0, SOFT=0, CLOSED=1, HARD=1 };

  /// Determine if @a value is in the range @a low to @a high, with boundary
  /// types defined by @a lowbound and @a highbound.
  /// @todo Optimise to one-line at compile time?
  inline bool inRange(double value, double low, double high, 
                      RangeBoundary lowbound=OPEN, RangeBoundary highbound=OPEN) {
    if (lowbound == OPEN && highbound == OPEN) {
      return (value > low && value < high);
    } else if (lowbound == OPEN && highbound == CLOSED) {
      return (value > low && value <= high);
    } else if (lowbound == CLOSED && highbound == OPEN) {
      return (value >= low && value < high);
    } else { // if (lowbound == CLOSED && highbound == CLOSED) {
      return (value >= low && value <= high);
    }
  }

  /// Named number-type squaring operation.
  template <typename Num>
  inline Num sq(Num a) {
    return a*a;
  }

  /// Named number-type addition in quadrature operation.
  template <typename Num>
  inline Num add_quad(Num a, Num b) {
    return sqrt(a*a + b*b);
  }

  /// Named number-type addition in quadrature operation.
  template <typename Num>
  inline Num add_quad(Num a, Num b, Num c) {
    return sqrt(a*a + b*b + c*c);
  }


}

#endif
