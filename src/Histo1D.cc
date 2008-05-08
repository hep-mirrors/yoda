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
                  AnalysisObject ( path, title ),
                  _bins (),
                  _underflow ( Bin(0,1) ),
                  _overflow ( Bin(0,1) ),
                  _cachedBinEdges( binedges ),
                  _nbins ( binedges.size()-1 ),
                  _binHash ()
  {
    sort(_cachedBinEdges.begin(), _cachedBinEdges.end());
    for (size_t i = 0; i < _nbins; i++) {
      _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
      // Insert upper bound mapped to bin ID
      _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
    }
  }


  Histo1D::Histo1D(const std::string& path, const std::string& title,
           size_t nbins, double lower, double upper, bool log) :
                  AnalysisObject ( path, title ),
                  _bins (),
                  _underflow ( Bin(0,1) ),
                  _overflow ( Bin(0,1) ),
                  _cachedBinEdges(),
                  _nbins ( nbins ),
                  _binHash ()
  {
    if (!log) {
      const double binwidth = (upper-lower)/static_cast<double>(_nbins);
      for (size_t i = 0; i <= _nbins; i++) {
        const double edge = lower + binwidth*i;
        //cout << " " << edge << flush;
        _cachedBinEdges.push_back(edge);
      }
      //cout << endl;
    } else {
      if (lower <= 0.0) {
        throw LogicError("YODA::Histo: log binning with 0 as lower bound.");
      }
      double c = log10(upper/lower)/nbins;
      for (size_t i = 0; i< nbins; ++i) {
        _cachedBinEdges.push_back(lower*pow(10.,c*i));
      }
      _cachedBinEdges.push_back(lower*pow(10.,c*(nbins+1)));
    }
    //cout << "Added bin edges:";
    for (size_t i = 0; i < _nbins; i++) {
      //cout << " " << _cachedBinEdges[i];
      _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
      _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
    }
    //cout << " " << _cachedBinEdges[_nbins] << endl;
  }


  Histo1D::Histo1D(std::string path, std::string title,
           const vector<Bin>& bins) :
                  AnalysisObject ( path, title ),
                  _bins ( bins ),
                  _underflow ( Bin(0,1) ),
                  _overflow ( Bin(0,1) ),
                  _cachedBinEdges(),
                  _nbins ( bins.size() ),
                  _binHash ()
  {
    for (size_t i = 0; i<_nbins; ++i) {
      _cachedBinEdges.push_back(_bins[i].lowEdge());
      _binHash.insert(make_pair(_bins[i].highEdge(),i));
    }
    _cachedBinEdges.push_back(_bins.back().highEdge());
  }


  void Histo1D::reset () {
    _underflow.reset();
    _overflow.reset();
    for (vector<Bin>::iterator b = _bins.begin();
         b != _bins.end(); ++b)
      b->reset();
  }


  void Histo1D::fill(double x, double weight) {
    pair<Histo1D::BinType, size_t> index = _coordToIndex(x);
    //cout << "Coord-to-index: coord=" << x << " -> " << index.second 
    //     << " (" << index.first << ")" << endl;
    if ( index.first == VALIDBIN ) {
      //cout << "Filling bin " << index.second << " with " << x 
      //     << " (w=" << weight << ")" << endl;
      _bins[index.second].fill(x, weight);
    } else if (index.first == UNDERFLOWBIN)
      _underflow.fill(0.5, weight);
    else
      _overflow.fill(0.5, weight);
  }


  void Histo1D::fillBin(size_t index, double weight) {
    if (index >= _nbins)
      throw RangeError("YODA::Histo: index out of range");
    double x = _bins[index].midpoint();
    Histo1D::_bins[index].fill(x, weight);
  }


  const vector<Bin>& Histo1D::bins() const {
    return _bins;
  }


  const Bin& Histo1D::bin(size_t index) const {
    if (index >= _nbins)
      throw RangeError("YODA::Histo: index out of range");
    return _bins[index];  
  }


  const Bin& Histo1D::bin(Histo1D::BinType binType) const {
    if (binType == UNDERFLOWBIN) return _underflow;
    if (binType == OVERFLOWBIN) return _overflow;
    throw RangeError("YODA::Histo: index out of range");
    // just to fix a warning
    return _underflow;
  }


  const Bin& Histo1D::binByCoord(double x) const {
    pair<Histo1D::BinType, size_t> index = _coordToIndex(x);
    if ( index.first == VALIDBIN ) return _bins[index.second];
    return bin(index.first);
  }


  pair<Histo1D::BinType, size_t> Histo1D::_coordToIndex(double coord) const {
    //cout << "Upper/lower bounds: " << _cachedBinEdges[0] << ", " << _cachedBinEdges[_nbins] << endl;
    if ( coord < _cachedBinEdges[0] ) return make_pair(UNDERFLOWBIN, 0);
    if ( coord >= _cachedBinEdges[_nbins] ) return make_pair(OVERFLOWBIN, 0);
    // size_t i = 0;
    //  while (_cachedBinEdges[i+1] < coord) i++;
    // SP: this is faster, I think;
    // if's above ensure, that we get
    // a valid iterator back
    size_t i = _binHash.upper_bound(coord)->second;
    return make_pair(VALIDBIN, i);
  }


  double Histo1D::totalArea() const {
    double area = 0;
    for (size_t i = 0; i < _nbins; i++)
      area += _bins[i].area();
    return area;
  }


  double Histo1D::mean() const {
    double sumwx = 0;
    double sumw  = 0;
    for (size_t i = 0; i < _nbins; i++) {
      sumwx += _bins[i].sumXWeight();
      sumw  += _bins[i].sumWeight();
    }
    return sumwx/sumw;
  }


  double Histo1D::variance() const {
    double sigma2 = 0;
    const double mean = this->mean();
    for (size_t i = 0; i < _nbins; i++) {
      const double diff = _bins[i].focus() - mean;
      sigma2 += diff * diff * _bins[i].sumWeight();
    }
    return sigma2/totalArea();
  }


  double Histo1D::stdDev() const {
    return std::sqrt(variance());
  }


  Histo1D& Histo1D::operator += (const Histo1D& toAdd) {
    if (_cachedBinEdges != toAdd._cachedBinEdges
        || _binHash != toAdd._binHash)
      throw LogicError("YODA::Histo1D: Cannot add histograms with different binnings.");
    for (size_t i = 0; i<_nbins; ++i)
      _bins[i] += toAdd._bins[i];
    _underflow += toAdd._underflow;
    _overflow += toAdd._overflow;
    return *this;
  }

  Histo1D& Histo1D::operator -= (const Histo1D& toSubtract) {
    if (_cachedBinEdges != toSubtract._cachedBinEdges
        || _binHash != toSubtract._binHash)
      throw LogicError("YODA::Histo1D: Cannot subtract histograms with different binnings.");
    for (size_t i = 0; i<_nbins; ++i)
      _bins[i] += toSubtract._bins[i];
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
