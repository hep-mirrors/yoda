// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT_H
#define YODA_POINT_H

#include "YODA/Error.h"
#include "YODA/Exception.h"
#include "YODA/Util.h"

#include <vector>
#include <string>
#include <map>
#include <utility>

namespace YODA {


  template <size_t N>
  class Point {
  public:

    /// @name Constructors
    //@{

    // Default constructor
    Point() {  }


    /// Values but no errors
    Point(const std::vector<double>& value)
      : _value(value)
    {  }


    /// Values with asymmetric (general) errors
    Point(const std::vector<double>& value,
          const std::vector< PointError<N> >& errors)
      : _value(value), _errors(errors)
    {  }

    //@}


  public:

    /// @name Value
    //@{

    /// Get the value of this point.
    virtual std::vector<double> value() const {
      assert(_value.size() == N);
      std::vector<double> rtn(N);
      std::copy(_value.begin(), _value.end(), rtn.begin());
      return rtn;
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
    YODA::Error1D error(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const {
      assert(dim < numDims());
      Error1D rtn = std::make_pair(0.0, 0.0);
      if (ecs == QUAD_COMB) {
        QuadErrComb<N> qec;
        rtn = error(dim, qec);
      } else if (ecs == LIN_COMB) {
        LinErrComb<N> lec;
        rtn = error(dim, lec);
      } else if (ecs == HYBRID_COMB) {
        throw Exception("HYBRID_COMB error combination not yet supported");
        /// @todo HYBRID_COMB error combination support needed
        //QuadLinErrComb<N> qlec;
        //rtn = error(qlec);
      } else {
        throw Exception("Requested unknown error combination scheme... how?!?");
      }
      return rtn;
    }

    /// Get the effective error of this point in direction @a dim, passing an explicit combining functor.
    template <typename ERRORCOMBINER>
    YODA::Error1D error(size_t dim, ERRORCOMBINER& ec) const {
      assert(dim < numDims());
      return ec.combine_errs(_errors.begin(), _errors.end()).at(dim);
    }

    /// Get the effective plus/minus-averaged error of this point in direction @a dim.
    double symmError(size_t dim, ErrorCombScheme ecs=QUAD_COMB) const {
      Error1D errs = error(dim, ecs);
      return errs.symmErr();
    }

    /// Get the effective plus/minus-averaged error of this point in direction @a dim, passing an explicit combining functor.
    template <typename ERRORCOMBINER>
    double symmError(size_t dim, ERRORCOMBINER& ec) const {
      Error1D errs = error(dim, ec);
      return errs.symmErr();
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
    virtual void setErrors(const std::vector< PointError<N> >& errors) {
      _errors = errors;
    }


    // virtual void setErrors(const std::vector<std::vector<std::pair<double,double> > >& errors);
    // virtual void setErrors(const std::vector<std::vector<double> >& errors);

    /// Add an error to this point.
    virtual void addError(const PointError<N>& error) {
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

    nvector<double,N> _value;
    std::vector< PointError<N> > _errors;
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
  //};


}

#endif
