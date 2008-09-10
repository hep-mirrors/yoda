// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  Histo1D::Histo1D(const std::string& path, const std::string& title,
           const vector<double>& binedges) :
    AnalysisObject( path, title ),
    _axis(binedges),
    _bins(),
    _underflow( HistoBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( HistoBin(0,1,Bin::OVERFLOWBIN) )
  {
    for (size_t i = 0; i < _axis.numBins(); ++i) {
      const pair<double, double> edges = _axis.binEdges(i);
      _bins.push_back( HistoBin(edges.first, edges.second) );
    }
  }


  Histo1D::Histo1D(const std::string& path, const std::string& title,
           size_t nbins, double lower, double upper) :
    AnalysisObject( path, title ),
    _axis(nbins, lower, upper),
    _bins(),
    _underflow( HistoBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( HistoBin(0,1,Bin::OVERFLOWBIN) )
  {
    for (size_t i = 0; i < _axis.numBins(); ++i) {
      const pair<double, double> edges = _axis.binEdges(i);
      _bins.push_back( HistoBin(edges.first, edges.second) );
    }
  }


  Histo1D::Histo1D(std::string path, std::string title,
                   const vector<HistoBin>& bins) :
    AnalysisObject( path, title ),
    _axis(1, 0.0, 1.0),
    _bins(bins),
    _underflow( HistoBin(0,1,Bin::UNDERFLOWBIN) ),
    _overflow( HistoBin(0,1,Bin::OVERFLOWBIN) )
  {
    vector<double> binedges;
    for (size_t i = 0; i < _bins.size(); ++i) {
      binedges.push_back(_bins[i].lowEdge());
    }
    binedges.push_back(_bins.back().highEdge());
    _axis = Axis(binedges);
  }


  void Histo1D::reset () {
    _underflow.reset();
    _overflow.reset();
    for (Bins::iterator b = _bins.begin(); b != _bins.end(); ++b) {
      b->reset();
    }
  }


  void Histo1D::fill(double x, double weight) {
    HistoBin& b = binByCoord(x);
    if ( b.type() == Bin::VALIDBIN ) {
      b.fill(x, weight);
    } else {
      b.fillBin(weight);
    }
  }


  void Histo1D::fillBin(size_t index, double weight) {
    if (index >= bins().size())
      throw RangeError("YODA::Histo: index out of range");
    HistoBin& hb = bins().at(index);
    hb.fill(hb.midpoint(), weight);
  }


  vector<HistoBin>& Histo1D::bins() {
    return _bins;
  }


  const vector<HistoBin>& Histo1D::bins() const {
    return _bins;
  }


  HistoBin& Histo1D::bin(size_t index) {
    if (index >= bins().size())
      throw RangeError("YODA::Histo: index out of range");
    return _bins[index];
  }


  const HistoBin& Histo1D::bin(size_t index) const {
    if (index >= bins().size())
      throw RangeError("YODA::Histo: index out of range");
    return _bins[index];
  }


  HistoBin& Histo1D::bin(Bin::BinType binType) {
    if (binType == Bin::UNDERFLOWBIN) return _underflow;
    if (binType == Bin::OVERFLOWBIN) return _overflow;
    throw RangeError("YODA::Histo: index out of range");
    // Just to fix a warning:
    return _underflow;
  }


  const HistoBin& Histo1D::bin(Bin::BinType binType) const {
    if (binType == Bin::UNDERFLOWBIN) return _underflow;
    if (binType == Bin::OVERFLOWBIN) return _overflow;
    throw RangeError("YODA::Histo: index out of range");
    // Just to fix a warning:
    return _underflow;
  }


  HistoBin& Histo1D::binByCoord(double x) {
    pair<Bin::BinType, size_t> index = _axis.findBinIndex(x);
    if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
    return bin(index.first);
  }


  const HistoBin& Histo1D::binByCoord(double x) const {
    pair<Bin::BinType, size_t> index = _axis.findBinIndex(x);
    if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
    return bin(index.first);
  }


  double Histo1D::sumWeight() const {
    double sumw = 0;
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      sumw += b->sumW();
    }
    return sumw;
  }


  double Histo1D::area() const {
    return sumWeight();
  }


  double Histo1D::mean() const {
    double sumwx = 0;
    double sumw  = 0;
    for (size_t i = 0; i < bins().size(); i++) {
      sumwx += _bins[i].sumWX();
      sumw  += _bins[i].sumW();
    }
    return sumwx/sumw;
  }


  double Histo1D::variance() const {
    double sigma2 = 0;
    const double mean = this->mean();
    for (Bins::const_iterator b = bins().begin(); b != bins().end(); ++b) {
      const double diff = b->focus() - mean;
      sigma2 += diff * diff * b->sumW();
    }
    return sigma2/sumWeight();
  }


  double Histo1D::stdDev() const {
    return std::sqrt(variance());
  }


  Histo1D& Histo1D::operator += (const Histo1D& toAdd) {
    if (_axis != toAdd._axis) {
      throw LogicError("YODA::Histo1D: Cannot add histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      _bins[i] += toAdd._bins[i];
    }
    _underflow += toAdd._underflow;
    _overflow += toAdd._overflow;
    return *this;
  }


  Histo1D& Histo1D::operator -= (const Histo1D& toSubtract) {
    if (_axis != toSubtract._axis) {
      throw LogicError("YODA::Histo1D: Cannot subtract histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      _bins[i] += toSubtract._bins[i];
    }
    _underflow += toSubtract._underflow;
    _overflow += toSubtract._overflow;
    return *this;
  }


  Histo1D operator + (const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp += second;
    return tmp;
  }


  Histo1D operator - (const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp -= second;
    return tmp;
  }

}
