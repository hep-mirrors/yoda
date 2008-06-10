// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/HistoBin.h"

#include <cassert>
#include <cmath>
using namespace std;

namespace YODA {


  HistoBin::Bin(double low, double high) 
    : Bin(low, high)
  { }


  HistoBin::Bin(std::pair<double, double> edges) 
    : Bin(edges) 
  { }


  void HistoBin::reset() {
    Bin::reset();
  }


  void HistoBin::fill(double x, double w) {
    assert( _edges.first < _edges.second );
    assert( x >= _edges.first && x < _edges.second );
    _xdbn.fill(x, w);
  }


  double HistoBin::area() const
  {
    return sumW();
  }


  double HistoBin::height() const
  {
    return area() / width();
  }


  double HistoBin::areaError() const
  {
    return sqrt( sumW2() );
  }


  double HistoBin::heightError() const
  {
    return areaError() / width();
  }


  HistoBin& HistoBin::add(const HistoBin& hb) {
    Bin::add(hb);
    return *this;
  }


  HistoBin& HistoBin::subtract(const HistoBin& hb) {
    Bin::subtract(hb);
    return *this;
  }


  HistoBin& HistoBin::operator += (const HistoBin& toAdd) {
    return add(toAdd);
  }


  HistoBin& HistoBin::operator -= (const HistoBin& toSubtract) {
    return subtract(toSubtract);
  }


  HistoBin operator + (const HistoBin& a, const HistoBin& b) {
    HistoBin rtn(a);
    rtn += a;
    return rtn;
  }


  HistoBin operator - (const HistoBin& a, const HistoBin& b) {
    HistoBin rtn(a);
    rtn -= a;
    return rtn;
  }


}
