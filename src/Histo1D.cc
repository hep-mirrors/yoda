// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  // typedef vector<HistoBin1D> Bins;


  void Histo1D::fill(double x, double weight) {
    // Fill the underflow and overflow nicely
    _axis.totalDbn().fill(x, weight);
    if (x < _axis.lowEdge()) { _axis.underflow().fill(x, weight); return; }
    if (x >= _axis.highEdge()) { _axis.overflow().fill(x, weight); return; }
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
    foreach (const HistoBin1D& b, bins()) {
      const double diff = b.focus() - mean;
      sigma2 += diff * diff * b.sumW();
    }
    return sigma2/sumW();
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
  Scatter2D divide(const Histo1D& numer, const Histo1D& denom,
                   ErrorCombination erropt) {
    //if (!numer._axis.compatible(denom._axis)) ...

    Scatter2D tmp;
    for (size_t i = 0; i < numer.numBins(); ++i) {
      const HistoBin1D& b1 = numer.bin(i);
      const HistoBin1D& b2 = denom.bin(i);
      const HistoBin1D bL = b1 + b2;

      /// @todo Can't we do this check with a method on Axis1D?
      if (!fuzzyEquals(b1.midpoint(), b2.midpoint())) {
        throw BinningError("Axis binnings are not equivalent");
      }

      const double x = bL.focus();
      /// @todo Use exceptions instead, since this is a user-inducible error
      assert(fuzzyEquals(b1.xMin(), b2.xMin()));
      assert(fuzzyEquals(b1.xMax(), b2.xMax()));
      const double exminus = x - b1.xMin();
      const double explus = b1.xMax() - x;
      /// @todo Use exceptions instead, since this is a user-inducible error
      assert(exminus >= 0);
      assert(explus >= 0);
      //
      const double y = b1.height() / b2.height();
      double ey = -1;

      switch (erropt) {
      case QUAD:
        ey = y * sqrt( sqr(b1.heightErr()/b1.height()) + sqr(b2.heightErr()/b2.height()) );
        break;
      case BINOMIAL:
        /// @todo Check that this is correct -- isn't it a problem that this varies if the same scale
        /// factor is applied to the weights on bins 1 and 2?
        /// @todo I think this is only valid if the fills of b1 are a subset of the fills of b2. Throw an
        /// error if Neff(b1) > Neff(b2)
        ey = sqrt( b1.effNumEntries() * (1 - b1.effNumEntries()/b2.effNumEntries()) ) / b2.effNumEntries();
        break;
      }

      tmp.addPoint(x, exminus, explus, y, ey, ey);

    }
    assert(tmp.numPoints() == numer.numBins());
    return tmp;
  }


}
