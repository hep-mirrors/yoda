// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2014 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Profile2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"

using namespace std;

namespace YODA {


  void Profile2D::fill(double x, double y, double z, double weight) {
    if ( std::isnan(x) ) throw RangeError("X is NaN");
    if ( std::isinf(x) ) throw RangeError("X is Inf");
    if ( std::isnan(y) ) throw RangeError("Y is NaN");
    if ( std::isinf(y) ) throw RangeError("Y is Inf");
    if ( std::isnan(z) ) throw RangeError("Z is NaN");
    if ( std::isinf(z) ) throw RangeError("Z is Inf");
    // Fill the overall distribution
    _axis.totalDbn().fill(x, y, z, weight);
    // Fill the bins and overflows
    try {
      ProfileBin2D& b = binAt(x, y);
      b.fill(x, y, z, weight);
    } catch (const RangeError& re) {
      size_t ix(0), iy(0);
      if (x <  _axis.xMin()) ix = -1; else if (x >= _axis.xMax()) ix = 1;
      if (y <  _axis.yMin()) iy = -1; else if (y >= _axis.yMax()) iy = 1;
      _axis.outflow(ix, iy).fill(x, y, z, weight);
    }
    // Lock the axis now that a fill has happened
    _axis._setLock(true);
  }


  void Profile2D::fillBin(size_t i, double z, double weight) {
    pair<double, double> mid = bin(i).midpoint();
    fill(mid.first, mid.second, z, weight);
  }


  double Profile2D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw = 0;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) sumw += b.sumW();
    return sumw;
  }


  double Profile2D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw = 0;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) sumw += b.sumW();
    return sumw;
  }


  double Profile2D::xMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xMean();
    double sumwx = 0;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) sumwx += b.sumWX();
    return sumwx/sumW();
  }


  double Profile2D::yMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yMean();
    double sumwy = 0;
    BOOST_FOREACH (const ProfileBin2D& b, bins()) sumwy += b.sumWY();
    return sumwy/sumW();
  }


  double Profile2D::xVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xVariance();
    /// @todo Improve this, by adding the Dbn2Ds and returning the resulting xVariance
    double sigma2 = 0;
    const double xMean = this->xMean();
    for (size_t i = 0; i < bins().size(); ++i) {
      const double diff = bin(i).focus().first - xMean;
      sigma2 += diff * diff * bin(i).sumW();
    }
    return sigma2/sumW();
  }


  double Profile2D::yVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yVariance();
    /// @todo Improve this, by adding the Dbn2Ds and returning the resulting yVariance
    double sigma2 = 0;
    const double yMean = this->yMean();
    for (size_t i = 0; i < bins().size(); ++i) {
      const double diff = bin(i).focus().first - yMean;
      sigma2 += diff * diff * bin(i).sumW();
    }
    return sigma2/sumW();
  }


  double Profile2D::xStdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xStdErr();
    const double effNumEntries = sumW(false)*sumW(false)/sumW2(false);
    return std::sqrt(xVariance(false) / effNumEntries);
  }


  double Profile2D::yStdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yStdErr();
    const double effNumEntries = sumW(false)*sumW(false)/sumW2(false);
    return std::sqrt(yVariance(false) / effNumEntries);
  }


  // double Profile2D::xRMS(bool includeoverflows) const {
  //   if (includeoverflows) return _axis.totalDbn().xRMS();
  //   /// @todo Finish
  // }


  // double Profile2D::yRMS(bool includeoverflows) const {
  //   if (includeoverflows) return _axis.totalDbn().yRMS();
  //   /// @todo Finish
  // }



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

}
