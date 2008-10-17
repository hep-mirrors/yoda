#include "YODA/Point.h"
#include "YODA/MathUtils.h"
#include "YODA/Exception.h"

using namespace std;

namespace YODA {


  Point::Point() { }


  double Point::value(size_t dim) {
    assert(dim < numDims());
    return _values.at(dim);
  }


  struct QuadErrComb : public ErrorCombiner {
    pair<double,double> combine_errs(const Point::ErrorSet::const_iterator& begin,
                                     const Point::ErrorSet::const_iterator& end) {
      double up(0), down(0);
      for (Point::ErrorSet::const_iterator e = begin; e != end; ++e) {
        down += sq(e->first);
        up += sq(e->second);
      }
      return make_pair(sqrt(down), sqrt(up));
    }
  };


  struct LinErrComb : public ErrorCombiner {
    pair<double,double> combine_errs(const Point::ErrorSet::const_iterator& begin,
                                     const Point::ErrorSet::const_iterator& end) {
      double up(0), down(0);
      for (Point::ErrorSet::const_iterator e = begin; e != end; ++e) {
        down += e->first;
        up += e->second;
      }
      return make_pair(down, up);
    }
  };


  pair<double,double> Point::error(size_t dim, ErrorCombiner& ec) {
    assert(dim < numDims());
    return ec.combine_errs(_errors.at(dim).begin(), _errors.at(dim).end());
  }


  pair<double,double> Point::error(size_t dim, ErrorCombScheme ecs) {
    assert(dim < numDims());
    pair<double,double> rtn = make_pair(0.0, 0.0);
    if (ecs == QUAD_COMB) {
      QuadErrComb qec;
      rtn = error(dim, qec);
    } else if (ecs == LIN_COMB) {
      LinErrComb lec;
      rtn = error(dim, lec);
    } else if (ecs == HYBRID_COMB) {
      throw Error("HYBRID_COMB error combination not yet supported");
      /// @todo HYBRID_COMB error combination support needed
      //QuadLinErrComb qlec;
      //rtn = error(qlec);
    } else {
      throw Error("Requested unknown error combination scheme... how?!?");
    }
    return rtn;
  }


  double Point::symmError(size_t dim, ErrorCombiner& ec) {
    pair<double,double> errs = error(dim, ec);
    return (errs.first + errs.second)/2.0;
  }


  double Point::symmError(size_t dim, ErrorCombScheme ecs) {
    pair<double,double> errs = error(dim, ecs);
    return (errs.first + errs.second)/2.0;
  }


  vector<Point::PointError> Point::errors(ErrorCombiner& ec) {
    vector<PointError> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      rtn[dim] = error(dim, ec);
    }
    return rtn;
  }


  vector<Point::PointError> Point::errors(ErrorCombScheme ecs) {
    vector<PointError> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      rtn[dim] = error(dim, ecs);
    }
    return rtn;
  }


  vector<double> Point::symmErrors(ErrorCombiner& ec) {
    vector<PointError> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      rtn[dim] = symmError(dim, ec);
    }
    return rtn;
  }


  vector<Point::PointError> Point::symmErrors(ErrorCombScheme ecs) {
    vector<double> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      rtn[dim] = symmError(dim, ecs);
    }
    return rtn;
  }



  //////////////////////////////////////////////



  // Asymmetric (general) errors
  Point1D::Point1D(const std::vector<double>& values,
                   const std::vector<std::pair<double,double> >& errors)
    : _values(values), _errors(errors)
  {  }
  

  // Symmetric errors
  Point1D::Point1D(const std::vector<double>& values,
                   const std::vector<double>& errors)
    : _values(values)
  {
    /// @todo Replace this with symm error constructors on PointError.
    for (std::vector<double>::const_iterator e = errors.begin(); e != errors.end(); ++e) {
      _errors.push_back(make_pair(*e, *e));
    }
  }
  

  size_t Point1D::numDims() const {
    return 1;
  }



  //////////////////////////////////////////////


  Point2D::Point2D(const std::vector<double>& values,
                   const std::vector<std::pair<double,double> >& errors)
    : _values(values), _errors(errors)
  {  }
  

  // Symmetric errors
  Point2D::Point2D(const std::vector<double>& values,
                   const std::vector<double>& errors)
    : _values(values)
  {
    /// @todo Replace this with symm error constructors on PointError.
    for (std::vector<double>::const_iterator e = errors.begin(); e != errors.end(); ++e) {
      _errors.push_back(make_pair(*e, *e));
    }
  }


  size_t Point2D::numDims() const {
    return 2;
  }


}
