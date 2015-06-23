// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Profile2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"

using namespace std;

namespace YODA {


  void Profile2D::fill(double x, double y, double z, double weight) {
    if ( std::isnan(x) ) throw RangeError("X is NaN");
    if ( std::isnan(y) ) throw RangeError("Y is NaN");
    if ( std::isnan(z) ) throw RangeError("Z is NaN");

    // Fill the overall distribution
    _axis.totalDbn().fill(x, y, z, weight);

    // Fill the bins and overflows
    /// Unify this with Histo2D's version, when binning and inheritance are reworked
    if (inRange(x, _axis.xMin(), _axis.xMax()) && inRange(y, _axis.yMin(), _axis.yMax())) {
      try {
        /// @todo Replace try block with a check that there is a bin at x, y
        binAt(x, y).fill(x, y, z, weight);
      } catch (const RangeError& re) {    }
    }
    /// @todo Reinstate! With outflow axis bin lookup
    // else {
    //   size_t ix(0), iy(0);
    //   if (x <  _axis.xMin()) ix = -1; else if (x >= _axis.xMax()) ix = 1;
    //   if (y <  _axis.yMin()) iy = -1; else if (y >= _axis.yMax()) iy = 1;
    //   _axis.outflow(ix, iy).fill(x, y, z, weight);
    // }

    // Lock the axis now that a fill has happened
    _axis._setLock(true);
  }


  void Profile2D::fillBin(size_t i, double z, double weight) {
    pair<double, double> mid = bin(i).xyMid();
    fill(mid.first, mid.second, z, weight);
  }


  /////////////// COMMON TO ALL BINNED

  unsigned long Profile2D::numEntries(bool includeoverflows) const {
    if (includeoverflows) return totalDbn().numEntries();
    unsigned long n = 0;
    BOOST_FOREACH (const Bin& b, bins()) n += b.numEntries();
    return n;
  }


  double Profile2D::effNumEntries(bool includeoverflows) const {
    if (includeoverflows) return totalDbn().effNumEntries();
    double n = 0;
    BOOST_FOREACH (const Bin& b, bins()) n += b.effNumEntries();
    return n;
  }


  double Profile2D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw = 0;
    BOOST_FOREACH (const Bin& b, bins()) sumw += b.sumW();
    return sumw;
  }


  double Profile2D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    BOOST_FOREACH (const Bin& b, bins()) sumw2 += b.sumW2();
    return sumw2;
  }

  // ^^^^^^^^^^^


  double Profile2D::xMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xMean();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.xMean();
  }


  double Profile2D::yMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yMean();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.yMean();
  }


  double Profile2D::xVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xVariance();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.xVariance();
  }


  double Profile2D::yVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yVariance();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.yVariance();
  }


  double Profile2D::xStdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xStdErr();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.xStdErr();
  }


  double Profile2D::yStdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yStdErr();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.yStdErr();
  }


  double Profile2D::xRMS(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xRMS();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.xRMS();
  }


  double Profile2D::yRMS(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yRMS();
    Dbn3D dbn;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) dbn += b.dbn();
    return dbn.yRMS();
  }


  /////////////////////////////////////


  /// A copy constructor with optional new path
  Profile2D::Profile2D(const Profile2D& p, const std::string& path)
    : AnalysisObject("Profile2D",
		     (path.size() == 0) ? p.path() : path,
		     p, p.title()),
      _axis(p._axis)
  {  }


  /// Constructor from a Scatter3D's binning, with optional new path
  Profile2D::Profile2D(const Scatter3D& s, const std::string& path)
    : AnalysisObject("Profile2D",
		     (path.size() == 0) ? s.path() : path,
		     s, s.title())
  {
    Bins bins;
    BOOST_FOREACH (const Scatter3D::Point& p, s.points()) {
      bins.push_back(ProfileBin2D(p.xMin(), p.yMin(), p.xMax(), p.yMax()));
    }
    _axis = Profile2DAxis(bins);
  }


  /// Constructor from a Histo2D's binning, with optional new path
  Profile2D::Profile2D(const Histo2D& h, const std::string& path)
    : AnalysisObject("Profile2D", (path.size() == 0) ? h.path() : path, h, h.title())
  {
    Bins bins;
    BOOST_FOREACH (const HistoBin2D& b, h.bins()) {
      bins.push_back(ProfileBin2D(b.xMin(), b.yMin(), b.xMax(), b.yMax()));
    }
    _axis = Profile2DAxis(bins);
  }


  /// Divide two profile histograms
  Scatter3D divide(const Profile2D& numer, const Profile2D& denom) {
    return divide(mkScatter(numer), mkScatter(denom));
  }


  /// @todo Add asymm for profile histos?


}
