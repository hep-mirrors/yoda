// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Profile1D.h"
#include "YODA/Histo1D.h"
#include "YODA/Scatter2D.h"

#include <cmath>
using namespace std;

namespace YODA {


  void Profile1D::fill(double x, double y, double weight) {
    if ( std::isnan(x) ) throw RangeError("X is NaN");
    if ( std::isnan(y) ) throw RangeError("Y is NaN");

    // Fill the overall distribution
    _axis.totalDbn().fill(x, y, weight);

    // Fill the bins and overflows
    /// Unify this with Histo1D's version, when binning and inheritance are reworked
    if (inRange(x, _axis.xMin(), _axis.xMax())) {
      try {
        /// @todo Replace try block with a check that there is a bin at x
        binAt(x).fill(x, y, weight);
      } catch (const RangeError& re) {    }
    } else if (x < _axis.xMin()) {
      _axis.underflow().fill(x, y, weight);
    } else if (x >= _axis.xMax()) {
      _axis.overflow().fill(x, y, weight);
    }

    // Lock the axis now that a fill has happened
    _axis._setLock(true);
  }


  void Profile1D::fillBin(size_t i, double y, double weight) {
    fill(bin(i).xMid(), y, weight);
  }


  double Profile1D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    BOOST_FOREACH (const ProfileBin1D& b, bins()) {
      sumw += b.sumW();
    }
    return sumw;
  }


  double Profile1D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    BOOST_FOREACH (const ProfileBin1D& b, bins()) {
      sumw2 += b.sumW2();
    }
    return sumw2;
  }


  ////////////////////////////////////////


  /// Copy constructor with optional new path
  Profile1D::Profile1D(const Profile1D& p, const std::string& path)
    : AnalysisObject("Profile1D", (path.size() == 0) ? p.path() : path, p, p.title())
  {
    _axis = p._axis;
  }


  /// Constructor from a Scatter2D's binning, with optional new path
  Profile1D::Profile1D(const Scatter2D& s, const std::string& path)
    : AnalysisObject("Profile1D", (path.size() == 0) ? s.path() : path, s, s.title())
  {
    std::vector<ProfileBin1D> bins;
    BOOST_FOREACH (const Scatter2D::Point& p, s.points()) {
      bins.push_back(ProfileBin1D(p.xMin(), p.xMax()));
    }
    _axis = Profile1DAxis(bins);
  }


  /// Constructor from a Histo1D's binning, with optional new path
  Profile1D::Profile1D(const Histo1D& h, const std::string& path)
    : AnalysisObject("Profile1D", (path.size() == 0) ? h.path() : path, h, h.title())
  {
    Bins bins;
    BOOST_FOREACH (const Histo1D::Bin& b, h.bins()) {
      bins.push_back(ProfileBin1D(b.xMin(), b.xMax()));
    }
    _axis = Profile1DAxis(bins);

  }


  ////////////////////////////////////////


  /// Divide two profile histograms
  Scatter2D divide(const Profile1D& numer, const Profile1D& denom) {
    return divide(mkScatter(numer), mkScatter(denom));
  }


  /// @todo Add asymm for profile histos?


}
