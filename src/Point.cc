#include "YODA/Point.h"
#include "YODA/MathUtils.h"
#include "YODA/Exception.h"

using namespace std;

namespace YODA {
  

  // Symmetric errors
  // Point::Point(const vector<double>& value,
  //              const vector<vector<double> >& errors)
  //   : _value(value)
  // {
  //   setErrors(errors);
  // }
  

  ///////////////////////////////


  
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
  

  // void addError(const std::vector<double>& error) {
  // }


  // void addError(const std::vector<std::pair<double,double> >& error) {
  // }


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
  


}
