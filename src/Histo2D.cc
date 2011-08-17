// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  // typedef vector<HistoBin2D> Bins;


  int Histo2D::fill(double x, double y, double weight) {
    _axis.totalDbn().fill(x, y, weight);

    int index = _axis.getBinIndex(x, y);
    if(index != -1) {
      HistoBin2D& bin = _axis.bin(index);
      bin.fill(x, y, weight);
    }
    else if(_axis.outflows().size() == 8) _axis.fillOutflows(x, y, weight);
    return index;
  }


  double Histo2D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    foreach (const Bin& b, bins()) {
      sumw += b.sumW();
    }
    return sumw;
  }


  double Histo2D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    foreach (const Bin& b, bins()) {
      sumw2 += b.sumW2();
    }
    return sumw2;
  }


  double Histo2D::xMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xMean();
    double sumwx = 0;
    double sumw  = 0;
    foreach (const Bin2D& b, bins()) {
      sumwx += b.sumWX();
      sumw  += b.sumW();
    }
    return sumwx/sumw;
  }


  double Histo2D::yMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yMean();
    double sumwy = 0;
    double sumw = 0;
    foreach (const Bin2D& b, bins()) {
        sumwy += b.sumWY();
        sumw  += b.sumW();
    }
    return sumwy/sumw;
  }


  double Histo2D::xVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xVariance();
    double sigma2 = 0;
    const double xMean = this->xMean();
    foreach (const Bin2D& b, bins()) {
      const double diff = b.focus().first - xMean;
      sigma2 += diff * diff * b.sumW();
    }
    return sigma2/sumW();
  }


  double Histo2D::yVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yVariance();
    double sigma2 = 0;
    const double yMean = this->yMean();
    foreach (const Bin2D& b, bins()) {
      const double diff = b.focus().first - yMean;
      sigma2 += diff * diff * b.sumW();
    }
    return sigma2/sumW();
  }


  Scatter3D operator / (const Histo2D& numer, const Histo2D& denom) {
    if(numer != denom) throw GridError("The two Histos are not the same!");
    Scatter3D tmp;
    for (size_t i = 0; i < numer.numBinsTotal(); ++i) {
      const HistoBin2D& b1 = numer.bin(i);
      const HistoBin2D& b2 = denom.bin(i);
      const HistoBin2D& bL = b1 + b2;
      assert(fuzzyEquals(b1.midpoint().first, b2.midpoint().first));
      assert(fuzzyEquals(b1.midpoint().second, b2.midpoint().second));

      const double x = bL.focus().first;
      const double y = bL.focus().second;

      const double exminus = x - bL.xMin();
      const double explus = bL.xMax() - x;

      const double eyminus = y - b1.yMin();
      const double eyplus = bL.yMax() - y;
      //cout << b1.xMin() << " " << b1.xMax() << " " << b1.yMin() << " " << b1.yMax() << " EMinus: " << exminus << " " << eyminus << " Focus: " << x << " " << y << endl;

      const double z = b1.height() / b2.height();
      const double ez = z * sqrt( sqr(b1.heightErr()/b1.height()) + sqr(b2.heightErr()/b2.height()) );
      tmp.addPoint(x, exminus, explus, y, eyminus, eyplus, z, ez, ez);
    }
    assert(tmp.numPoints() == numer.numBinsTotal());
    return tmp;
  }


}
