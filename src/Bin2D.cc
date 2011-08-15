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

    pair<pair<double,double>, pair<double,double> > edge1 =
      make_pair(make_pair(lowedgeX, lowedgeY), make_pair(lowedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge2 =
      make_pair(make_pair(lowedgeX, highedgeY), make_pair(highedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge3 =
      make_pair(make_pair(highedgeX, lowedgeY), make_pair(highedgeX, highedgeY));
    pair<pair<double,double>, pair<double,double> > edge4 =
      make_pair(make_pair(lowedgeX, lowedgeY), make_pair(highedgeX, lowedgeY));

    _edges.push_back(edge1); _edges.push_back(edge2);
    _edges.push_back(edge3); _edges.push_back(edge4);
  }


  Bin2D::Bin2D(std::vector<std::pair<std::pair<double,double>,
               std::pair<double,double> > > edges) {
    assert(edges.size() == 4);
    _edges.push_back(edges[0]); _edges.push_back(edges[1]);
    _edges.push_back(edges[2]); _edges.push_back(edges[3]);
  }


  void Bin2D::scaleXY(double scaleX, double scaleY) {
    for (unsigned int i = 0; i < _edges.size(); ++i) {
      _edges[i].first.first *= scaleX;
      _edges[i].second.first *= scaleX;
      _edges[i].first.second *= scaleY;
      _edges[i].second.second *= scaleY;
    }
    _dbn.scaleXY(scaleX, scaleY);
  }


}
