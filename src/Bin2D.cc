#include "YODA/Bin2D.h"
#include "YODA/HistoBin1D.h"

#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

namespace YODA {

  Bin2D::Bin2D(double lowedgeX, double lowedgeY, double highedgeX, double highedgeY) {
    assert(lowedgeX <= highedgeX && lowedgeY <= highedgeY);
    
    /// @todo Why store with so much redundancy?
    pair<pair<double,double>, pair<double,double> > edge1 =
      make_pair(make_pair(lowedgeX, lowedgeY), make_pair(lowedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge2 =
      make_pair(make_pair(lowedgeX, highedgeY), make_pair(highedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge3 =
      make_pair(make_pair(highedgeX, lowedgeY), make_pair(highedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge4 =
      make_pair(make_pair(lowedgeX, lowedgeY), make_pair(highedgeX, lowedgeY));

    _edges.push_back(edge1);
    _edges.push_back(edge2);
    _edges.push_back(edge3);
    _edges.push_back(edge4);

    isReal = true;
  }

  Bin2D::Bin2D(std::vector<std::pair<std::pair<double,double>,
               std::pair<double,double> > > edges) {
    assert(edges.size() == 4);
    _edges.push_back(edges[0]);
    _edges.push_back(edges[1]);
    _edges.push_back(edges[2]);
    _edges.push_back(edges[3]);

    isReal = true;
  }


  void Bin2D::scaleXY(double scaleX, double scaleY) {
    for (size_t i = 0; i < _edges.size(); ++i) {
      _edges[i].first.first *= scaleX;
      _edges[i].second.first *= scaleX;
      _edges[i].first.second *= scaleY;
      _edges[i].second.second *= scaleY;
    }
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
