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
          const ErrorSet& errors);


    /// Values with symmetric errors
    // Point(const std::vector<double>& values,
    //       const std::vector<std::vector<double> >& errors);

    //@}


  public:

    /// @name Value
    //@{

    /// Get the value of this point.
    virtual std::vector<double> value() const;

    /// Get the value of this point in direction @a dim.
    virtual double value(size_t dim) const;

    /// Set the value of this point in direction @a dim.
    virtual void setValue(const std::vector<double>& values);

    /// Set the value of this point in direction @a dim.
    virtual void setValue(size_t dim, double value);

    //@}


    /// @name Errors
    //@{

    /// Get the effective error of this point in direction @a dim.
    virtual std::pair<double,double> error(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const;

    /// Get the effective error of this point in direction @a dim, passing an explicit combining functor.
    virtual std::pair<double,double> error(size_t dim, ErrorCombiner& ec) const;

    /// Get the effective plus/minus-averaged error of this point in direction @a dim.
    virtual double symmError(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const;

    /// Get the effective plus/minus-averaged error of this point in direction @a dim, passing an explicit combining functor.
    virtual double symmError(size_t dim, ErrorCombiner& ec) const;

    /// @todo Reinstate

    /// Get the effective error of this point, in all directions.
    // virtual std::vector<std::pair<double,double> > errors(ErrorCombScheme ecs=QUAD_COMB) const;

    // /// Get the effective error of this point, in all directions, passing an explicit combining functor.
    // virtual std::vector<std::pair<double,double> > errors(ErrorCombiner& ec) const;

    // /// Get the effective plus/minus-averaged error of this point in all directions.
    // virtual std::vector<double> symmErrors(ErrorCombScheme ecs=QUAD_COMB) const;

    // /// Get the effective plus/minus-averaged error of this point in all directions, passing an explicit combining functor.
    // virtual std::vector<double> symmErrors(ErrorCombiner& ec) const;


    /// Set the errors on this point, for all directions simultaneously.
    virtual void setErrors(const ErrorSet& errors);
    // virtual void setErrors(const std::vector<std::vector<std::pair<double,double> > >& errors);
    // virtual void setErrors(const std::vector<std::vector<double> >& errors);

    /// Add an error to this point.
    virtual void addError(const PointError& error);
    // virtual void addError(const std::vector<double>& error);
    // virtual void addError(const std::vector<std::pair<double,double> >& error);

    //@}


    /// How many dimensions in this instantiation?
    virtual size_t numDims() const = 0;

    
  protected:

    std::vector<double> _value;
    ErrorSet _errors;
  };



  ////////////////////////////////////////////////////



  // 1D specialisation
  class Point1D : public Point {
  public:

    // Default constructor
    Point1D();


    /// Values but no errors
    Point1D(const std::vector<double>& value);


    /// Values with asymmetric (general) errors
    Point1D(const std::vector<double>& value,
            const ErrorSet& errors);


    /// Values with symmetric errors
    // Point1D(const std::vector<double>& values,
    //       const std::vector<double>& errors);

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
    Point2D(const std::vector<double>& value);


    /// Values with asymmetric (general) errors
    Point2D(const std::vector<double>& value,
            const ErrorSet& errors);


    /// Values with symmetric errors
    // Point2D(const std::vector<double>& values,
    //       const std::vector<double>& errors);

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
