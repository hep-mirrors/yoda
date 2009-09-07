// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ProfileBin.h"
#include <cassert>

namespace YODA {

 
  ProfileBin::ProfileBin(double lowedge, double highedge, BinType type)
    : Bin(lowedge, highedge, type)
  { }


  ProfileBin::ProfileBin(std::pair<double,double> edges, BinType type)
    : Bin(edges, type)
  { }

    
  void ProfileBin::fill(double x, double d, double w) {
    assert( _edges.first < _edges.second );
    assert( x >= _edges.first && x < _edges.second );
    _xdbn.fill(x, w);
    _ddbn.fill(d, w);
  }


  void ProfileBin::fillBin(double d, double w) {
    _xdbn.fill(midpoint(), w);
    _ddbn.fill(d, w);
  }

  
  void ProfileBin::reset() {
    Bin::reset();
    _ddbn.reset();
  }
  
  
  double ProfileBin::mean() const {
    return _ddbn.mean();
  }
  
  
  double ProfileBin::stdDev() const {
    return _ddbn.stdDev();
  }
  
  
  double ProfileBin::variance() const {
    return _ddbn.variance();
  }
  
  
  double ProfileBin::stdErr() const {
    return _ddbn.stdErr();
  }
    
  
  double ProfileBin::sumWD() const {
    return _ddbn.sumWX();
  }
  
  
  double ProfileBin::sumWD2() const {
    return _ddbn.sumWX2();
  }
  

  ProfileBin& ProfileBin::add(const ProfileBin& pb) {
    Bin::add(pb);
    _ddbn += pb._ddbn;
    return *this;
  }


  ProfileBin& ProfileBin::subtract(const ProfileBin& pb) {
    Bin::subtract(pb);
    _ddbn -= pb._ddbn;
    return *this;
  }


  ProfileBin& ProfileBin::operator += (const ProfileBin& toAdd) {
    return add(toAdd);
  }
  
  
  ProfileBin& ProfileBin::operator -= (const ProfileBin& toSubtract) {
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
