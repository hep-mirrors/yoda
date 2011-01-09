// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Bin.h"

#include <cassert>
#include <cmath>
using namespace std;

namespace YODA {


  Bin::Bin(double lowedge, double highedge)
    : _edges( make_pair(lowedge, highedge) )
  {
    assert( _edges.second > _edges.first );
  }


  Bin::Bin(std::pair<double, double> edges)
    : _edges( edges )
  {
    assert( _edges.second >= _edges.first );
  }


  void Bin::reset () {
    _xdbn.reset();
  }


  double Bin::lowEdge() const {
    return _edges.first;
  }


  double Bin::highEdge() const {
    return _edges.second;
  }


  pair<double,double> Bin::edges() const {
    return _edges;
  }


  double Bin::width() const {
    return _edges.second - _edges.first;
  }


  double Bin::focus() const {
    if (_xdbn.sumW() != 0) {
      return xMean();
    } else {
      return midpoint();
    }
  }


  double Bin::midpoint() const {
    return ( _edges.second + _edges.first ) / 2;
  }


  double Bin::xMean() const {
    return _xdbn.mean();
  }


  double Bin::xVariance() const {
    return _xdbn.variance();
  }


  double Bin::xStdDev() const {
    return _xdbn.stdDev();
  }


  double Bin::xStdError() const {
    return _xdbn.stdErr();
  }


  unsigned long Bin::numEntries() const {
    return _xdbn.numEntries();
  }


  double Bin::sumW() const {
    return _xdbn.sumW();
  }


  double Bin::sumW2() const {
    return _xdbn.sumW2();
  }


  double Bin::sumWX() const {
    return _xdbn.sumWX();
  }


  double Bin::sumWX2() const {
    return _xdbn.sumWX2();
  }


  Bin& Bin::add(const Bin& b) {
    assert(_edges == b._edges);
    _xdbn += b._xdbn;
    return *this;
  }


  Bin& Bin::subtract(const Bin& b) {
    assert(_edges == b._edges);
    _xdbn -= b._xdbn;
    return *this;
  }


  Bin& Bin::operator += (const Bin& b) {
    return add(b);
  }


  Bin& Bin::operator -= (const Bin& b) {
    return subtract(b);
  }


  Bin operator + (const Bin& a, const Bin& b) {
    Bin rtn = a;
    rtn += b;
    return rtn;
  }


  Bin operator - (const Bin& a, const Bin& b) {
    Bin rtn = a;
    rtn -= b;
    return rtn;
  }


}
