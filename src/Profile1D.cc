// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Profile1D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {
  
  
  /// Constructor giving range and number of bins
  Profile1D::Profile1D(const std::string& path, const std::string& title,
                       size_t nxbins,
                       double xlower, double xupper) :
    AnalysisObject( path, title ),
    _axis(nxbins, xlower, xupper),
    _bins(),
    _underflow( ProfileBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( ProfileBin(0,1,Bin::OVERFLOWBIN) )
  {
    for (size_t i = 0; i < _axis.numBins(); ++i) {
      const pair<double, double> edges = _axis.binEdges(i);
      _bins.push_back( ProfileBin(edges.first, edges.second) );
    }
  }
  
  
  /// Constructor giving explicit bin edges
  /// For n bins, binedges.size() == n+1, the last
  /// one being the upper bound of the last bin
  Profile1D::Profile1D(const std::string& path, const std::string& title,
                       const std::vector<double>& xbinedges) :
    AnalysisObject( path, title ),
    _axis(xbinedges),
    _bins(),
    _underflow( ProfileBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( ProfileBin(0,1,Bin::OVERFLOWBIN) )
  {
    for (size_t i = 0; i < _axis.numBins(); ++i) {
      const pair<double, double> edges = _axis.binEdges(i);
      _bins.push_back( ProfileBin(edges.first, edges.second) );
    }
  }


  /// Constructor giving a vector of bins
  Profile1D::Profile1D(std::string path, std::string title,
                       const vector<ProfileBin>& xbins) :
    AnalysisObject( path, title ),
    _axis(1, 0.0, 1.0),
    _bins(xbins),
    _underflow( ProfileBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( ProfileBin(0,1,Bin::OVERFLOWBIN) )
  {
    vector<double> binedges;
    for (size_t i = 0; i < _bins.size(); ++i) {
      binedges.push_back(_bins[i].lowEdge());
    }
    binedges.push_back(_bins.back().highEdge());
    _axis = Axis(binedges);
  }


  void Profile1D::reset () {
    _underflow.reset();
    _overflow.reset();
    for (Bins::iterator b = _bins.begin(); b != _bins.end(); ++b) {
      b->reset();
    }
  }


  void Profile1D::fill(double x, double y, double weight) {
    ProfileBin& b = binByCoord(x);
    if ( b.type() == Bin::VALIDBIN ) {
      b.fill(x, y, weight);
    } else {
      b.fillBin(y, weight);
    }
  }


  void Profile1D::fillBin(size_t index, double y, double weight) {
    if (index >= bins().size())
      throw RangeError("YODA::Profile: index out of range");
    ProfileBin& pb = bins().at(index);
    pb.fill(pb.midpoint(), y, weight);
  }


  vector<ProfileBin>& Profile1D::bins() {
    return _bins;
  }


  const vector<ProfileBin>& Profile1D::bins() const {
    return _bins;
  }


  ProfileBin& Profile1D::bin(size_t index) {
    if (index >= bins().size())
      throw RangeError("YODA::Profile: index out of range");
    return _bins[index];
  }


  const ProfileBin& Profile1D::bin(size_t index) const {
    if (index >= bins().size())
      throw RangeError("YODA::Profile: index out of range");
    return _bins[index];
  }


  ProfileBin& Profile1D::bin(Bin::BinType binType) {
    if (binType == Bin::UNDERFLOWBIN) return _underflow;
    if (binType == Bin::OVERFLOWBIN) return _overflow;
    throw RangeError("YODA::Profile: index out of range");
    // Just to fix a warning:
    return _underflow;
  }


  const ProfileBin& Profile1D::bin(Bin::BinType binType) const {
    if (binType == Bin::UNDERFLOWBIN) return _underflow;
    if (binType == Bin::OVERFLOWBIN) return _overflow;
    throw RangeError("YODA::Profile: index out of range");
    // Just to fix a warning:
    return _underflow;
  }


  ProfileBin& Profile1D::binByCoord(double x) {
    pair<Bin::BinType, size_t> index = _axis.findBinIndex(x);
    if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
    return bin(index.first);
  }


  const ProfileBin& Profile1D::binByCoord(double x) const {
    pair<Bin::BinType, size_t> index = _axis.findBinIndex(x);
    if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
    return bin(index.first);
  }


  double Profile1D::sumWeight() const {
    double sumw = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw += b->sumW();
    }
    return sumw;
  }


  double Profile1D::sumWeight2() const {
    double sumw2 = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw2 += b->sumW2();
    }
    return sumw2;
  }


  Profile1D& Profile1D::operator += (const Profile1D& toAdd) {
    if (_axis != toAdd._axis) {
      throw LogicError("YODA::Profile1D: Cannot add histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      _bins[i] += toAdd._bins[i];
    }
    _underflow += toAdd._underflow;
    _overflow += toAdd._overflow;
    return *this;
  }


  Profile1D& Profile1D::operator -= (const Profile1D& toSubtract) {
    if (_axis != toSubtract._axis) {
      throw LogicError("YODA::Profile1D: Cannot subtract histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      _bins[i] += toSubtract._bins[i];
    }
    _underflow += toSubtract._underflow;
    _overflow += toSubtract._overflow;
    return *this;
  }


  Profile1D operator + (const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp += second;
    return tmp;
  }


  Profile1D operator - (const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp -= second;
    return tmp;
  }


}
