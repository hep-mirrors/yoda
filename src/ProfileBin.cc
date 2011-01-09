// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ProfileBin.h"
#include <cassert>

namespace YODA {


  ProfileBin::ProfileBin(double lowedge, double highedge)
    : Bin(lowedge, highedge)
  { }


  ProfileBin::ProfileBin(std::pair<double,double> edges)
    : Bin(edges)
  { }


  void ProfileBin::fill(double x, double d, double w) {
    assert( _edges.first < _edges.second );
    assert( x >= _edges.first && x < _edges.second );
    _xdbn.fill(x, w);
    _ydbn.fill(d, w);
  }


  void ProfileBin::fillBin(double d, double w) {
    _xdbn.fill(midpoint(), w);
    _ydbn.fill(d, w);
  }


  void ProfileBin::reset() {
    Bin::reset();
    _ydbn.reset();
  }


  double ProfileBin::mean() const {
    return _ydbn.mean();
  }


  double ProfileBin::stdDev() const {
    return _ydbn.stdDev();
  }


  double ProfileBin::variance() const {
    return _ydbn.variance();
  }


  double ProfileBin::stdErr() const {
    return _ydbn.stdErr();
  }


  double ProfileBin::sumWY() const {
    return _ydbn.sumWX();
  }


  double ProfileBin::sumWY2() const {
    return _ydbn.sumWX2();
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
