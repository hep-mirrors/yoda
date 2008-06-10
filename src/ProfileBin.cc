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
    assert( _edges.first < _edges.second );
    assert( x >= _edges.first && x < _edges.second );
    _xdbn.fill(x, w);
    _ydbn.fill(y, w);
  }

  
  void ProfileBin::reset() {
    Bin::reset();
    _ydbn.reset();
  }
  
  
  double ProfileBin::meanY() const {
    return _ydbn.mean();
  }
  
  
  double ProfileBin::yStdDev() const {
    return _ydbn.stdDev();
  }
  
  
  double ProfileBin::yVariance() const {
    return _ydbn.variance();
  }
  
  
  double ProfileBin::yStdErr() const {
    return _ydbn.stdErr();
  }
    
  
  double ProfileBin::sumYWeight() const {
    return _ydbn.sumWX();
  }
  
  
  double ProfileBin::sumY2Weight() const {
    return _ydbn.sumWX2;
  }
  

  ProfileBin& ProfileBin::add(const ProfileBin& pb) {
    Bin::add(pb);
    _ydbn += pb._ydbn;
    return *this;
  }


  ProfileBin& ProfileBin::subtract(const ProfileBin& pb) {
    Bin::subtract(pb);
    _ydbn -= pb._ydbn;
    return *this;
  }


  ProfileBin& ProfileBin::operator += (const ProfileBin& toAdd) {
    return add(toAdd);
  }
  
  
  ProfileBin& ProfileBin::operator -= (const ProfileBin&) {
    return subtract(toSubtract);
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
