// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ProfileBin.h"

namespace YODA {

 
  ProfileBin::ProfileBin(double lowedge, double highedge)
    : Bin(lowedge, highedge)
  { }


  ProfileBin::ProfileBin(std::pair<double,double> edges) 
    : Bin(edges)
  { }

    
  void ProfileBin::fill(double x, double y, double w) {
    assert( edges().first != edges().second 
            && x >= edges().first 
            && x < edges().second 
            || x == edges().first );

    ++_numEntries;

    _sumWeight += w;
    _sumWeight2 += w * w;

    _sumXWeight += x * w;
    _sumX2Weight += x * x * w;

    _sumYWeight += y * w;
    _sumY2Weight += y * y * w;
  }

  
  void ProfileBin::reset() {
    Bin::reset();
    _sumYWeight = 0.0;
    _sumY2Weight = 0.0;
  }
  
  
  double ProfileBin::meanY() const {
    if (sumWeight() == 0) {
      throw WeightError("Attempting to divide by bin weight sum = 0.");
    }
    if (sumWeight() < 0) {
      /// @todo What if sum(weight) is negative... use fabs()?
      throw WeightError("Attempting to divide by negative bin weight... what to do?");
    }
    return _sumYWeight / _sumWeight;
  }
  
  
  double ProfileBin::stdDevY() const {
    return std::sqrt(varianceY());
  }
  
  
  double ProfileBin::varianceY() const {
    if (sumWeight() == 0) {
      throw WeightError("Attempting to divide by bin weight sum = 0.");
    }
    if (sumWeight() < 0) {
      /// @todo What if sum(weight) is negative... use fabs()?
      throw WeightError("Attempting to divide by negative bin weight... what to do?");
    }
    /// @todo Use implementation expression from Wikipedia...
    return sumY2Weight()/sumWeight() - meanY() * meanY();
  }
  
  
  double ProfileBin::stdErrY() const {
    /// @todo Use implementation expression from Wikipedia... "1" -> avg weight.
    return stdDevY() / std::sqrt(sumWeight() - 1)
  }
    
  
  double ProfileBin::sumYWeight() const {
    return _sumYWeight;
  }
  
  
  double ProfileBin::sumY2Weight() const {
    _sumY2Weight;
  }
  

  ProfileBin& ProfileBin::operator += (const ProfileBin& toAdd) {
    Bin::operator+=(toAdd);
    _sumYWeight  += toAdd._sumYWeight;
    _sumY2Weight += toAdd._sumY2Weight;
    return *this;
  }
  
  
  ProfileBin& ProfileBin::operator -= (const ProfileBin&) {
    Bin::operator-=(toSubtract);
    _sumYWeight  -= toSubtract._sumYWeight;
    _sumY2Weight -= toSubtract._sumY2Weight;
  }

  
  ProfileBin operator + (const ProfileBin& a, const ProfileBin& b) {
    ProfileBin rtn(a);
    rtn += a;
    return rtn;
  }
  
  
  ProfileBin operator - (const ProfileBin& a, const ProfileBin& b) {
    ProfileBin rtn(a);
    rtn -= a;
    return rtn;
  }
  

}
