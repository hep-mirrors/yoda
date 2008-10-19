#include "YODA/Point.h"
#include "YODA/MathUtils.h"
#include "YODA/Exception.h"

using namespace std;

namespace YODA {


  // Default constructor
  Point::Point() 
  {  }


  // Values but no errors
  Point::Point(const vector<double>& values)
    : _values(values)
  {  }


  // Asymmetric (general) errors
  Point::Point(const vector<double>& values,
               const vector<ErrorSet>& errors)
    : _values(values), _errors(errors)
  {  }
  

  // Symmetric errors
  Point::Point(const vector<double>& values,
               const vector<vector<double> >& errors)
    : _values(values)
  {
    /// @todo Replace this with symm error constructors on PointError.
    for (vector<vector<double> >::const_iterator es = errors.begin(); es != errors.end(); ++es) {
      vector<pair<double, double> > errs;
      for (vector<double>::const_iterator e = es->begin(); e != es->end(); ++e) {
        errs.push_back(make_pair(*e, *e));
      }
      _errors.push_back(errs);
    }
  }
  

  ///////////////////////////////


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
      throw Exception("HYBRID_COMB error combination not yet supported");
      /// @todo HYBRID_COMB error combination support needed
      //QuadLinErrComb qlec;
      //rtn = error(qlec);
    } else {
      throw Exception("Requested unknown error combination scheme... how?!?");
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
    vector<double> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      const double err = symmError(dim, ec);
      rtn[dim] = err;
    }
    return rtn;
  }


  vector<double> Point::symmErrors(ErrorCombScheme ecs) {
    vector<double> rtn(numDims());
    for (size_t dim = 0; dim < numDims(); ++dim) {
      const double err = symmError(dim, ecs);
      rtn[dim] = err;
    }
    return rtn;
  }



  //////////////////////////////////////////////


  // Default constructor
  Point1D::Point1D() 
    : Point()
  {  }


  // Values but no errors
  Point1D::Point1D(const vector<double>& values)
    : Point(values)
  {  }


  // Asymmetric (general) errors
  Point1D::Point1D(const vector<double>& values,
                   const vector<pair<double,double> >& errors)
    : Point(values)
  {  
    for (vector<pair<double,double> >::const_iterator e = errors.begin();
         e != errors.end(); ++e) {
      ErrorSet es;
      es.push_back(*e);
      _errors.push_back(es);
    }
  }
  

  // Symmetric errors
  Point1D::Point1D(const vector<double>& values,
                   const vector<double>& errors)
    : Point(values)
  {
    for (vector<double>::const_iterator e = errors.begin();
         e != errors.end(); ++e) {
      ErrorSet es;
      PointError pe = make_pair(*e,*e);
      es.push_back(pe);
      _errors.push_back(es);
    }

}
  

  size_t Point1D::numDims() const {
    return 1;
  }



  //////////////////////////////////////////////


  // Default constructor
  Point2D::Point2D() 
    : Point()
  {  }


  // Values but no errors
  Point2D::Point2D(const vector<double>& values)
    : Point(values)
  {  }


  // Asymmetric (general) errors
  Point2D::Point2D(const vector<double>& values,
                   const vector<pair<double,double> >& errors)
    : Point(values)
  {  
    for (vector<pair<double,double> >::const_iterator e = errors.begin();
         e != errors.end(); ++e) {
      ErrorSet es;
      es.push_back(*e);
      _errors.push_back(es);
    }
  }
  

  // Symmetric errors
  Point2D::Point2D(const vector<double>& values,
                   const vector<double>& errors)
    : Point(values)
  {
    for (vector<double>::const_iterator e = errors.begin();
         e != errors.end(); ++e) {
      ErrorSet es;
      PointError pe = make_pair(*e,*e);
      es.push_back(pe);
      _errors.push_back(es);
    }
  }
  

  size_t Point2D::numDims() const {
    return 2;
  }


}
