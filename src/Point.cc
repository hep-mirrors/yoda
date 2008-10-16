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


  //std::vector<std::pair<double,double> > Point::errors();
  //std::vector<double> Point::symmErrors();



  //////////////////////////////////////////////



  // // Asymmetric (general) errors
  // Point2D::Point1D(const std::vector<double>& values,
  //         const std::vector<std::pair<double,double> >& errors);
  
  // // Symmetric errors
  // Point2D::Point1D(const std::vector<double>& values,
  //         const std::vector<double>& errors);
  
  size_t Point1D::numDims() {
    return 1;
  }



  //////////////////////////////////////////////



  // // Asymmetric (general) errors
  // Point2D::Point2D(const std::vector<double>& values,
  //         const std::vector<std::pair<double,double> >& errors);
  
  // // Symmetric errors
  // Point2D::Point2D(const std::vector<double>& values,
  //         const std::vector<double>& errors);

  size_t Point2D::numDims() {
    return 2;
  }


}
