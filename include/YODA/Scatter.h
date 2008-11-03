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
    Scatter();

    Scatter(const std::string& path, const std::string& title);

    /// How many dimensions in this instantiation?
    virtual size_t numDims() const {
      return N;
    }

    virtual size_t numPoints() const = 0;

    Scatter();

    Scatter(const std::string& path, const std::string& title);

    Scatter(const std::vector<YODA::Point<N> >& points);

    Scatter(const std::string& path, const std::string& title,
            const std::vector<YODA::Point<N> >& points);

    size_t numPoints() const;

    std::vector<Point<N> >& points();

    const std::vector<Point<N> >& points() const;

    Point<N>& point(size_t index);

    const Point<N>& point(size_t index) const;

    Scatter& addPoint(const Point<N>&);

    Scatter1D& combineWith(const Scatter<N>& other);
    Scatter1D& combineWith(const std::vector<Scatter<N> >& others);

    /// Something like findNearest(vec<N>, tolerance)?

    void reset();

  private:
    std::vector<Point<N> > _points;
  };

  template <size_t N>
  Scatter<N> combine(const Scatter<N>& a, const Scatter<N>& b);

  template <size_t N>
  Scatter<N> combine(const std::vector<Scatter<N> >& scatters);


}


#endif
