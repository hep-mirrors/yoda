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
  
  
  typedef vector<ProfileBin> Bins;


  Profile1D::Profile1D(size_t nxbins, double xlower, double xupper, Binning binning) :
    AnalysisObject(),
    _axis(nxbins, xlower, xupper binning)
  {  }


  Profile1D::Profile1D(const std::string& path, const std::string& title,
                       size_t nxbins, double xlower, double xupper, Binning binning) :
    AnalysisObject( path, title ),
    _axis(nxbins, xlower, xupper, binning)
  {  }
  
  
  Profile1D::Profile1D(const std::vector<double>& xbinedges) :
    AnalysisObject(),
    _axis(xbinedges),
  {  }


  Profile1D::Profile1D(const std::string& path, const std::string& title,
                       const std::vector<double>& xbinedges) :
    AnalysisObject( path, title ),
    _axis(xbinedges),
  {  }


  Profile1D::Profile1D(const std::vector<ProfileBin>& xbins) :
    AnalysisObject(),
    _axis(xbins)
  {  }


  Profile1D::Profile1D(std::string path, std::string title,
                       const vector<ProfileBin>& xbins) :
    AnalysisObject( path, title ),
    _axis(xbins)
  {  }


  void Profile1D::reset () {
    _axis.reset();
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
    return _axis.bins();
  }


  const vector<ProfileBin>& Profile1D::bins() const {
    return _axis.bins();
  }


  ProfileBin& Profile1D::bin(size_t index) {
    return _axis.bin(index);
  }


  const ProfileBin& Profile1D::bin(size_t index) const {
    return _axis.bin(index);
  }


  ProfileBin& Profile1D::bin(Bin::BinType binType) {
    return _axis.bin(binType);
  }


  const ProfileBin& Profile1D::bin(Bin::BinType binType) const {
    return _axis.bin(binType);
  }


  ProfileBin& Profile1D::binByCoord(double x) {
    return _axis.binByCoord(x);
  }


  const ProfileBin& Profile1D::binByCoord(double x) const {
    return _axis.binByCoord(x);
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
    /// @todo Use Axis<>::operator+= instead

    if (_axis != toAdd._axis) {
      throw LogicError("YODA::Profile1D: Cannot add histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      bins().at(i) += toAdd.bins().at(i);
    }
    _axis.bin(UNDERFLOW) += toAdd._axis.bin(UNDERFLOW);
    _axis.bin(OVERFLOW)  += toAdd._axis.bin(OVERFLOW);
    return *this;
  }


  Profile1D& Profile1D::operator -= (const Profile1D& toSubtract) {
    /// @todo Use Axis<>::operator+= instead
    if (_axis != toSubtract._axis) {
      throw LogicError("YODA::Profile1D: Cannot subtract histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      bins().at(i) += toSubtract.bins().at(i);
    }
    _axis.bin(UNDERFLOW) += toSubtract._axis.bin(UNDERFLOW);
    _axis.bin(OVERFLOW)  += toSubtract._axis.bin(OVERFLOW);
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
