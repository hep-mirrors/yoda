// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Profile1D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  typedef vector<ProfileBin1D> Bins;


  void Profile1D::fill(double x, double y, double weight) {
    /// @todo Fill the underflow and overflow nicely
    ProfileBin1D& b = binByCoord(x);
    b.fill(x, y, weight);
  }


  double Profile1D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw += b->sumW();
    }
    return sumw;
  }


  double Profile1D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw2 += b->sumW2();
    }
    return sumw2;
  }


}
