// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Dbn1D.h"

namespace YODA {


  void Dbn1D::fill(double val, double weight) {
    _numFills += 1;
    _sumW += weight;
    double w2 = weight*weight;
    /// @todo Correct?
    if (weight < 0) w2 *= -1;
    _sumW2 += w2;
    _sumWX += weight*val;
    _sumWX2 += weight*val*val;
  }


  double Dbn1D::variance() const {
    // Weighted variance defined as
    // sig2 = ( sum(wx**2) * sum(w) - sum(wx)**2 ) / ( sum(w)**2 - sum(w**2) )
    // see http://en.wikipedia.org/wiki/Weighted_mean
    if (effNumEntries() == 0.0) {
      throw LowStatsError("Requested width of a distribution with no net fill weights");
    } else if (effNumEntries() <= 1.0) {
      throw LowStatsError("Requested width of a distribution with only one effective entry");
    }
    const double num = _sumWX2*_sumW - _sumWX*_sumWX;
    const double den = _sumW*_sumW - _sumW2;
    if (den == 0) {
      throw WeightError("Undefined weighted variance");
    }
    /// @todo Isn't this sensitive to the overall scale of the weights?
    if (fabs(num) < 1E-10 && fabs(den) < 1E-10) {
      throw WeightError("Numerically unstable weights in width calculation");
    }
    const double var = num/den;
    return var;
  }


  double Dbn1D::stdErr() const {
    // Handle zero/negative sum weight
    if (effNumEntries() == 0) {
      throw LowStatsError("Requested std error of a distribution with no net fill weights");
    }
    /// @todo Unbiased should check that Neff > 1 and divide by N-1?
    return std::sqrt(variance() / effNumEntries());
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


}
