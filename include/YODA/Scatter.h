// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER_H
#define YODA_SCATTER_H 1

#include "YODA/AnalysisObject.h"
#include <vector>
#include <string>
#include "Point.h"

namespace YODA {


  template <size_t N>
  class Scatter : public AnalysisObject {

  public:
    Scatter();

    Scatter(const std::string& path, const std::string& title);

    Scatter(const std::vector<YODA::Point<N> >& points);

    Scatter(const std::string& path, const std::string& title,
            const std::vector<YODA::Point<N> >& points);


    Scatter& point(size_t index);
    const Scatter& point(size_t index) const;

    Scatter& addPoint(const Point&);



    Scatter combineWith(const Scatter& other);
    Scatter combineWith(const std::vector<Scatter> others);

    Scatter& operator+=(const Scatter&);
    Scatter& operator-=(const Scatter&);
    Scatter& operator*=(const Scatter&);
    Scatter& operator/=(const Scatter&);


  private:

    // For debugging
    bool _isConsistent() const;


  private:

    std::vector<Point> _points;
  };



  Scatter combine(const Scatter& a, const Scatter& b);
  Scatter combine(const std::vector<Scatter> scatters);

  Scatter operator+(const Scatter &, const Scatter &);
  Scatter operator-(const Scatter &, const Scatter &);
  Scatter operator*(const Scatter &, const Scatter &);
  Scatter operator/(const Scatter &, const Scatter &);

}

#endif
