// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT_H
#define YODA_POINT_H

#include "YODA/Error.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  class Point {
  public:

    /// @name Constructors
    //@{

    // Default constructor
    Point();


    /// Values but no errors
    Point(const std::vector<double>& values);


    /// Values with asymmetric (general) errors
    Point(const std::vector<double>& values,
          const std::vector<ErrorSet>& errors);


    /// Values with symmetric errors
    Point(const std::vector<double>& values,
          const std::vector<std::vector<double> >& errors);

    //@}


  public:

    /// @name Errors
    //@{

    /// Get the value of this point in direction @a dim.
    virtual double value(size_t dim);

    /// Get the effective error of this point in direction @a dim.
    virtual std::pair<double,double> error(size_t dim, ErrorCombScheme ecs=QUAD_COMB);
    /// Get the effective error of this point in direction @a dim, passing an explicit combining functor.
    virtual std::pair<double,double> error(size_t dim, ErrorCombiner& ec);
    /// Get the effective plus/minus-averaged error of this point in direction @a dim.
    virtual double symmError(size_t dim, ErrorCombScheme ecs=QUAD_COMB);
    /// Get the effective plus/minus-averaged error of this point in direction @a dim, passing an explicit combining functor.
    virtual double symmError(size_t dim, ErrorCombiner& ec);

    /// Get the effective error of this point, in all directions.
    virtual std::vector<std::pair<double,double> > errors(ErrorCombScheme ecs=QUAD_COMB);
    /// Get the effective error of this point, in all directions, passing an explicit combining functor.
    virtual std::vector<std::pair<double,double> > errors(ErrorCombiner& ec);
    /// Get the effective plus/minus-averaged error of this point in all directions.
    virtual std::vector<double> symmErrors(ErrorCombScheme ecs=QUAD_COMB);
    /// Get the effective plus/minus-averaged error of this point in all directions, passing an explicit combining functor.
    virtual std::vector<double> symmErrors(ErrorCombiner& ec);

    //@}


    /// How many dimensions in this instantiation?
    virtual size_t numDims() const = 0;

    
  protected:

    std::vector<double> _values;
    std::vector<ErrorSet> _errors;
  };



  ////////////////////////////////////////////////////



  // 1D specialisation
  class Point1D : public Point {
  public:

    // Default constructor
    Point1D();


    /// Values but no errors
    Point1D(const std::vector<double>& values);


    /// Values with asymmetric (general) errors
    Point1D(const std::vector<double>& values,
            const std::vector<std::pair<double,double> >& errors);


    /// Values with symmetric errors
    Point1D(const std::vector<double>& values,
          const std::vector<double>& errors);

    //public:
    /// @todo Add simple (no "dimension arg") value and error accessors

  public:
    size_t numDims() const;
  };



  // 2D specialisation
  class Point2D : public Point {
  public:

    // Default constructor
    Point2D();


    /// Values but no errors
    Point2D(const std::vector<double>& values);


    /// Values with asymmetric (general) errors
    Point2D(const std::vector<double>& values,
            const std::vector<std::pair<double,double> >& errors);


    /// Values with symmetric errors
    Point2D(const std::vector<double>& values,
          const std::vector<double>& errors);

  public:
    size_t numDims() const;

    /// @todo Add X, Y value and error accessors

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
  };


}

#endif
