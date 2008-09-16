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


  typedef vector<HistoBin> Bins;


  Histo1D::Histo1D(const std::string& path, const std::string& title,
                   const vector<double>& binedges) :
    AnalysisObject( path, title ),
    _axis(binedges)
  {  }


  Histo1D::Histo1D(const std::vector<double>& binedges) :
    AnalysisObject(),
    _axis(binedges)
  {  }


  Histo1D::Histo1D(const std::string& path, const std::string& title,
                   size_t nbins, double lower, double upper, Binning binning) :
    AnalysisObject( path, title ),
    _axis(nbins, lower, upper, binning)
  {  }


  Histo1D::Histo1D(size_t nbins, double lower, double upper, Binning binning) :
    AnalysisObject(),
    _axis(nbins, lower, upper, binning)
  {  }


  Histo1D::Histo1D(std::string path, std::string title,
                   const vector<HistoBin>& bins) :
    AnalysisObject( path, title ),
    _axis(bins)
  {  }


  Histo1D::Histo1D(const std::vector<HistoBin>& bins) :
    AnalysisObject(),
    _axis(bins)
  {  }


  /////////////////////////////////////////////////


  void Histo1D::reset () {
    _axis.reset();
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
    return _axis.bins();
  }


  const vector<HistoBin>& Histo1D::bins() const {
    return _axis.bins();
  }


  HistoBin& Histo1D::bin(size_t index) {
    return _axis.bin(index);
  }


  const HistoBin& Histo1D::bin(size_t index) const {
    return _axis.bin(index);
  }


  HistoBin& Histo1D::bin(Bin::BinType binType) {
    return _axis.bin(binType);
  }


  const HistoBin& Histo1D::bin(Bin::BinType binType) const {
    return _axis.bin(binType);
  }


  HistoBin& Histo1D::binByCoord(double x) {
    return _axis.binByCoord(x);
  }


  const HistoBin& Histo1D::binByCoord(double x) const {
    return _axis.binByCoord(x);
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
      sumwx += bins().at(i).sumWX();
      sumw  += bins().at(i).sumW();
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
    /// @todo Use Axis<>::operator+= instead
    if (_axis != toAdd._axis) {
      throw LogicError("YODA::Histo1D: Cannot add histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      bins().at(i) += toAdd.bins().at(i);
    }
    _axis.bin(UNDERFLOW) += toAdd._axis.bin(UNDERFLOW);
    _axis.bin(OVERFLOW)  += toAdd._axis.bin(OVERFLOW);
    return *this;
  }


  Histo1D& Histo1D::operator -= (const Histo1D& toSubtract) {
    /// @todo Use Axis<>::operator-= instead

    if (_axis != toSubtract._axis) {
      throw LogicError("YODA::Histo1D: Cannot subtract histograms with different binnings.");
    }
    for (size_t i = 0; i < bins().size(); ++i) {
      bins().at(i) += toSubtract.bins().at(i);
    }
    _axis.bin(UNDERFLOW) += toSubtract._axis.bin(UNDERFLOW);
    _axis.bin(OVERFLOW)  += toSubtract._axis.bin(OVERFLOW);
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
