// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2013 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  void Histo1D::fill(double x, double weight) {
    if ( isnan(x) ) throw RangeError("X is NaN");
    if ( isinf(x) ) throw RangeError("X is Inf");
    // Fill the overall distribution
    _axis.totalDbn().fill(x, weight);
    // Fill the bins and overflows
    try {
      HistoBin1D& b = binByCoord(x);
      b.fill(x, weight);
    } catch (const RangeError& re) {
      if      (x <  _axis.xMin()) _axis.underflow().fill(x, weight);
      else if (x >= _axis.xMax()) _axis.overflow().fill(x, weight);
    }
    // Lock the axis now that a fill has happened
    _axis._setLock(true);
  }


  /// @todo Improve/centralise this statistical aggregation by exposing the Dbn1D/2D objects
  /// in the bins and using their native += operators to do the aggregation.


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
    foreach (const Bin& b, bins()) {
      sumwx += b.sumWX();
      sumw  += b.sumW();
    }
    if ( sumw == 0 ) {
      throw LowStatsError("Requested mean of a Histo1D with no net fill weights");
    }
    return sumwx/sumw;
  }


  double Histo1D::variance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().variance();
    double sigma2 = 0;
    const double mean = this->mean();
    foreach (const HistoBin1D& b, bins()) {
      const double diff = b.focus() - mean;
      sigma2 += diff * diff * b.sumW();
    }
    return sigma2/sumW();
  }


  double Histo1D::stdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().stdErr();
    const double effNumEntries = sumW(false)*sumW(false)/sumW2(false);
    return std::sqrt(variance(false) / effNumEntries);
  }


  ////////////////////////////////////////


  /// Copy constructor with optional new path
  Histo1D::Histo1D(const Histo1D& h, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? h.path() : path, h, h.title())
  {
    _axis = h._axis;
  }


  /// Constructor from a Scatter2D's binning, with optional new path
  Histo1D::Histo1D(const Scatter2D& s, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? s.path() : path, s, s.title())
  {
    std::vector<HistoBin1D> bins;
    foreach (const Scatter2D::Point& p, s.points()) {
      bins.push_back(HistoBin1D(p.xMin(), p.xMax()));
    }
    _axis = Histo1DAxis(bins);
  }


  /// Constructor from a Profile1D's binning, with optional new path
  Histo1D::Histo1D(const Profile1D& p, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? p.path() : path, p, p.title())
  {
    std::vector<HistoBin1D> bins;
    foreach (const ProfileBin1D& b, p.bins()) {
      bins.push_back(HistoBin1D(b.xMin(), b.xMax()));
    }
    _axis = Histo1DAxis(bins);
  }


  ////////////////////////////////////////


  // Divide two histograms
  Scatter2D divide(const Histo1D& numer, const Histo1D& denom) {
    return divide(mkScatter(numer), mkScatter(denom));
  }


  // Calculate a histogrammed efficiency ratio of two histograms
  Scatter2D efficiency(const Histo1D& accepted, const Histo1D& total) {
    Scatter2D tmp = divide(accepted, total);
    for (size_t i = 0; i < accepted.numBins(); ++i) {
      const HistoBin1D& b_acc = accepted.bin(i);
      const HistoBin1D& b_tot = total.bin(i);
      Point2D& point = tmp.point(i);

      // Check that the numerator is a subset of the denominator
      if (b_acc.effNumEntries() > b_tot.effNumEntries() || b_acc.sumW() > b_tot.sumW())
        throw UserError("Attempt to calculate an efficiency when the numerator is not a subset of the denominator");

      // If no entries on the denominator, set eff = 0 and move to the next bin
      /// @todo Or mark/remove the invalid point?
      if (b_tot.effNumEntries() == 0) {
        point.setY(0.0);
        point.setYErr(0.0, 0.0);
        continue;
      }

      // Calculate the values and errors
      // const double eff = b_acc.effNumEntries() / b_tot.effNumEntries();
      // const double ey = sqrt( b_acc.effNumEntries() * (1 - b_acc.effNumEntries()/b_tot.effNumEntries()) ) / b_tot.effNumEntries();
      const double eff = b_acc.sumW() / b_tot.sumW(); //< Actually this is already calculated by the division...
      const double ey = sqrt(abs( ((1-2*eff)*sqr(b_acc.areaErr()) + sqr(eff)*sqr(b_tot.areaErr())) / sqr(b_tot.sumW()) ));
      // assert(point.y() == eff); //< @todo Correct? So we don't need to reset the eff on the next line?
      point.setY(eff, ey);
    }
    return tmp;
  }


  // Convert a Histo1D to a Scatter2D representing the integral of the histogram
  Scatter2D toIntegralHisto(const Histo1D& h, bool includeunderflow) {
    /// @todo Check that the histogram binning has no gaps, otherwise throw a BinningError
    Scatter2D tmp = mkScatter(h);
    double integral = includeunderflow ? h.underflow().sumW() : 0.0;
    for (size_t i = 0; i < h.numBins(); ++i) {
      Point2D& point = tmp.point(i);
      integral += h.bin(i).sumW();
      const double err = sqrt(integral);
      point.setY(integral);
      point.setYErr(err, err);
    }
    return tmp;
  }


}
