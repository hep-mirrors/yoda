// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"
#include <cmath>
using namespace std;

namespace YODA {


  int Histo2D::fill(double x, double y, double weight) {
    return _axis.fill(x, y, weight);
  }


  double Histo2D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    for(size_t i = 0; i < bins().size(); ++i) sumw += bin(i).sumW();
    return sumw;
  }


  double Histo2D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    for(size_t i = 0; i < bins().size(); ++i) sumw2+= bin(i).sumW2();
    return sumw2;
  }


  double Histo2D::xMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xMean();
    double sumwx = 0;
    double sumw  = 0;
    for(size_t i = 0; i < bins().size(); ++i) {
      sumwx += bin(i).sumW2();
      sumw  += bin(i).sumW();
    }
    return sumwx/sumw;
  }


  double Histo2D::yMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yMean();
    double sumwy = 0;
    double sumw = 0;
    for(size_t i = 0; i < bins().size(); ++i) {
      sumwy += bin(i).sumWY();
      sumw  += bin(i).sumW();
    }
    return sumwy/sumw;
  }


  double Histo2D::xVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xVariance();
    double sigma2 = 0;
    const double xMean = this->xMean();
    for(size_t i = 0; i < bins().size(); ++i) {
      const double diff = bin(i).focus().first - xMean;
      sigma2 += diff * diff * bin(i).sumW();
    }
    return sigma2/sumW();
  }


  double Histo2D::yVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().yVariance();
    double sigma2 = 0;
    const double yMean = this->yMean();
    for(size_t i = 0; i < bins().size(); ++i) {
      const double diff = bin(i).focus().first - yMean;
      sigma2 += diff * diff * bin(i).sumW();
    }
    return sigma2/sumW();
  }


    Histo1D Histo2D::cutterX(double atY, const std::string& path, const std::string& title) {
      if (!_axis.isGrid()) throw GridError("Attempt to cut a Histo2D that is not a grid!");

      if (atY < lowEdgeY() || atY > highEdgeY()) throw RangeError("Y is outside the grid");
      vector<HistoBin1D> tempBins;

      for (double i = binByCoord(lowEdgeX(), atY).xMin(); i < highEdgeX(); i += binByCoord(i, atY).widthX()) {
        const HistoBin2D& b2 = binByCoord(i, atY);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeX(), b2.highEdgeX(), dbn2));
      }

      /// Setting under/over flows
      Dbn2D underflow;
      underflow += _axis.outflows()[7][_axis.getBinRow(_axis.getBinIndex(lowEdgeX(), atY))];

      Dbn2D overflow;
      overflow += _axis.outflows()[3][_axis.getBinRow(_axis.getBinIndex(lowEdgeX(), atY))];

      return Histo1D(tempBins, _axis.totalDbn().transformX(), underflow.transformX(), overflow.transformX(), path, title);

    }


    Histo1D Histo2D::cutterY(double atX, const std::string& path, const std::string& title) {
      if (!_axis.isGrid()) throw GridError("Attempt to cut a Histo2D that is not a grid!");

      if (atX < lowEdgeX() || atX > highEdgeX()) throw RangeError("X is outside the grid");
      vector<HistoBin1D> tempBins;

      for (double i = binByCoord(atX, lowEdgeY()).yMin(); i < highEdgeY(); i += binByCoord(atX, i).widthY()) {
        const HistoBin2D& b2 = binByCoord(atX, i);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeY(), b2.highEdgeY(), dbn2));
      }

      // Setting under/over flows
      Dbn2D underflow;
      underflow += _axis.outflows()[1][_axis.getBinColumn(_axis.getBinIndex(atX, lowEdgeY()))];

      Dbn2D overflow;
      overflow += _axis.outflows()[5][_axis.getBinColumn(_axis.getBinIndex(atX, lowEdgeY()))];
      Dbn2D total = _axis.totalDbn();

      // Making sure that we rotate our distributions, as we are cutting parallel to Y axis now
      total.flipXY();
      underflow.flipXY();
      overflow.flipXY();

      return Histo1D(tempBins, total.transformX(), underflow.transformX(), overflow.transformX(), path, title);
    }


    Profile1D Histo2D::mkProfileX() {
      if (!_axis.isGrid()) throw GridError("Profile1D cannot be made from a histogram that is not a grid!");

      vector<ProfileBin1D> prof;
      for(int i = lowEdgeX() + _axis.bin(0).midpoint().first; i < highEdgeX(); i+= _axis.bin(0).widthX()) {
        HistoBin2D& bin(_axis.binByCoord(i, lowEdgeY()));
        HistoBin2D composite(bin.xMin(), bin.xMax(), bin.yMin(), bin.yMax()) ;
        for(int j = lowEdgeY() + _axis.bin(0).midpoint().second; j < highEdgeY(); j += _axis.bin(0).widthY()) {
          composite += _axis.binByCoord(i, j);
        }
      prof.push_back(composite.transformX());
      }

      vector<vector<Dbn2D> >& outflows = _axis.outflows();

      /// Properly setting an underflow
      Dbn2D underflow;
      underflow += outflows[0][0]; underflow += outflows[6][0];
      for(size_t i = 0; i < outflows[7].size(); ++i) {
        underflow += outflows[7][i];
      }

      /// Setting an overflow
      Dbn2D overflow;
      overflow += outflows[2][0]; overflow += outflows[4][0];
      for(size_t i = 0; i < outflows[3].size(); ++i) {
        overflow += outflows[3][i];
      }

      /// And constructing a profile 1D from all this data
      Profile1D ret(prof, _axis.totalDbn(), underflow, overflow);
      return ret;

    }

    Profile1D Histo2D::mkProfileY() {
      if (!_axis.isGrid()) throw GridError("Profile1D cannot be made from a histogram that is not a grid!");

      vector<ProfileBin1D> prof;
      for(int i = lowEdgeY() + _axis.bin(0).midpoint().second; i < highEdgeY(); i+= _axis.bin(0).widthY()) {
        HistoBin2D& bin(_axis.binByCoord(i, lowEdgeY()));
        HistoBin2D composite(bin.xMin(), bin.xMax(), bin.yMin(), bin.yMax()) ;
        for(int j = lowEdgeX() + _axis.bin(0).midpoint().first; j < highEdgeX(); j += _axis.bin(0).widthX()) {
          composite += _axis.binByCoord(i, j);
        }
      prof.push_back(composite.transformY());
      }

      vector<vector<Dbn2D> >& outflows = _axis.outflows();

      /// Properly setting an underflow
      Dbn2D underflow;
      underflow += outflows[0][0]; underflow += outflows[2][0];
      for(size_t i = 0; i < outflows[1].size(); ++i) {
        underflow += outflows[1][i];
      }

      /// Setting an overflow
      Dbn2D overflow;
      overflow += outflows[6][0]; overflow += outflows[4][0];
      for(size_t i = 0; i < outflows[5].size(); ++i) {
        overflow += outflows[5][i];
      }

      /// Setting a flipped total distribution
      Dbn2D td = _axis.totalDbn();
      td.flipXY();

      /// And constructing a profile 1D from all this data
      Profile1D ret(prof, td, underflow, overflow);
      return ret;
    }


  /// @todo Check how finding the correct bins works in the case of
  /// a sparse representation, and if it is decent, code it in here.
  Scatter3D divide(const Histo2D& numer, const Histo2D& denom) {
    if (numer != denom) throw GridError("The two histos are not equivalently binned!");
    Scatter3D tmp;
    for (size_t i = 0; i < numer.numBinsTotal(); ++i) {
      const HistoBin2D& b1 = numer.bin(i);
      const HistoBin2D& b2 = denom.binByCoord(b1.midpoint().first, b1.midpoint().second);
      const HistoBin2D& bL = b1 + b2;
      assert(fuzzyEquals(b1.midpoint().first, b2.midpoint().first));
      assert(fuzzyEquals(b1.midpoint().second, b2.midpoint().second));

      const double x = bL.focus().first;
      const double y = bL.focus().second;

      const double exminus = x - bL.xMin();
      const double explus = bL.xMax() - x;

      const double eyminus = y - b1.yMin();
      const double eyplus = bL.yMax() - y;

      const double z = b1.height() / b2.height();
      const double ez = z * sqrt( sqr(b1.heightErr()/b1.height()) + sqr(b2.heightErr()/b2.height()) );
      tmp.addPoint(x, exminus, explus, y, eyminus, eyplus, z, ez, ez);
    }
    assert(tmp.numPoints() == numer.numBinsTotal());
    return tmp;
  }


}
