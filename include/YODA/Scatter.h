// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER_H
#define YODA_SCATTER_H

#include "YODA/AnalysisObject.h"
#include "YODA/Point.h"
#include <vector>
#include <string>

namespace YODA {


  template <size_t N>
  class Scatter : public AnalysisObject {
  public:
    Scatter() {  }


    Scatter(const std::string& path, const std::string& title)
      : AnalysisObject(path, title)
    {  }


    Scatter(const std::vector<YODA::Point<N> >& points)
      : _points(points)
    {  }


    Scatter(const std::string& path, const std::string& title,
            const std::vector<YODA::Point<N> >& points)
      : AnalysisObject(path, title),
        _points(points)
    {  }


    ///////////////////////////////////////////////////


    /// How many dimensions in this instantiation?
    size_t numDims() const {
      return N;
    }


    size_t numPoints() const {
      return _points.size();
    }


    std::vector<Point<N> >& points() {
      return _points;
    }


    const std::vector<Point<N> >& points() const {
      return _points;
    }


    Point<N>& point(size_t index) {
      assert(index < numPoints());
      return _points.at(index);
    }


    const Point<N>& point(size_t index) const {
      assert(index < numPoints());
      return _points.at(index);
    }


    Scatter& addPoint(const Point<N>&) {
      _points.push_back(pt);
      return *this;
    }


    Scatter& combineWith(const Scatter<N>& other) {
      for (vector<Point1D>::const_iterator pt = other.points().begin(); 
           pt != other.points().end(); ++pt) {
        addPoint(*pt);
      }
      return *this;
    }
  

    Scatter& combineWith(const std::vector<Scatter<N> >& others) {
      for (vector<Scatter1D>::const_iterator s = others.begin(); 
           s != others.end(); ++s) {
        combineWith(*s);
      }
      return *this;
    }


    /// @todo Something like findNearest(vec<N>, tolerance)?

    void reset() {
      _points.clear();
    }


  private:

    std::vector<Point<N> > _points;
  };



  template <size_t N>
  inline Scatter<N> combine(const Scatter<N>& a, const Scatter<N>& b) {
    Scatter<N> rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  template <size_t N>
  inline Scatter<N> combine(const std::vector<Scatter<N> >& scatters) {
    Scatter<N> rtn;
    for (vector<Scatter1D>::const_iterator s = scatters.begin(); 
         s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }


}


#endif
