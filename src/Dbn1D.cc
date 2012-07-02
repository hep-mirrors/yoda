// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Dbn1D.h"

namespace YODA {


  void Dbn1D::fill(double val, double weight) {
    _numFills += 1;
    _sumW += weight;
    //const double w2 = sign(weight) * weight*weight;
    _sumW2 += weight*weight;
    _sumWX += weight*val;
    _sumWX2 += weight*val*val;
  }


  double Dbn1D::mean() const {
    if (effNumEntries() == 0) {
      throw LowStatsError("Requested mean of a distribution with no net fill weights");
    }
    // This is ok, even for negative sum(w)
    return _sumWX/_sumW;
  }


  double Dbn1D::variance() const {
    // Weighted variance defined as
    // sig2 = ( sum(wx**2) * sum(w) - sum(wx)**2 ) / ( sum(w)**2 - sum(w**2) )
    // see http://en.wikipedia.org/wiki/Weighted_mean
    if (effNumEntries() == 0) {
      throw LowStatsError("Requested width of a distribution with no net fill weights");
    } else if (fuzzyLessEquals(effNumEntries(), 1.0)) {
      throw LowStatsError("Requested width of a distribution with only one effective entry");
    }
    const double num = _sumWX2*_sumW - _sumWX*_sumWX;
    const double den = _sumW*_sumW - _sumW2;
    if (isZero(den)) {
      throw WeightError("Undefined weighted variance");
    }
    /// @todo Isn't this sensitive to the overall scale of the weights?
    /// Shouldn't it check if den is bigger then num by a set number of
    /// orders of magnitude and vice versa?
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


  double Dbn1D::rms() const {
    // Weighted RMS defined as
    // rms = sqrt(sum{w x^2} / sum{w})
    if (effNumEntries() == 0) {
      throw LowStatsError("Requested RMS of a distribution with no net fill weights");
    }
    const double meansq = sumWX2() / sumW();
    return std::sqrt(meansq);
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
    _numFills += d._numFills; //< @todo Hmm, add or subtract?!?
    _sumW     -= d._sumW;
    _sumW2    -= d._sumW2;
    _sumWX    -= d._sumWX;
    _sumWX2   -= d._sumWX2;
    return *this;
  }


}
