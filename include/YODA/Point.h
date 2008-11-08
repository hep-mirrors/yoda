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


  template <size_t N>
  class Point {
  public:

    /// @name Constructors
    //@{

    // Default constructor
    Point() {  }


    /// Values but no errors
    Point(const std::vector<double>& values)
      : _values(values)
    {  }


    /// Values with asymmetric (general) errors
    Point(const std::vector<double>& values,
          const ErrorSet<N>& errors)
      : _value(value), _errors(errors)
    {  }

    //@}


  public:

    /// @name Value
    //@{

    /// Get the value of this point.
    virtual std::vector<double> value() const {
      assert(_value.size() == N);
      return _value;
    }

    /// Get the value of this point in direction @a dim.
    virtual double value(size_t dim) const {
      assert(dim < numDims());
      return _value.at(dim);
    }

    /// Set the value of this point in direction @a dim.
    virtual void setValue(const std::vector<double>& values) {
      assert(values.size() == numDims());
      _value = values;
    }

    /// Set the value of this point in direction @a dim.
    virtual void setValue(size_t dim, double value) {
      assert(dim < numDims());
      _value[dim] = value;
    }

    //@}


    /// @name Errors
    //@{

    /// Get the effective error of this point in direction @a dim.
    virtual std::pair<double,double> error(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const {
      assert(dim < numDims());
      pair<double,double> rtn = make_pair(0.0, 0.0);
      if (ecs == QUAD_COMB) {
        QuadErrComb qec;
        rtn = error(dim, qec);
      } else if (ecs == LIN_COMB) {
        LinErrComb lec;
        rtn = error(dim, lec);
      } else if (ecs == HYBRID_COMB) {
        throw Exception("HYBRID_COMB error combination not yet supported");
        /// @todo HYBRID_COMB error combination support needed
        //QuadLinErrComb qlec;
        //rtn = error(qlec);
      } else {
        throw Exception("Requested unknown error combination scheme... how?!?");
      }
      return rtn;
    }

    /// Get the effective error of this point in direction @a dim, passing an explicit combining functor.
    virtual std::pair<double,double> error(size_t dim, ErrorCombiner& ec) const {
      assert(dim < numDims());
      return ec.combine_errs(_errors.begin(), _errors.end()).at(dim);
    }

    /// Get the effective plus/minus-averaged error of this point in direction @a dim.
    virtual double symmError(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const {
      pair<double,double> errs = error(dim, ecs);
      return (errs.first + errs.second)/2.0;
    }

    /// Get the effective plus/minus-averaged error of this point in direction @a dim, passing an explicit combining functor.
    virtual double symmError(size_t dim, ErrorCombiner& ec) const {
      pair<double,double> errs = error(dim, ec);
      return (errs.first + errs.second)/2.0;
    }

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
    virtual void setErrors(const ErrorSet<N>& errors) {
      _errors = errors;
    }


    // virtual void setErrors(const std::vector<std::vector<std::pair<double,double> > >& errors);
    // virtual void setErrors(const std::vector<std::vector<double> >& errors);

    /// Add an error to this point.
    virtual void addError(const PointError& error) {
      _errors.push_back(error);
    }
    // virtual void addError(const std::vector<double>& error);
    // virtual void addError(const std::vector<std::pair<double,double> >& error);

    //@}


    /// How many dimensions in this instantiation?
    size_t numDims() const {
      return N;
    }

    
  protected:

    double _value[N];
    ErrorSet<N> _errors;
  };



  ////////////////////////////////////////////////////



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
