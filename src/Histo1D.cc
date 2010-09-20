// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  typedef vector<HistoBin> Bins;


  void Histo1D::fill(double x, double weight) {
    /// @todo Fill the underflow and overflow nicely
    HistoBin& b = binByCoord(x);
    b.fill(x, weight);
  }


  double Histo1D::sumW() const {
    double sumw = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw += b->sumW();
    }
    return sumw;
  }


  double Histo1D::mean() const {
    double sumwx = 0;
    double sumw  = 0;
    for (size_t i = 0; i < bins().size(); i++) {
      sumwx += bins().at(i).sumWX();
      sumw  += bins().at(i).sumW();
    }
    return sumwx/sumw;
  }


  double Histo1D::variance() const {
    double sigma2 = 0;
    const double mean = this->mean();
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      const double diff = b->focus() - mean;
      sigma2 += diff * diff * b->sumW();
    }
    return sigma2/sumW();
  }


}
