// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER2D_H
#define YODA_SCATTER2D_H

#include "YODA/AnalysisObject.h"
#include "YODA/Point2D.h"
#include <vector>
#include <set>
#include <string>

namespace YODA {


  class Scatter2D : public AnalysisObject {
  public:
    Scatter2D() {  }


    Scatter2D(const std::string& title)
      : AnalysisObject(title)
    {  }


    Scatter2D(const std::set<YODA::Point2D>& points)
      : _points(points)
    {  }


    Scatter2D(const std::string& title,
              const std::set<YODA::Point2D>& points)
      : AnalysisObject(title),
        _points(points)
    {  }


    ///////////////////////////////////////////////////


    size_t numPoints() const {
      return _points.size();
    }


    // std::vector<Point2D>& points() {
    //   return _points;
    // }


    const std::set<Point2D>& points() const {
      return _points;
    }


    // Point2D& point(size_t index) {
    //   assert(index < numPoints());
    //   /// @todo Fix to use an ordered set or similar
    //   return _points.at(index);
    // }


    // const Point2D& point(size_t index) const {
    //   assert(index < numPoints());
    //   /// @todo Fix to use an ordered set or similar
    //   return _points.at(index);
    // }


    Scatter2D& addPoint(const Point2D& pt) {
      _points.insert(pt);
      return *this;
    }


    Scatter2D& combineWith(const Scatter2D& other) {
      for (std::set<Point2D>::const_iterator pt = other.points().begin();
           pt != other.points().end(); ++pt) {
        addPoint(*pt);
      }
      return *this;
    }


    Scatter2D& combineWith(const std::vector<Scatter2D>& others) {
      for (std::vector<Scatter2D>::const_iterator s = others.begin();
           s != others.end(); ++s) {
        combineWith(*s);
      }
      return *this;
    }


    void reset() {
      _points.clear();
    }


  private:

    std::set<Point2D> _points;
  };



  inline Scatter2D combine(const Scatter2D& a, const Scatter2D& b) {
    Scatter2D rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  inline Scatter2D combine(const std::vector< Scatter2D >& scatters) {
    Scatter2D rtn;
    for (std::vector<Scatter2D>::const_iterator s = scatters.begin();
         s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }


}

#endif
