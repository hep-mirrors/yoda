#include "YODA/Point.h"
#include "YODA/MathUtils.h"
#include "YODA/Exception.h"

using namespace std;

namespace YODA {


  // Default constructor
  Point::Point() 
  {  }


  // Values but no errors
  Point::Point(const vector<double>& value)
    : _value(value)
  {  }


  // Asymmetric (general) errors
  Point::Point(const vector<double>& value,
               const ErrorSet& errors)
    : _value(value), _errors(errors)
  {  }
  

  // Symmetric errors
  // Point::Point(const vector<double>& value,
  //              const vector<vector<double> >& errors)
  //   : _value(value)
  // {
  //   setErrors(errors);
  // }
  

  ///////////////////////////////


  vector<double> Point::value() const {
    return _value;
  }


  double Point::value(size_t dim) const {
    assert(dim < numDims());
    return _value.at(dim);
  }


  void Point::setValue(const vector<double>& values) {
    assert(values.size() == numDims());
    _value = values;
  }


  void Point::setValue(size_t dim, double value) {
    assert(dim < numDims());
    _value[dim] = value;
  }



  void Point::setErrors(const ErrorSet& errors) {
    _errors = errors;
  }


  // void setErrors(const std::vector<std::vector<std::pair<double,double> > >& errors) {
  //   assert(errors.size() == numDims());
  //   for (size_t dim = 0; dim != errors.size(); ++dim) {
  //     for (vector<double>::const_iterator e = errors[i].begin(); e != errors[i].end(); ++e) {
  //       _errors[dim].push_back(PointError(dim, *e));
  //     }
  //   }
  // }


  // void setErrors(const std::vector<std::vector<double> >& errors) {
  //   assert(errors.size() == numDims());
  //   for (size_t dim = 0; dim != errors.size(); ++dim) {
  //     for (vector<double>::const_iterator e = errors[i].begin(); e != errors[i].end(); ++e) {
  //       _errors[dim].push_back(PointError(dim, *e));
  //     }
  //   }
  // }
  

  void Point::addError(const PointError& error) {
    _errors.push_back(error);
  }


  // void addError(const std::vector<double>& error) {
  // }


  // void addError(const std::vector<std::pair<double,double> >& error) {
  // }


  pair<double,double> Point::error(size_t dim, ErrorCombiner& ec) const {
    assert(dim < numDims());
    return ec.combine_errs(_errors.begin(), _errors.end()).at(dim);
  }


  pair<double,double> Point::error(size_t dim, ErrorCombScheme ecs) const {
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


  double Point::symmError(size_t dim, ErrorCombiner& ec) const {
    pair<double,double> errs = error(dim, ec);
    return (errs.first + errs.second)/2.0;
  }


  double Point::symmError(size_t dim, ErrorCombScheme ecs) const {
    pair<double,double> errs = error(dim, ecs);
    return (errs.first + errs.second)/2.0;
  }

  /// @todo Reinstate

  // vector<PointError> Point::errors(ErrorCombiner& ec) const {
  //   vector<PointError> rtn(numDims());
  //   for (size_t dim = 0; dim < numDims(); ++dim) {
  //     rtn[dim] = error(dim, ec);
  //   }
  //   return rtn;
  // }


  // vector<PointError> Point::errors(ErrorCombScheme ecs) const {
  //   vector<PointError> rtn(numDims());
  //   for (size_t dim = 0; dim < numDims(); ++dim) {
  //     rtn[dim] = error(dim, ecs);
  //   }
  //   return rtn;
  // }


  // vector<double> Point::symmErrors(ErrorCombiner& ec) const {
  //   vector<double> rtn(numDims());
  //   for (size_t dim = 0; dim < numDims(); ++dim) {
  //     const double err = symmError(dim, ec);
  //     rtn[dim] = err;
  //   }
  //   return rtn;
  // }


  // vector<double> Point::symmErrors(ErrorCombScheme ecs) const {
  //   vector<double> rtn(numDims());
  //   for (size_t dim = 0; dim < numDims(); ++dim) {
  //     const double err = symmError(dim, ecs);
  //     rtn[dim] = err;
  //   }
  //   return rtn;
  // }



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
  Point1D::Point1D(const vector<double>& value,
                   const ErrorSet& errors)
    : Point(value, errors)
  {  }
  

  // Symmetric errors
  // Point1D::Point1D(const vector<double>& values,
  //                  const vector<double>& errors)
  //   : Point(values)
  // {
  //   for (vector<double>::const_iterator e = errors.begin();
  //        e != errors.end(); ++e) {
  //     ErrorSet es;
  //     PointError pe = make_pair(*e,*e);
  //     es.push_back(pe);
  //     _errors.push_back(es);
  //   }
  //
  // }
  

  size_t Point1D::numDims() const {
    return 1;
  }



  //////////////////////////////////////////////


  // Default constructor
  Point2D::Point2D() 
    : Point()
  {  }


  // Values but no errors
  Point2D::Point2D(const vector<double>& value)
    : Point(value)
  {  }


  // Asymmetric (general) errors
  Point2D::Point2D(const vector<double>& value,
                   const ErrorSet& errors)
    : Point(value, errors)
  {  }
  

  // Symmetric errors
  // Point2D::Point2D(const vector<double>& values,
  //                  const vector<double>& errors)
  //   : Point(values)
  // {
  //   for (vector<double>::const_iterator e = errors.begin();
  //        e != errors.end(); ++e) {
  //     ErrorSet es;
  //     PointError pe = make_pair(*e,*e);
  //     es.push_back(pe);
  //     _errors.push_back(es);
  //   }
  // }
  

  size_t Point2D::numDims() const {
    return 2;
  }


}
