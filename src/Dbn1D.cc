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
    return _sumWX/_sumW;
  }
  
  
  double variance() const {
    
  }
  
  
  double stdDev() const {
    return std::sqrt(variance));
  }
  
  
  double stdErr() const {
    
  }
  

}

#endif
