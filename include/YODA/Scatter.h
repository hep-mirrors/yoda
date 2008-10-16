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


  class Scatter : public AnalysisObject {
  public:
    Scatter();

    /// How many dimensions in this instantiation?
    virtual size_t numDims() = 0;

    virtual size_t numPoints() const = 0;

    virtual void reset() = 0;

  };



  class Scatter1D : Scatter {
  public:
    Scatter1D(const std::string& path, const std::string& title);

    Scatter1D(const std::vector<YODA::Point1D>& points);

    Scatter1D(const std::string& path, const std::string& title,
              const std::vector<YODA::Point1D>& points);

    size_t numPoints() const;
    std::vector<Point1D>& points();
    const std::vector<Point1D>& points() const;
    Point1D& point(size_t index);
    const Point1D& point(size_t index) const;
    Scatter1D& addPoint(const Point1D&);

    Scatter1D& combineWith(const Scatter1D& other);
    Scatter1D& combineWith(const std::vector<Scatter1D>& others);

    void reset();

  private:
    std::vector<Point1D> _points;    
  };

  Scatter1D combine(const Scatter1D& a, const Scatter1D& b);
  Scatter1D combine(const std::vector<Scatter1D>& scatters);


  ////////////////////////////////////////////////////////


  class Scatter2D : Scatter {
  public:
    Scatter2D(const std::string& path, const std::string& title);

    Scatter2D(const std::vector<YODA::Point2D>& points);

    Scatter2D(const std::string& path, const std::string& title,
              const std::vector<YODA::Point2D>& points);

    size_t numPoints() const;
    std::vector<Point2D>& points();
    const std::vector<Point2D>& points() const;
    Point2D& point(size_t index);
    const Point2D& point(size_t index) const;
    Scatter2D& addPoint(const Point2D&);

    Scatter2D& combineWith(const Scatter2D& other);
    Scatter2D& combineWith(const std::vector<Scatter2D>& others);

    void reset();

  private:
    std::vector<Point2D> _points;
  };

  Scatter2D combine(const Scatter2D& a, const Scatter2D& b);
  Scatter2D combine(const std::vector<Scatter2D>& scatters);


}


#endif
