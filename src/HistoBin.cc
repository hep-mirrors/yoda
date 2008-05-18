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
    Bin::reset ();
  }


  void HistoBin::fill(double x, double w)
  {
    assert( _edges.first != _edges.second 
        && x >= _edges.first 
        && x < _edges.second 
        || x == _edges.first );

    ++_numEntries;

    _sumWeight += w;
    _sumWeight2 += w * w;

    _sumXWeight += x * w;
    _sumX2Weight += x * x * w;
  }


  double HistoBin::area() const
  {
    return sumWeight();
  }


  double HistoBin::height() const
  {
    return area() / width();
  }


  double HistoBin::areaError() const
  {
    return sqrt( _sumWeight2 );
  }


  double HistoBin::heightError() const
  {
    return areaError() / width();
  }


  Bin& HistoBin::operator += (const Bin& toAdd) {
    assert(_edges == toAdd._edges);
    _numEntries += toAdd._numEntries;
    _sumWeight += toAdd._sumWeight;
    _sumWeight2 += toAdd._sumWeight2;
    _sumXWeight += toAdd._sumXWeight;
    _sumX2Weight += toAdd._sumX2Weight;
    return *this;
  }

  Bin& HistoBin::operator -= (const Bin& toSubtract) {
    assert(_edges == toSubtract._edges);
    _numEntries -= toSubtract._numEntries;
    _sumWeight -= toSubtract._sumWeight;
    _sumWeight2 -= toSubtract._sumWeight2;
    _sumXWeight -= toSubtract._sumXWeight;
    _sumX2Weight -= toSubtract._sumX2Weight;
    return *this;
  }


  Bin operator + (const Bin& a, const Bin& b) {
    ProfileBin rtn(a);
    rtn += a;
    return rtn;
  }


  Bin operator - (const Bin& a, const Bin& b) {
    ProfileBin rtn(a);
    rtn -= a;
    return rtn;
  }


}
