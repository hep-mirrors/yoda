#include "YODA/Error.h"
#include "YODA/MathUtils.h"
#include "YODA/Exception.h"
#include <map>

using namespace std;

namespace YODA {


  Error1D::Error1D() 
  {  }


  Error1D::Error1D(double symm_err) 
    : _minus(symm_err), _plus(symm_err)
  {  }


  Error1D::Error1D(pair<double,double> pm_errs) 
  {  
    _minus = pm_errs.first;
    _plus = pm_errs.second;
  }


  Error1D::Error1D(double minus_err, double plus_err) 
    : _minus(minus_err), _plus(plus_err)
  {  }
  

  double Error1D::minusErr() const {  
    return _minus;
  }


  double Error1D::plusErr() const {  
    return _plus;
  }


  double Error1D::symmErr() const {  
    return ( minusErr() + plusErr() ) / 2.0;
  }


  pair<double,double> Error1D::errs() const {  
    return make_pair(minusErr(), plusErr());
  }
  

  Error1D& Error1D::setMinusErr(double minus_err) {  
    _minus = minus_err;
    return *this;
  }


  Error1D& Error1D::setPlusErr(double plus_err) {  
    _plus = plus_err;
    return *this;
  }


  Error1D& Error1D::setErrs(double symm_err) {  
    setMinusErr(symm_err);
    setPlusErr(symm_err);
    return *this;
  }


  Error1D& Error1D::setErrs(pair<double,double> pm_errs) {  
    setMinusErr(pm_errs.first);
    setPlusErr(pm_errs.second);
    return *this;
  }


  Error1D& Error1D::setErrs(double minus_err, double plus_err) {  
    setMinusErr(minus_err);
    setPlusErr(plus_err);
    return *this;
  }
  



  /////////////////////////////////////////////////



  
  vector<pair<double,double> >
  QuadErrComb::combine_errs(const ErrorSet::const_iterator& begin,
                            const ErrorSet::const_iterator& end) {
    vector<pair<double,double> > rtn;
    return rtn;
    // const size_t numDims = begin->numDims();
    // double up(0), down(0);
    // for (ErrorSet::const_iterator e = begin; e != end; ++e) {
    //   down += sq(e->minusErr());
    //   up += sq(e->plusErr());
    // }
    // return make_pair(sqrt(down), sqrt(up));
  }
  

  vector<pair<double,double> >
  LinErrComb::combine_errs(const ErrorSet::const_iterator& begin,
                           const ErrorSet::const_iterator& end) {
    vector<pair<double,double> > rtn;
    return rtn;
    // const size_t numDims = begin->numDims();
    // double up(0), down(0);
    // for (ErrorSet::const_iterator e = begin; e != end; ++e) {
    //   down += e->minusErr();
    //   up += e->plusErr();
    // }
    // return make_pair(down, up);
  }
  

}
