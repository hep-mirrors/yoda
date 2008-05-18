// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Bin.h"

#include <cassert>
#include <cmath>
using namespace std;

namespace YODA {


  Bin::Bin(double lowedge, double highedge) 
    : _edges( make_pair(lowedge, highedge) )
  {
    assert( _edges.second >= _edges.first );
  }


  Bin::Bin(std::pair<double, double> edges) 
    : _edges( edges )
  {
    assert( _edges.second >= _edges.first );
  }


  void Bin::reset () {
    _numEntries = 0;
    _sumWeight = 0.0;
    _sumWeight2 = 0.0;
    _sumXWeight = 0.0;
    _sumX2Weight = 0.0;
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
    /// @todo Is this appropriate?
    if (_sumWeight != 0) {
      /// @todo What if sum(weight) is negative... use fabs()?
      return _sumXWeight / _sumWeight;
    } else {
      return midpoint();
    }
  }


  double Bin::midpoint() const {
    return ( _edges.second + _edges.first ) / 2;
  }


  double Bin::xVariance() const {
    const double num = sumX2Weight()*sumWeight() - sumXWeight()*sumXWeight();
    const double denom = sumWeight()*sumWeight() - sumWeight2();    
    return num/denom;
  }


  double Bin::xStdDev() const {
    return sqrt(xVariance());
  }


  double Bin::xStdError() const {
    /// @todo Correct to proper weighted defn with &lt;w&gt; rather than "1"
    return sqrt(xVariance() / (sumWeight() - 1));
  }


  double Bin::sumWeight() const {
    return _sumWeight;
  }


  double Bin::sumWeight2() const {
    return _sumWeight2;
  }


  double Bin::sumXWeight() const {
    return _sumXWeight;
  }


  double Bin::sumX2Weight() const {
    return _sumX2Weight;
  }


}
