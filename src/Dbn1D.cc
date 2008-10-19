// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Dbn1D.h"
#include <cmath>
#include <iostream>

namespace YODA {
  
  
  void Dbn1D::fill(double val, double weight) {
    _numFills += 1;
    _sumW += weight;
    double w2 = weight*weight;
    if (weight < 0) w2 *= -1;
    _sumW2 += w2;
    _sumWX += weight*val;
    _sumWX2 += weight*val*val;
  }
  
  
  void Dbn1D::reset() {
    _numFills = 0;
    _sumW = 0;
    _sumW2 = 0;
    _sumWX = 0;
    _sumWX2 = 0;
  }
  
  
  unsigned long Dbn1D::numEntries() const {
    return _numFills;
  }
  

  double Dbn1D::sumW() const {
    return _sumW;
  }


  double Dbn1D::sumW2() const {
    return _sumW2;
  }
  
  
  double Dbn1D::sumWX() const {
    return _sumWX;
  }
  
  
  double Dbn1D::sumWX2() const {
    return _sumWX2;
  }
  

  double Dbn1D::mean() const {
    /// @todo Handle zero/negative sum weight
    /// @todo What if sum(weight) is negative... use fabs()?
    return _sumWX/_sumW;
  }
    

  double Dbn1D::variance() const {
    // Weighted variance defined as
    // sig2 = ( sum(wx**2) * sum(w) - sum(wx)**2 ) / ( sum(w)**2 - sum(w**2) )
    // see http://en.wikipedia.org/wiki/Weighted_mean
    if (sumW() == 0) {
      throw Exception("Requested width of a distribution with no net fill weights");
    } 
    const double num = _sumWX2*_sumW - _sumWX*_sumWX;
    const double den = _sumW*_sumW - _sumW2;
    //std::cerr << "***" << num << ", " << den << std::endl;
    if (numEntries() < 2 || (fabs(num) < 1E-10 && fabs(den) < 1E-10)) {
      throw Exception("Requested width of a distribution with only one effective entry");
    } 
    const double var = num/den;
    return var;
  }
  
  
  double Dbn1D::stdDev() const {
    return std::sqrt(variance());
  }
  
  
  double Dbn1D::stdErr() const {
    // Handle zero/negative sum weight
    if (sumW() == 0) {
      throw Exception("Requested std error of a distribution with no net fill weights");
    } 
    return std::sqrt(variance() / sumW());
  }




  Dbn1D& Dbn1D::add(const Dbn1D& d) {
    _numFills += d._numFills;
    _sumW     += d._sumW;
    _sumW2    += d._sumW2;
    _sumWX    += d._sumWX;
    _sumWX2   += d._sumWX2;
    return *this;
  }


  Dbn1D& Dbn1D::subtract(const Dbn1D& d) {
    _numFills += d._numFills; //< @todo Hmm, what's best?!?
    _sumW     -= d._sumW;
    _sumW2    -= d._sumW2;
    _sumWX    -= d._sumWX;
    _sumWX2   -= d._sumWX2;
    return *this;
  }


  Dbn1D& Dbn1D::operator += (const Dbn1D& d) {
    return add(d);
  }


  Dbn1D& Dbn1D::operator -= (const Dbn1D& d) {
    return subtract(d);
  }


  Dbn1D operator + (const Dbn1D& a, const Dbn1D& b) {
    Dbn1D rtn = a;
    rtn += b;
    return rtn;
  }


  Dbn1D operator - (const Dbn1D& a, const Dbn1D& b) {
    Dbn1D rtn = a;
    rtn -= b;
    return rtn;
  }


}
