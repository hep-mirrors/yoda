// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/HistoBin.h"

#include <cassert>
#include <cmath>
using namespace std;

namespace YODA {


  HistoBin::HistoBin(double low, double high, BinType type) 
    : Bin(low, high, type)
  { }


  HistoBin::HistoBin(std::pair<double, double> edges, BinType type) 
    : Bin(edges, type) 
  { }


  void HistoBin::reset() {
    Bin::reset();
  }


  void HistoBin::fill(double x, double w) {
    assert( _edges.first < _edges.second );
    assert( x >= _edges.first && x < _edges.second );
    _xdbn.fill(x, w);
  }


  void HistoBin::fillBin(double weight) {
    _xdbn.fill(midpoint(), weight);
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
