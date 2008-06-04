// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Dbn1D.h"
#include <cmath>

namespace YODA {
  
  
  void fill(double val, double weight) {
    _numFills += 1;
    _sumW += weight;
    _sumW2 += weight*weight;
    _sumWX += weight*val;
    _sumWX2 += weight*val*val;
  }
  
  
  void reset() {
    _numFills = 0;
    _sumW = 0;
    _sumW2 = 0;
    _sumWX = 0;
    _sumWX2 = 0;
  }
  
  
  double numEntries() const {
    return _numFills;
  }
  
  
  double mean() const {
    /// @todo Handle zero/negative sum weight
    /// @todo What if sum(weight) is negative... use fabs()?
    return _sumWX/_sumW;
  }
  
  
  double variance() const {
    // Weighted variance defined as
    // sig2 = ( sum(wx**2) * sum(w) - sum(wx)**2 ) / ( sum(w)**2 - sum(w**2) )
    // see http://en.wikipedia.org/wiki/Weighted_mean
    const double num = _sumWX2*_sumW - _sumWX*_sumWX;
    const double den = _sumW*_sumW - _sumW2;
    const double var = num/den;
    return var;
  }
  
  
  double stdDev() const {
    return std::sqrt(variance());
  }
  
  
  double stdErr() const {
    /// @todo Handle zero/negative sum weight
    return std::sqrt(variance() / _sumW);
  }
  

}

#endif