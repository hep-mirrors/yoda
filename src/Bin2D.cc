#include "YODA/Bin2D.h"
#include "YODA/HistoBin1D.h"
#include "YODA/Exceptions.h"

#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

namespace YODA {

  Bin2D::Bin2D(double lowedgeX, double lowedgeY, double highedgeX, double highedgeY) {
    if(lowedgeX > highedgeX || lowedgeY > highedgeY) throw RangeError("The bins are wrongly defined!"); 
    
    _edges.first.first = lowedgeX;
    _edges.first.second = lowedgeY;
    _edges.second.first = highedgeX;
    _edges.second.second = highedgeY;

    isReal = true;
  }

  Bin2D::Bin2D(std::vector<std::pair<std::pair<double,double>,
               std::pair<double,double> > > edges) {
    if(edges.size() != 4) throw RangeError("The edge vector does not define a full rectangle!");
    
    _edges.first.first = edges[0].first.first;
    _edges.first.second = edges[0].first.second;
    _edges.second.first = edges[1].second.first;
    _edges.second.second = edges[1].second.second;

    isReal = true;
  }

  void Bin2D::scaleXY(double scaleX, double scaleY) {
    _edges.first.first *= scaleX;
    _edges.second.first *= scaleX;

    _edges.first.second *= scaleY;
    _edges.second.second *= scaleY;

    _dbn.scaleXY(scaleX, scaleY);
  }

  std::pair<double,double> Bin2D::midpoint() const {
    return make_pair((double)(xMax() - xMin())/2 + xMin(), (double)(yMax() - yMin())/2 + yMin());
  }

    Bin2D& Bin2D::subtract(const Bin2D& b) {
        /// Automatically resize if adding a bin that does not have the same location
        /// this way merging the bins works perfectly
        if(_edges != b._edges) {
          if (b.xMax() > xMax()) _setBounds(xMin(), yMin(), b.xMax(), yMax());
          if (b.yMax() > yMax()) _setBounds(xMin(), yMin(), xMax(), b.yMax());
          if (b.xMin() < xMin()) _setBounds(b.xMin(), yMin(), xMax(), yMax());
          if (b.yMin() < yMin()) _setBounds(xMin(), b.yMin(), xMax(), yMax());
        }
        _dbn -= b._dbn;
        return *this;
    }

    Bin2D& Bin2D::add(const Bin2D& b) {
        if(_edges != b._edges) {
          if (b.highEdgeX() > highEdgeX());// _setBounds(xMin(), yMin(), b.xMax(), yMax());
          if (b.yMax() > yMax()) _setBounds(xMin(), yMin(), xMax(), b.yMax());
          if (b.xMin() < xMin()) _setBounds(b.xMin(), yMin(), xMax(), yMax());
          if (b.yMin() < yMin()) _setBounds(xMin(), b.yMin(), xMax(), yMax());
        }
        _dbn += b._dbn;
        return *this;
    }
}
