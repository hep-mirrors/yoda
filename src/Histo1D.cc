// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  typedef vector<HistoBin1D> Bins;


  void Histo1D::fill(double x, double weight) {
    // Fill the underflow and overflow nicely
    _axis.totalDbn().fill(x, weight);
    if (x < _axis.lowEdge()) { _axis.underflow().fill(x, weight); return; }
    if (x > _axis.highEdge()) { _axis.overflow().fill(x, weight); return; }
    // Fill the normal bins
    HistoBin1D& b = binByCoord(x);
    b.fill(x, weight);
  }


  double Histo1D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    foreach (const Bin& b, bins()) {
      sumw += b.sumW();
    }
    return sumw;
  }


  double Histo1D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    foreach (const Bin& b, bins()) {
      sumw2 += b.sumW2();
    }
    return sumw2;
  }


  double Histo1D::mean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().mean();
    double sumwx = 0;
    double sumw  = 0;
    for (size_t i = 0; i < bins().size(); i++) {
      sumwx += bins().at(i).sumWX();
      sumw  += bins().at(i).sumW();
    }
    return sumwx/sumw;
  }


  double Histo1D::variance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().variance();
    double sigma2 = 0;
    const double mean = this->mean();
    foreach (const Bin1D& b, bins()) {
      const double diff = b.focus() - mean;
      sigma2 += diff * diff * b.sumW();
    }
    return sigma2/sumW();
  }


}
