// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER_H
#define YODA_SCATTER_H 1

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

    Scatter(const std::vector<YODA::Point<N> >& points);

    Scatter(const std::string& path, const std::string& title,
            const std::vector<YODA::Point<N> >& points);


    Scatter<N>& point(size_t index);
    const Scatter<N>& point(size_t index) const;

    Scatter<N>& addPoint(const YODA::Point<N>&);


    Scatter<N> combineWith(const Scatter<N>& other);
    Scatter<N> combineWith(const std::vector<Scatter<N> > others);

    Scatter<N>& operator+=(const Scatter<N>&);
    Scatter<N>& operator-=(const Scatter<N>&);
    Scatter<N>& operator*=(const Scatter<N>&);
    Scatter<N>& operator/=(const Scatter<N>&);


    virtual void reset() {
      _points.clear();
    }


  private:

    std::vector<Point<N> > _points;
  };


  template <size_t N>
  Scatter<N> combine(const Scatter<N>& a, const Scatter<N>& b);
  template <size_t N>
  Scatter<N> combine(const std::vector<Scatter<N> > scatters);

  template <size_t N>
  Scatter<N> operator+(const Scatter<N>&, const Scatter<N>&);
  template <size_t N>
  Scatter<N> operator-(const Scatter<N>&, const Scatter<N>&);
  template <size_t N>
  Scatter<N> operator*(const Scatter<N>&, const Scatter<N>&);
  template <size_t N>
  Scatter<N> operator/(const Scatter<N>&, const Scatter<N>&);


  // class Scatter1D : Scatter<1> {
  //   // virtual void reset {
  //   //   _points.clear();
  //   // }
  // };


  // class Scatter2D : Scatter<2> {
  //   // virtual void reset {
  //   //   _points.clear();
  //   // }
  // };


}


#endif
