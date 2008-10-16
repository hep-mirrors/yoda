// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT_H
#define YODA_POINT_H 1

#include <vector>
#include <string>
#include <map>

namespace YODA {


  template <size_t N>
  class Point {

    /// @todo Allow multiple (enum/string-typed) errors

  public:

    /// @name Constructors
    //@{

    /// Asymmetric (general) errors
    Point(const std::vector<double>& values,
          const std::vector<std::pair<double,double> >& errors);

    /// Symmetric errors
    Point(const std::vector<double>& values,
          const std::vector<double>& errors);

    //@}


  public:

    /// @name Errors
    //@{

    enum ErrorCombScheme { QUADRATURE, LINEAR, QUAD_STAT_LIN_SYS };

    double value(size_t dim);
    std::pair<double,double> error(ErrorCombScheme=QUADRATURE);
    double symmError(ErrorCombScheme=QUADRATURE);

    std::vector<std::pair<double,double> > errors();
    std::vector<double> symmErrors();
    //@}


  private:

    std::vector<double> _values;
    std::vector<std::pair<double,double> > _errors;
  };



  ////////////////////////////////////////////////////



  // // 1D specialisation
  // class Point1D : public Point<2> {

  // };



  // // 2D specialisation
  // class Point2D : public Point<2> {

  // public:
  //   Point2D(double x, double y,
  //         double xerror,
  //         double yerror);

  //   Point2D(double x, double y,
  //         double xerrorminus,
  //         double xerrorplus,
  //         double yerrorminus,
  //         double yerrorplus);

  //   Point2D(double x, double y,
  //         std::vector<double> xerror,
  //         std::vector<double> yerror);

  //   Point2D(double x, double y,
  //         std::vector<double> xerrorminus,
  //         std::vector<double> xerrorplus,
  //         std::vector<double> yerrorminus,
  //         std::vector<double> yerrorplus);

  // public:
  //   double getX();
  //   double getY();

  //   std::vector<double> getXErrorMinus();
  //   std::vector<double> getXErrorPlus();
  //   std::vector<double> getYErrorMinus();
  //   std::vector<double> getYErrorPlus();

  // };

}

#endif
